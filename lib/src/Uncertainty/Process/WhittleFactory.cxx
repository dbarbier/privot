//                                               -*- C++ -*-
/**
 *  @file  WhittleFactory.cxx
 *  @brief An interface for all implementation class of process
 *
 *  Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 *  This library is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  along with this library.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  @author schueller
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "PersistentObjectFactory.hxx"
#include "WhittleFactory.hxx"
#include "Exception.hxx"
#include "WelchFactory.hxx"
#include "ARMACoefficients.hxx"
#include "UserDefinedSpectralModel.hxx"
#include "SpecFunc.hxx"
#include "algocobyla.h"
#include "Cobyla.hxx"
#include "Log.hxx"
#include "Normal.hxx"
#include "UniVariatePolynomial.hxx"

BEGIN_NAMESPACE_OPENTURNS


TEMPLATE_CLASSNAMEINIT(PersistentCollection<WhittleFactoryState>);
static Factory<PersistentCollection<WhittleFactoryState> > RegisteredFactory2("PersistentCollection<WhittleFactoryState>");

CLASSNAMEINIT(WhittleFactory);

static Factory<WhittleFactory> RegisteredFactory("WhittleFactory");


/* Default constructor */
WhittleFactory::WhittleFactory(const String & name)
  : ARMAFactoryImplementation(name),
    spectralFactory_(WelchFactory()),
    normalizedFrequencies_(0),
    timeGrid_(),
    m_(0),
    spectralDensity_(),
    sigma2_(0.0),
    verbose_(false),
    isHistoryEnabled_(true),
    history_(0),
    startingPoints_(0)
{
  // Nothing to do
}

/* Standard constructor */
WhittleFactory::WhittleFactory(const UnsignedLong p,
                               const UnsignedLong q,
                               const Bool invertible,
                               const String & name)
  : ARMAFactoryImplementation(p, q, invertible, name),
    spectralFactory_(WelchFactory()),
    normalizedFrequencies_(0),
    timeGrid_(),
    m_(0),
    spectralDensity_(),
    sigma2_(0.0),
    verbose_(false),
    isHistoryEnabled_(true),
    history_(0),
    startingPoints_(0)
{
  // Create the set of starting points using the parameters in the ResourceMap
  initializeStartingPoints();
}

/* Standard constructor */
WhittleFactory::WhittleFactory(const Indices & p,
                               const Indices & q,
                               const Bool invertible,
                               const String & name)
  : ARMAFactoryImplementation(p, q, invertible, name),
    spectralFactory_(WelchFactory()),
    normalizedFrequencies_(0),
    timeGrid_(),
    m_(0),
    spectralDensity_(),
    sigma2_(0.0),
    verbose_(false),
    isHistoryEnabled_(true),
    history_(0),
    startingPoints_(0)
{
  // Create the set of starting points using the parameters in the ResourceMap
  initializeStartingPoints();
}

/* Virtual constructor */
WhittleFactory * WhittleFactory::clone() const
{
  return new WhittleFactory(*this);
}
/* Estimate the spectral density */
void WhittleFactory::buildSpectralDensity(const TimeSeries & timeSeries) const
{
  timeGrid_ = timeSeries.getTimeGrid();
  // Estimate a spectral model
  computeSpectralDensity(spectralFactory_.build(timeSeries));
}

/* Estimate the spectral density */
void WhittleFactory::buildSpectralDensity(const ProcessSample & sample) const
{
  timeGrid_ = sample.getTimeGrid();
  // Estimate a spectral model
  computeSpectralDensity(spectralFactory_.build(sample));
}

/* Compute the spectral density in normalized frequencies */
void WhittleFactory::computeSpectralDensity(const SpectralModel & spectralModel) const
{
  // We get the frequency grid and fix it for computation purposes
  const RegularGrid frequencyGrid(spectralModel.getFrequencyGrid());
  m_ = frequencyGrid.getN();
  // Index of the first frequency to take into account
  UnsignedLong kStart(0);
  // Suppress first value if the associated frequency is zero
  if (frequencyGrid.getStart() == 0.0)
    {
      --m_;
      kStart = 1;
    }
  const NumericalScalar fMax(frequencyGrid.getEnd());
  normalizedFrequencies_ = NumericalPoint(m_);
  spectralDensity_ = NumericalPoint(m_);
  NumericalScalar dt(timeGrid_.getStep());
  for (UnsignedLong k = 0; k < m_ ; ++k)
    {
      const NumericalScalar frequency(frequencyGrid.getValue(k + kStart));
      const NumericalScalar estimatedValue(std::real(spectralModel.computeSpectralDensity(frequency)(0, 0)) / dt);
      spectralDensity_[k] = estimatedValue;
      normalizedFrequencies_[k] = M_PI * frequency / fMax;
    }
}

