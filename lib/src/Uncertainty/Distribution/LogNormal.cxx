//                                               -*- C++ -*-
/**
 *  @file  LogNormal.cxx
 *  @brief The LogNormal distribution
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
#include <cmath>
#include "LogNormal.hxx"
#include "DistFunc.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Normal.hxx"
#include "HermiteFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(LogNormal);

static Factory<LogNormal> RegisteredFactory("LogNormal");

/* Default constructor */
LogNormal::LogNormal()
  : NonEllipticalDistribution("LogNormal"),
    muLog_(0.0),
    sigmaLog_(0.0),
    gamma_(0.0),
    // 1 / SQRT(2Pi)
    normalizationFactor_(0.39894228040143267794),
    H_(0),
    hermiteNodes_(0),
    hermiteWeights_(0)
{
  setDimension(1);
  // The arguments must be different from the initialization values, which is the case as sigmaLog_ is initialized by 0
  setMuLogSigmaLog(0.0, 1.0);
  computeRange();
}

/* Default constructor */
LogNormal::LogNormal(const NumericalScalar arg1,
                     const NumericalScalar arg2,
                     const NumericalScalar gamma,
                     const ParameterSet set)
  : NonEllipticalDistribution("LogNormal"),
    muLog_(0.0),
    sigmaLog_(0.0),
    gamma_(gamma),
    normalizationFactor_(0.0),
    H_(0.0),
    hermiteNodes_(0),
    hermiteWeights_(0)
{
  // Adapt the integration nodes number to the needs of the characteristic function integration
  switch (set)
    {
    case MUSIGMA_LOG:
      // This call set also the range.
      // The arguments must be different from the initialization values, which is the case as sigmaLog_ is initialized by 0
      setMuLogSigmaLog(arg1, arg2);
      break;

    case MUSIGMA:
      // This call set also the range.
      setMuSigma(arg1, arg2);
      break;

    case MU_SIGMAOVERMU:
      if (arg1 == 0.0) throw InvalidArgumentException(HERE) << "Error: mu cannot be null in the parameter set (mu, sigmaOverMu)";
      // This call set also the range.
      setMuSigma(arg1, arg1 * arg2);
      break;

    default:
      throw InvalidArgumentException(HERE) << "Invalid parameter set argument";

    } /* end switch */
  normalizationFactor_ = 1.0 / (sigmaLog_ * sqrt(2.0 * M_PI));
  setDimension(1);
}

/* Comparison operator */
Bool LogNormal::operator ==(const LogNormal & other) const
{
  if (this == &other) return true;
  return (muLog_ == other.muLog_) && (sigmaLog_ == other.sigmaLog_) && (gamma_ == other.gamma_);
}

/* String converter */
String LogNormal::__repr__() const
{
  OSS oss;
  oss << "class=" << LogNormal::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " muLog=" << muLog_
      << " sigmaLog=" << sigmaLog_
      << " gamma=" << gamma_;
  return oss;
}

String LogNormal::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(muLog = " << muLog_ << ", sigmaLog = " << sigmaLog_ << ", gamma = " << gamma_ << ")";
  return oss;
}

/* Virtual constructor */
LogNormal * LogNormal::clone() const
{
  return new LogNormal(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void LogNormal::computeRange()
{
  NumericalPoint lowerBound(1, gamma_);
  const NumericalPoint upperBound(computeUpperBound());
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Get one realization of the distribution */
NumericalPoint LogNormal::getRealization() const
{
  return NumericalPoint(1, gamma_ + exp(muLog_ + sigmaLog_ * DistFunc::rNormal()));
}



/* Get the DDF of the distribution */
NumericalPoint LogNormal::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return NumericalPoint(1, 0.0);
  NumericalScalar v(sigmaLog_ * sigmaLog_);
  return NumericalPoint(1, (muLog_ - log(x) - v) / (v * x) * computePDF(point));
}


/* Get the PDF of the distribution */
NumericalScalar LogNormal::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - gamma_);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return 0.0;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  return normalizationFactor_ * exp(-0.5 * logX * logX) / x;
}

NumericalScalar LogNormal::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - gamma_);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return -SpecFunc::MaxNumericalScalar;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  return log(normalizationFactor_) - 0.5 * logX * logX - log(x);
}

/* Get the CDF of the distribution */
NumericalScalar LogNormal::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return 0.0;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  return DistFunc::pNormal(logX);
}

NumericalScalar LogNormal::computeComplementaryCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return 1.0;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  return DistFunc::pNormal(logX, true);
}