NumericalScalar WhittleFactory::computeLogLikelihood(const NumericalPoint & theta) const
{
  NumericalScalar logTerm(0.0);
  NumericalScalar ratioTerm(0.0);
  for (UnsignedLong j = 0; j < m_; ++j)
    {
      const NumericalScalar frequency(normalizedFrequencies_[j]);
      // Gj computation
      const UnsignedLong n(std::max(currentP_, currentQ_));
      NumericalComplex numerator(1.0, 0.0);
      NumericalComplex denominator(1.0, 0.0);
      NumericalComplex y(std::polar(1.0, -frequency));
      NumericalComplex z(1.0, 0.0);
      for (UnsignedLong i = 0; i < n; ++i)
        {
          z *= y;
          if (i < currentQ_) numerator += theta[currentP_ + i] * z;
          if (i < currentP_) denominator += theta[i] * z;
        }
      const NumericalScalar gJ(std::norm(numerator) / std::norm(denominator));
      // Whittle likelihood update
      logTerm += log(gJ);
      ratioTerm += spectralDensity_[j] / gJ;
    }
  sigma2_ = ratioTerm / m_;
  return -logTerm - m_ * log(ratioTerm);
}

/* objective function ==> function to be optimized */
int WhittleFactory::ComputeObjectiveAndConstraint(int n,
                                                  int m,
                                                  double *x,
                                                  double *f,
                                                  double *con,
                                                  void *state)
{
  const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("WhittleFactory-RootEpsilon"));

  NumericalPoint theta(n);
  for (UnsignedLong k = 0; k < static_cast<UnsignedLong>(n); ++k) theta[k] = x[k];

  WhittleFactory * factory = static_cast<WhittleFactory *>(state);
  *f = -factory->computeLogLikelihood( theta );
  // AR dimension
  const UnsignedLong p(factory->getCurrentP());
  const UnsignedLong q(factory->getCurrentQ());
  UnsignedLong constraintIndex(0);
  // If not pure MA, check the roots of the AR polynom
  if (p > 0)
    {
      NumericalPoint arCoefficients(p + 1, 1.0);
      for (UnsignedLong i = 0; i < p; ++i) arCoefficients[i + 1] = theta[i];
      UniVariatePolynomial polynom(arCoefficients);
      // Check the roots only if the polynom is not constant
      if (polynom.getDegree() > 0)
        {
          Collection<NumericalComplex> roots(polynom.getRoots());
          NumericalScalar minRootModule(std::norm(roots[0]));
          for (UnsignedLong i = 1; i < p; ++i)
            {
              const NumericalScalar rootModule(std::norm(roots[i]));
              if (rootModule < minRootModule) minRootModule = rootModule;
            }
          con[constraintIndex] = minRootModule - 1.0 - epsilon;
        }
      // For constant polynom, assume root 0
      else con[constraintIndex] = -1.0 - epsilon;
      ++constraintIndex;
    }
  // If invertible and not pure AR, check the roots of the MA polynom
  if (factory->getInvertible() && q > 0)
    {
      NumericalPoint maCoefficients(q + 1, 1.0);
      for (UnsignedLong i = 0; i < q; ++i) maCoefficients[i + 1] = theta[i + p];
      UniVariatePolynomial polynom(maCoefficients);
      // Check the roots only if the polynom is not constant
      if (polynom.getDegree() > 0)
        {
          Collection<NumericalComplex> roots(polynom.getRoots());
          NumericalScalar minRootModule(std::norm(roots[0]));
          for (UnsignedLong i = 1; i < q; ++i)
            {
              const NumericalScalar rootModule(std::norm(roots[i]));
              if (rootModule < minRootModule) minRootModule = rootModule;
            }
          con[constraintIndex] = minRootModule - 1.0 - epsilon;
        }
      // For constant polynom, assume root 0
      else con[constraintIndex] = -1.0 - epsilon;
    }
  return 0;
}