/* Compute the integrand that is involved in the computation of the characteristic function */
NumericalComplex LogNormal::characteristicIntegrand(const NumericalScalar eta,
                                                    const NumericalScalar sStar) const
{
  return exp(NumericalComplex(-sStar * (eta - exp(sigmaLog_ * eta) / sigmaLog_), -M_PI * eta / (2.0 * sigmaLog_)));
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X))
 * Algorithm adapted from:
 * John A. Gubner, "A New Formula for Lognormal Characteristic Functions",
 * IEEE transactions on vehicular technology, vol. 55, no. 5, September 2006.
 */
NumericalComplex LogNormal::computeCharacteristicFunction(const NumericalScalar x) const
{
  // Quick return for null argument
  if (x == 0.0) return 1.0;
  return exp(computeLogCharacteristicFunction(x));
}

NumericalComplex LogNormal::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  // Quick return for null argument
  if (x == 0.0) return 0.0;
  // Compute the characteristic function for the positive arguments
  const NumericalScalar nu(fabs(x) * exp(muLog_));
  const NumericalScalar sigma2(sigmaLog_ * sigmaLog_);
  NumericalComplex logCFValue(0.0);
  // Quick return for small argument
  if (nu < 0.001 * exp(-1.5 * sigma2))
    {
      const NumericalScalar nu2(nu * nu);
      logCFValue = log(NumericalComplex(1.0 - 0.5 * nu2 * exp(2.0 * sigma2), nu * exp(0.5 * sigma2) * (1.0 - nu2 * exp(4.0 * sigma2) / 6.0)));
    }
  else
    {
      UnsignedLong integrationNodesNumber(ResourceMap::GetAsUnsignedLong("LogNormal-CharacteristicFunctionIntegrationNodes"));
      // Check if the integration nodes have to be recomputed
      if (integrationNodesNumber != hermiteNodes_.getSize()) hermiteNodes_ = HermiteFactory().getNodesAndWeights(integrationNodesNumber, hermiteWeights_);
      // Hermite integration of the Fourier transform for small sigma
      if (sigmaLog_ <= ResourceMap::GetAsNumericalScalar("LogNormal-CharacteristicFunctionSmallSigmaThreshold"))
        {
          // Adjust the integration nodes number
          // nu*exp(sigma*x_n) - nu * exp(sigma*x_{n-1}) < 2*\pi/8
          // nu*exp(sigma*x_n)(1 - exp(sigma*(x_{n-1}-x_n))) < 2*\pi/8
          // 1 - exp(sigma*(x_{n-1}-x_n)) < 2*\pi/(8*nu*exp(sigma*x_n))
          // sigma << 1
          // sigma(x_n-x_{n-1}) < 2*\pi/(8*nu)
          // const NumericalScalar maxPulsation(exp(sigmaLog_ * nodes[integrationNodesNumber - 1]));
          // const UnsignedLong minimumIntegrationNodesNumber(static_cast<UnsignedLong>(8 * 2 * M_PI * nu));
          // integrationNodesNumber = std::max(integrationNodesNumber, static_cast<UnsignedLong>(8 * 2 * M_PI * nu));
          NumericalComplex value(0.0);
          for (UnsignedLong i = 0; i < integrationNodesNumber; ++i) value += hermiteWeights_[i] * exp(NumericalComplex(0.0, nu * exp(sigmaLog_ * hermiteNodes_[i])));
          logCFValue = log(value);
        } // Small sigma
      else
        {
          // Hermite integration centered on the maximal amplitude
          // Compute the characteristic function for the positive arguments
          const NumericalScalar sStar(-SpecFunc::LambertW(sigmaLog_ * sigmaLog_ * nu) / sigmaLog_);
          NumericalComplex value(0.0);
          for (UnsignedLong i = 0; i < integrationNodesNumber; ++i) value += hermiteWeights_[i] * characteristicIntegrand(hermiteNodes_[i], sStar);
          logCFValue = log(value) + H_ - NumericalComplex(sStar * sStar / 2.0, M_PI * sStar / (2.0 * sigmaLog_));
        } // Large sigma
    } // Large argument
  logCFValue += NumericalComplex(0.0, x * gamma_);
  // Use symmetry for negative arguments
  if (x < 0.0) logCFValue = conj(logCFValue);
  return logCFValue;
}