/* String converter */
String WhittleFactory::__repr__() const
{
  OSS oss;
  oss << "class= " << WhittleFactory::GetClassName()
      << " p = " << p_ << " q = " << q_
      << " spectralModelFactory = " << spectralFactory_;
  return oss;
}

/* String converter */
String WhittleFactory::__str__(const String & offset) const
{
  return this->__repr__();
}

/* SpectralModelFactory set acccessor */
SpectralModelFactory WhittleFactory::getSpectralModelFactory() const
{
  return spectralFactory_;
}

/* Verbosity accessor */
Bool WhittleFactory::getVerbose() const
{
  return verbose_;
}

void WhittleFactory::setVerbose(const Bool verbose)
{
  verbose_ = verbose;
}

/* Enable or disable the estimation history */
void WhittleFactory::enableHistory() const
{
  isHistoryEnabled_ = true;
}

void WhittleFactory::disableHistory() const
{
  isHistoryEnabled_ = false;
}

Bool WhittleFactory::isHistoryEnabled() const
{
  return isHistoryEnabled_;
}

void WhittleFactory::resetHistory() const
{
  history_ = Collection< WhittleFactoryState >(0);
}

Collection< WhittleFactoryState > WhittleFactory::getHistory() const
{
  return history_;
}

/* SpectralModelFactory get acccessor */
void WhittleFactory::setSpectralModelFactory(const SpectralModelFactory & factory)
{
  spectralFactory_ = factory;
}

/* Build method
 * The Whittle likelihood is a ratio of spectral densities:
 * \log L(\theta, \sigma^2) = -\sum_{j=1}^m\log f(\lambda_j|\theta, \sigma^2)-\frac{1}{2\pi}\sum_{j=1}^m\frac{I(\lambda_j)}{f(\lambda_j|\theta,\sigma^2)}
 * with:
 * I(\lambda_j]=\frac{1}{n}\left|\sum_{k=1}^n w_ky_k\exp(-ik\lambda_j\right|^2
 * f(\lambda | \theta, \sigma^~)=\frac{\sigma^2}{2\pi}\left|\frac{\beta(\exp(-i\lambda))}{\alpha(\exp(-i\lambda))}\right|^2
 * and \theta = (\alpha, \lambda), m=ceil((n-1)/2)
 *
 * In its reduced form, we have:
 * \log L(\theta) = m\log(2\pi)-m\log\left[\frac{1}{m}\sum_{j=1}^m\frac{I(\lambda_j)}{g(\lambda_j|\theta)}\right]-\sum_{j=1}^m\log g(\lambda_j|\theta)-m
 * and
 * \sigma^2=\frac{1}{m}\sum_{j=1}^m\frac{I(\lambda_j)}{g(\lambda_j|\theta)}
 * with g(\lambda|\theta)=\frax{2\pi}{\sigma^2}f(\lambda|\theta,\sigma^2)
 *                       =\left|\frac{\beta(\exp(-i\lambda))}{\alpha(\exp(-i\lambda))}\right|^2
 * It can be noticed that dropping the term -\sum_{j=1}^m\log f(\lambda_j|\theta, \sigma^2) in the Whittle likelihood or the term
 */
ARMA * WhittleFactory::build(const TimeSeries & timeSeries,
                             NumericalPoint & informationCriteria) const
{
  if (timeSeries.getDimension() != 1)
    throw NotYetImplementedException(HERE) << "Currently implemented for 1 d case only";

  // Compute the tapered periodogramme for the time series using the Welch method
  // The computation is done once
  buildSpectralDensity(timeSeries);
  return maximizeLogLikelihood(informationCriteria);
}

ARMA * WhittleFactory::build(const TimeSeries & timeSeries) const
{
  NumericalPoint informationCriteria;
  return build(timeSeries, informationCriteria);
}

/* Build method */
ARMA * WhittleFactory::build(const ProcessSample & sample,
                             NumericalPoint & informationCriteria) const
{
  if (sample.getDimension() != 1)
    throw NotYetImplementedException(HERE) << "Currently implemented for 1 d case only";

  // Compute the tapered periodogramme for the process sample using the Welch method
  buildSpectralDensity(sample);
  return maximizeLogLikelihood(informationCriteria);
}

ARMA * WhittleFactory::build(const ProcessSample & sample) const
{
  NumericalPoint informationCriteria;
  return build(sample, informationCriteria);
}

/* Do the likelihood maximization */
ARMA * WhittleFactory::maximizeLogLikelihood(NumericalPoint & informationCriteria) const
{
  // First, clean the history
  resetHistory();
  const UnsignedLong sizeP(p_.getSize());
  const UnsignedLong sizeQ(q_.getSize());

  // Best parameters
  NumericalPoint bestTheta(0);
  NumericalScalar bestSigma2(0.0);
  NumericalPoint bestInformationCriteria(3, SpecFunc::MaxNumericalScalar);
  UnsignedLong bestP(0);
  UnsignedLong bestQ(0);

  UnsignedLong pointIndex(0);
  for (UnsignedLong pIndex = 0; pIndex < sizeP; ++pIndex)
    {
      currentP_ = p_[pIndex];
      for (UnsignedLong qIndex = 0; qIndex < sizeQ; ++qIndex)
        {
          currentQ_ = q_[qIndex];

          if (verbose_) LOGINFO(OSS() << "Current parameters p=" << currentP_ << ", q=" << currentQ_);
          // Dimension of the optimization problem
          int n(currentP_ + currentQ_);
          int m(0);
          // Check the minimum root module of the AR polynom only if not pure MA process
          if (currentP_ > 0) ++m;
          // Check the minimum root module of the MA polynom only if not pure AR process and if invertible
          if (invertible_ && currentQ_ > 0) ++m;
          // Current parameters vector
          NumericalPoint theta(startingPoints_[pointIndex]);

          // Optimize only if there is some ARMA parameters to estimate
          if (n > 0)
            {
              // use attributes to pass the data

              // Cobyla parameters
              // cobyla rhobeg ==>  a reasonable initial change to the variables
              // cobyla rhoend ==> the required accuracy for the variables
              // if the cobyla will be the default method, then we must add these variables in the ResourceMap
              // maxfun ==> on input, the maximum number of function evaluations on output, the number of function evaluations done
              NumericalScalar rhoBeg(ResourceMap::GetAsNumericalScalar("WhittleFactory-DefaultRhoBeg"));
              NumericalScalar rhoEnd(ResourceMap::GetAsNumericalScalar("WhittleFactory-DefaultRhoEnd"));
              int maxFun(static_cast<int>(ResourceMap::GetAsUnsignedLong("WhittleFactory-DefaultMaxFun")));
              // verbosity level
              cobyla_message message( verbose_ ? COBYLA_MSG_INFO : COBYLA_MSG_NONE );

              // call cobyla algo
              int returnCode(cobyla( n, m, &theta[0], rhoBeg, rhoEnd, message, &maxFun, WhittleFactory::ComputeObjectiveAndConstraint, (void*) this ));
              if (returnCode != 0)
                LOGWARN(OSS() << "Problem solving maximum likelihood problem by cobyla method, message=" << cobyla_rc_string[returnCode - COBYLA_MINRC]);
            }
          // Compute the information criteria
          // First, the corrected AIC
          const NumericalScalar logLikelihood(computeLogLikelihood(theta));
          NumericalPoint currentInformationCriteria(3);
          if (m_ > static_cast<UnsignedLong>(n + 2)) currentInformationCriteria[0] = -2.0 * logLikelihood + 2.0 * (n + 1) * m_ / (m_ - n - 2);
          else
            {
              LOGWARN(OSS() << "Warning! Unable to compute the corrected AIC criteria, too few data (" << m_ << ") for the model complexity (" << n + 1);
              currentInformationCriteria[0] = SpecFunc::MaxNumericalScalar;
            }
          // Second, the AIC
          currentInformationCriteria[1] = -2.0 * logLikelihood + 2.0 * (n + 1);
          // Third, the BIC
          currentInformationCriteria[2] = -2.0 * logLikelihood + 2.0 * (n + 1) * log(m_);
          if (verbose_) LOGINFO(OSS(false) << "Current estimate: theta=" << theta << ", sigma2=" << sigma2_ << ", Current information criteria=" << currentInformationCriteria);
          if (isHistoryEnabled_) history_.add(WhittleFactoryState(currentP_, theta, sigma2_, currentInformationCriteria, timeGrid_));
          // Keep the best model according to the first criteria
          if (currentInformationCriteria[0] < bestInformationCriteria[0])
            {
              bestInformationCriteria = currentInformationCriteria;
              bestSigma2 = sigma2_;
              bestTheta = theta;
              bestP = currentP_;
              bestQ = currentQ_;
            }
          if (verbose_) LOGINFO(OSS(false) << "Best so far: p=" << bestP << ", q=" << bestQ << ", theta=" << bestTheta << ", sigma2=" << bestSigma2 << ", information criteria=" << bestInformationCriteria);
          ++pointIndex;
        } // Loop over q
    } // Loop over p
  // Return the best information criteria
  informationCriteria = bestInformationCriteria;
  return WhittleFactoryState(bestP, bestTheta, bestSigma2, bestInformationCriteria, timeGrid_).getARMA().clone();
}