/* Get the PDFGradient of the distribution */
NumericalPoint LogNormal::computePDFGradient(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  NumericalPoint pdfGradient(3, 0.0);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return pdfGradient;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  NumericalScalar pdf(normalizationFactor_ * exp(-0.5 * logX * logX) / x);
  pdfGradient[0] = pdf * logX / sigmaLog_;
  pdfGradient[1] = pdf * (logX - 1.0) * (logX + 1.0) / sigmaLog_;
  pdfGradient[2] = pdf * (1.0 + logX / sigmaLog_) / x;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint LogNormal::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalScalar x(point[0] - gamma_);
  NumericalPoint cdfGradient(3, 0.0);
  // Here we keep the bound within the special case as the distribution is continuous
  if (x <= 0.0) return cdfGradient;
  NumericalScalar logX((log(x) - muLog_) / sigmaLog_);
  NumericalScalar pdf(normalizationFactor_ * exp(-0.5 * logX * logX) / x);
  cdfGradient[0] = -x * pdf;
  cdfGradient[1] = -logX * x * pdf;
  cdfGradient[2] = -pdf;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar LogNormal::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  return gamma_ + exp(muLog_ + sigmaLog_ * DistFunc::qNormal(prob, tail));
}

/* Compute the mean of the distribution */
void LogNormal::computeMean() const
{
  mean_ = NumericalPoint(1, getMu());
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint LogNormal::getStandardDeviation() const
{
  return NumericalPoint(1, getSigma());
}

/* Get the skewness of the distribution */
NumericalPoint LogNormal::getSkewness() const
{
  NumericalScalar expSigmaLog2(exp(sigmaLog_ * sigmaLog_));
  return NumericalPoint(1, (expSigmaLog2 + 2.0) * sqrt(expSigmaLog2 - 1.0));
}

/* Get the kurtosis of the distribution */
NumericalPoint LogNormal::getKurtosis() const
{
  NumericalScalar expSigmaLog2(exp(sigmaLog_ * sigmaLog_));
  return NumericalPoint(1, -3.0 + expSigmaLog2 * expSigmaLog2 * (3.0 + expSigmaLog2 * (2.0 + expSigmaLog2)));
}

/* Get the moments of the standardized distribution */
NumericalPoint LogNormal::getStandardMoment(const UnsignedLong n) const
{
  return NumericalPoint(1, exp(n * muLog_ + 0.5 * pow(n * sigmaLog_, 2)));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
LogNormal::Implementation LogNormal::getStandardRepresentative() const
{
  return LogNormal(muLog_, sigmaLog_, 0.0).clone();
}

/* Compute the covariance of the distribution */
void LogNormal::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar expSigmaLog2(exp(sigmaLog_ * sigmaLog_));
  covariance_(0, 0) = expSigmaLog2 * exp(2.0 * muLog_) * (expSigmaLog2 - 1.0);
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
LogNormal::NumericalPointWithDescriptionCollection LogNormal::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(3);
  Description description(point.getDimension());
  point[0] = muLog_;
  point[1] = sigmaLog_;
  point[2] = gamma_;
  description[0] = "muLog";
  description[1] = "sigmaLog";
  description[2] = "gamma";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void LogNormal::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = LogNormal(parametersCollection[0][0], parametersCollection[0][1], parametersCollection[0][2]);
}


/* Interface specific to LogNormal */

/* MuLogSigmaLog accessor */
void LogNormal::setMuLogSigmaLog(const NumericalScalar muLog,
                                 const NumericalScalar sigmaLog)
{
  if (sigmaLog <= 0.0) throw InvalidArgumentException(HERE) << "SigmaLog MUST be positive, here sigmaLog=" << sigmaLog;
  if ((muLog != muLog_) || (sigmaLog != sigmaLog_))
    {
      muLog_ = muLog;
      sigmaLog_ = sigmaLog;
      // Check if the parameters values are not crazy
      static const Interval range(Normal().getRange());
      const NumericalScalar rMin(muLog_ + range.getUpperBound()[0] * sigmaLog_);
      const NumericalScalar rMax(muLog_ + range.getLowerBound()[0] * sigmaLog_);
      if ((rMin >= SpecFunc::LogMaxNumericalScalar) ||
          (rMax <= SpecFunc::LogMinNumericalScalar)) throw InvalidArgumentException(HERE) << "MuLog=" << muLog_ << " and SigmaLog=" << sigmaLog_ << " lead to a LogNormal distribution with a too much wide range";
      H_ = M_PI * M_PI / (8.0 * sigmaLog_ * sigmaLog_);
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* MuLog accessor */
void LogNormal::setMuLog(const NumericalScalar muLog)
{
  if (muLog != muLog_)
    {
      muLog_ = muLog;
      // Check if the parameters values are not crazy
      static const Interval range(Normal().getRange());
      const NumericalScalar rMin(muLog_ + range.getUpperBound()[0] * sigmaLog_);
      const NumericalScalar rMax(muLog_ + range.getLowerBound()[0] * sigmaLog_);
      if ((rMin >= SpecFunc::LogMaxNumericalScalar) ||
          (rMax <= SpecFunc::LogMinNumericalScalar)) throw InvalidArgumentException(HERE) << "MuLog and SigmaLog lead to a LogNormal distribution with a too much wide range";
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar LogNormal::getMuLog() const
{
  return muLog_;
}


/* SigmaLog accessor */
void LogNormal::setSigmaLog(const NumericalScalar sigmaLog)
{
  if (sigmaLog <= 0.) throw InvalidArgumentException(HERE) << "SigmaLog MUST be positive, here sigmaLog=" << sigmaLog;
  if (sigmaLog != sigmaLog_)
    {
      sigmaLog_ = sigmaLog;
      // Check if the parameters values are not crazy
      static const Interval range(Normal().getRange());
      const NumericalScalar rMin(muLog_ + range.getUpperBound()[0] * sigmaLog_);
      const NumericalScalar rMax(muLog_ + range.getLowerBound()[0] * sigmaLog_);
      if ((rMin >= SpecFunc::LogMaxNumericalScalar) ||
          (rMax <= SpecFunc::LogMinNumericalScalar)) throw InvalidArgumentException(HERE) << "MuLog and SigmaLog lead to a LogNormal distribution with a too much wide range";
      H_ = pow(M_PI / (2.0 * sigmaLog_), 2.0) / 2.0 - 0.5 * log(2.0 * M_PI);
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar LogNormal::getSigmaLog() const
{
  return sigmaLog_;
}

NumericalScalar LogNormal::getMu() const
{
  return gamma_ + exp(muLog_ + 0.5 * sigmaLog_ * sigmaLog_);
}


/* Sigma accessor */
void LogNormal::setMuSigma(const NumericalScalar mu,
                           const NumericalScalar sigma)
{
  if (sigma <= 0.0) throw InvalidArgumentException(HERE) << "Error: sigma must be > 0, here sigma=" << sigma;
  if (mu <= gamma_) throw InvalidArgumentException(HERE) << "Error: mu must be greater than gamma, here mu=" << mu << " and gamma=" << gamma_;
  NumericalScalar shift(mu - gamma_);
  NumericalScalar shiftSquared(shift * shift);
  NumericalScalar deltaSquareRoot(sqrt(shiftSquared + sigma * sigma));
  // This call takes care of the range and the mean and covariance flags
  setMuLogSigmaLog(log(shiftSquared / deltaSquareRoot), sqrt(2 * log(deltaSquareRoot / shift)));
}

NumericalScalar LogNormal::getSigma() const
{
  NumericalScalar expSigmaLog2(exp(sigmaLog_ * sigmaLog_));
  return exp(muLog_) * sqrt(expSigmaLog2 * (expSigmaLog2 - 1.0));
}

/* Gamma accessor */
void LogNormal::setGamma(const NumericalScalar gamma)
{
  if (gamma != gamma_)
    {
      gamma_ = gamma;
      isAlreadyComputedMean_ = false;
      // The covariance does not depends on gamma
      computeRange();
    }
}

NumericalScalar LogNormal::getGamma() const
{
  return gamma_;
}

/* SigmaOverMu accessor */
NumericalScalar LogNormal::getSigmaOverMu() const
{
  NumericalScalar mu(getMu());
  if (mu == 0.0) throw NotDefinedException(HERE) << "Error: trying to get sigmaOverMu with mu equals to zero";
  return getSigma() / mu;
}

/* Method save() stores the object through the StorageManager */
void LogNormal::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "muLog_", muLog_ );
  adv.saveAttribute( "sigmaLog_", sigmaLog_ );
  adv.saveAttribute( "gamma_", gamma_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
  adv.saveAttribute( "H_", H_ );
}

/* Method load() reloads the object from the StorageManager */
void LogNormal::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "muLog_", muLog_ );
  adv.loadAttribute( "sigmaLog_", sigmaLog_ );
  adv.loadAttribute( "gamma_", gamma_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  adv.loadAttribute( "H_", H_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