/* Starting points accessor */
void WhittleFactory::setStartingPoints(const Collection< NumericalPoint > & startingPoints)
{
  const UnsignedLong sizeP(p_.getSize());
  const UnsignedLong sizeQ(q_.getSize());
  // First, check the size of the collection
  if (startingPoints.getSize() != sizeP * sizeQ) throw InvalidArgumentException(HERE) << "Error: the given collection has size=" << startingPoints.getSize() << " but should have size=" << sizeP * sizeQ;
  // Second, check that all the points have the correct dimension
  UnsignedLong k(0);
  for (UnsignedLong pIndex = 0; pIndex < sizeP; ++pIndex)
    {
      const UnsignedLong p(p_[pIndex]);
      for (UnsignedLong qIndex = 0; qIndex < sizeQ; ++qIndex)
        {
          const UnsignedLong q(q_[qIndex]);
          // Dimension of the optimization problem
          const UnsignedLong n(p + q);
          if (startingPoints[k].getSize() != n) throw InvalidArgumentException(HERE) << "Error: the point at index=" << k << " should have a size=" << n << " instead of size=" << startingPoints[k].getSize();
          ++k;
        }
    }
  // Everything is ok
  startingPoints_ = startingPoints;
}

Collection< NumericalPoint > WhittleFactory::getStartingPoints() const
{
  return startingPoints_;
}


/* Initialize the starting points using the ResourceMap */
void WhittleFactory::initializeStartingPoints()
{
  startingPoints_ = Collection< NumericalPoint >(0);
  // Initialization of the starting point
  const NumericalScalar theta0(ResourceMap::GetAsNumericalScalar("WhittleFactory-DefaultStartingPointScale"));
  const UnsignedLong sizeP(p_.getSize());
  const UnsignedLong sizeQ(q_.getSize());
  for (UnsignedLong pIndex = 0; pIndex < sizeP; ++pIndex)
    {
      const UnsignedLong p(p_[pIndex]);
      for (UnsignedLong qIndex = 0; qIndex < sizeQ; ++qIndex)
        {
          const UnsignedLong q(q_[qIndex]);
          // Dimension of the optimization problem
          const UnsignedLong n(p + q);
          NumericalPoint theta(n);
          if (n > 0)
            {
              theta[0] = theta0;
              for (UnsignedLong k = 1; k < static_cast<UnsignedLong>(n); ++k) theta[k] = 0.5 * theta[k - 1];
            }
          startingPoints_.add(theta);
        } // Loop over q
    } // Loop over p
}

/* Method save() stores the object through the StorageManager */
void WhittleFactory::save(Advocate & adv) const
{
  ARMAFactoryImplementation::save(adv);
  adv.saveAttribute( "spectralFactory_", spectralFactory_);
  adv.saveAttribute( "verbose_", verbose_);
  adv.saveAttribute( "isHistoryEnabled_", isHistoryEnabled_);
  adv.saveAttribute( "history_", history_);
  adv.saveAttribute( "startingPoints_", startingPoints_);
}

/* Method load() reloads the object from the StorageManager */
void WhittleFactory::load(Advocate & adv)
{
  ARMAFactoryImplementation::load(adv);
  adv.loadAttribute( "spectralFactory_", spectralFactory_);
  adv.loadAttribute( "verbose_", verbose_);
  adv.loadAttribute( "isHistoryEnabled_", isHistoryEnabled_);
  adv.loadAttribute( "history_", history_);
  adv.loadAttribute( "startingPoints_", startingPoints_);
}

END_NAMESPACE_OPENTURNS
