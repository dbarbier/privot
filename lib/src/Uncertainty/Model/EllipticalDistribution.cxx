//                                               -*- C++ -*-
/**
 *  @file  EllipticalDistribution.cxx
 *  @brief Abstract top-level class for elliptical distributions
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#include <cmath>
#include "EllipticalDistribution.hxx"
#include "NatafEllipticalDistributionEvaluation.hxx"
#include "NatafEllipticalDistributionGradient.hxx"
#include "NatafEllipticalDistributionHessian.hxx"
#include "InverseNatafEllipticalDistributionEvaluation.hxx"
#include "InverseNatafEllipticalDistributionGradient.hxx"
#include "InverseNatafEllipticalDistributionHessian.hxx"
#include "Description.hxx"
#include "PersistentObjectFactory.hxx"
#include "Lapack.hxx"
#include "IdentityMatrix.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(EllipticalDistribution);

static Factory<EllipticalDistribution> RegisteredFactory("EllipticalDistribution");

/* Default constructor */
EllipticalDistribution::EllipticalDistribution(const String & name)
  : ContinuousDistribution(name)
{
  // Nothing to do
}

/* Parameter constructor */
EllipticalDistribution::EllipticalDistribution(const NumericalPoint & mean,
                                               const NumericalPoint & sigma,
                                               const CorrelationMatrix & R,
                                               const NumericalScalar covarianceScalingFactor,
                                               const String & name)
  : ContinuousDistribution(name)
  , sigma_(sigma)
  , R_(R)
  , covarianceScalingFactor_(covarianceScalingFactor)
{
  const UnsignedLong dimension(R.getDimension());
  // We check if the inputs have the same dimension. If not, we throw an exception
  if ( (dimension != mean.getDimension()) ||
       (dimension != sigma.getDimension()) )
    throw InvalidArgumentException(HERE)
      << "Arguments have incompatible dimensions: R dimension=" << dimension
      << " sigma dimension=" << sigma.getDimension()
      << " mean dimension=" << mean.getDimension();
  // We check that the given correlation matrix is definite positive
  if ( !R_.isPositiveDefinite()) throw InvalidArgumentException(HERE) << "The correlation matrix must be definite positive R=" << R;
  // We check that the marginal standard deviations are > 0
  for(UnsignedLong i = 0; i < dimension; ++i)
    if (sigma[i] <= 0.0) throw InvalidArgumentException(HERE) << "The marginal standard deviations must be > 0 sigma=" << sigma;
  // Then we set the dimension of the Elliptical distribution
  setDimension(dimension);
  // We initialize the description
  Description description(dimension);
  // Set default description
  for (UnsignedLong i = 0; i < dimension; ++i) description[i] = OSS() << "marginal " << i + 1;
  setDescription(description);
  // The mean attribute is stored at an upper level
  mean_ = mean;
  // Compute the auxiliary attributes. It also set isAlreadyComputedMean to true
  update();
}


/* Comparison operator */
Bool EllipticalDistribution::operator ==(const EllipticalDistribution & other) const
{
  if (this == &other) return true;
  return DistributionImplementation::operator ==(other) &&
    (R_ == other.R_) &&
    (mean_ == other.mean_) &&
    (sigma_ == other.sigma_);
}

/* Centers and reduces a value u = Diag(sigma_)^(-1) * (x - mean_) */
NumericalPoint EllipticalDistribution::normalize(const NumericalPoint & x) const
{
  NumericalPoint u(x - mean_);
  for (UnsignedLong i = 0; i < getDimension(); ++i) u[i] /= sigma_[i];
  return u;
}

/* Decenters and scales a value x = mean_ + Diag(sigma_) * u */
NumericalPoint EllipticalDistribution::denormalize(const NumericalPoint & u) const
{
  NumericalPoint x(mean_);
  for (UnsignedLong i = 0; i < getDimension(); ++i) x[i] += sigma_[i] * u[i];
  return x;
}

/* String converter */
String EllipticalDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << EllipticalDistribution::GetClassName()
      << "parameters collection=" << getParametersCollection();
  return oss;
}

/* Tell if the distribution is elliptical */
Bool EllipticalDistribution::isElliptical() const
{
  return true;
}

/* Tell if the distribution has elliptical copula */
Bool EllipticalDistribution::hasEllipticalCopula() const
{
  return true;
}


/* Compute the density generator of the elliptical distribution, i.e.
 *  the function phi such that the density of the distribution can
 *  be written as p(x) = phi((x-mean_).C^{-1} * (x-mean_))                      */
NumericalScalar EllipticalDistribution::computeDensityGenerator(const NumericalScalar betaSquare) const
{
  throw NotYetImplementedException(HERE);
}

/* Compute the derivative of the density generator */
NumericalScalar EllipticalDistribution::computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const
{
  // Use centered finite difference
  const NumericalScalar epsilon(pow(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon"), 1.0 / 3.0));
  return (computeDensityGenerator(betaSquare + epsilon) - computeDensityGenerator(betaSquare - epsilon)) / epsilon;
}

/* Compute the seconde derivative of the density generator */
NumericalScalar EllipticalDistribution::computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const
{
  // Use centered finite difference
  const NumericalScalar epsilon(pow(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultPDFEpsilon"), 0.25));
  return (computeDensityGenerator(betaSquare + epsilon) - 2.0 * computeDensityGenerator(betaSquare) + computeDensityGenerator(betaSquare - epsilon)) / (epsilon * epsilon);
}

/* Get the DDF of the distribution */
NumericalPoint EllipticalDistribution::computeDDF(const NumericalPoint & point) const
{
  const NumericalPoint iLx(inverseCholesky_ * (point - mean_));
  const NumericalScalar betaSquare(iLx.norm2());
  return 2.0 * normalizationFactor_ * computeDensityGeneratorDerivative(betaSquare) * inverseCholesky_.transpose() * iLx;
}

/* Get the PDF of the distribution */
NumericalScalar EllipticalDistribution::computePDF(const NumericalPoint & point) const
{
  const NumericalPoint iLx(inverseCholesky_ * (point - mean_));
  const NumericalScalar betaSquare(iLx.norm2());
  return normalizationFactor_ * computeDensityGenerator(betaSquare);
}

/* Get the PDF gradient of the distribution */
NumericalPoint EllipticalDistribution::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalPoint minusGardientMean(computeDDF(point));
  const UnsignedLong dimension(getDimension());
  const NumericalPoint u(normalize(point));
  const NumericalPoint iRu(inverseR_ * u);
  const NumericalScalar betaSquare(dot(u, iRu));
  const NumericalScalar phi(computeDensityGenerator(betaSquare));
  const NumericalScalar phiDerivative(computeDensityGeneratorDerivative(betaSquare));
  NumericalPoint pdfGradient(2 * dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalScalar iSigma(1.0 / sigma_[i]);
      // dPDF / dmu_i
      pdfGradient[i] = -2.0 * normalizationFactor_ * phiDerivative * iRu[i] * iSigma;
      // dPDF / dsigma_i
      pdfGradient[dimension + i] = pdfGradient[i] * u[i] - normalizationFactor_ * phi * iSigma;
    }
  return pdfGradient;
}

/* Update the derivative attributes */
void EllipticalDistribution::update()
{
  const UnsignedLong dimension(getDimension());
  shape_ = CovarianceMatrix(dimension);
  // Build the shape matrix
  shape_ = R_;
  for (UnsignedLong i = 0; i < dimension; ++i)
    for (UnsignedLong j = 0; j <= i; ++j)
      shape_(i, j) *= sigma_[i] * sigma_[j];
  // Compute its Cholesky factor
  cholesky_ = shape_.computeCholesky();
  // Inversion of the Cholesky factor using the dtrsm blas level 3 routine
  // side tells if we solve M.X = alpha.B or X.M = alpha.B
  char side('L');
  int lside(1);
  // M must be triangular. uplo tells if it is upper or lower triangular
  char uplo('L');
  int luplo(1);
  // transa tells if M is transposed or not
  char transa('N');
  int ltransa(1);
  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);
  // the row dimension of M
  int m(dimension);
  // the column dimension of M
  int n(dimension);
  // we solve the case alpha=1
  double alpha(1.0);
  // leading dimension of M
  int lda(dimension);
  // leading dimension of B
  int ldb(dimension);
  // As we want to inverse M, we set B = Id
  inverseCholesky_ = IdentityMatrix(dimension);
  // B stores the result of the routine
  DTRSM_F77(&side, &uplo, &transa, &diag, &m, &n, &alpha, const_cast<double*>(&((*cholesky_.getImplementation())[0])), &lda, const_cast<double*>(&((*inverseCholesky_.getImplementation())[0])), &ldb, &lside, &luplo, &ltransa, &ldiag);

  // Inverse the correlation matrix R = D^(-1).L.L'.D^(-1)
  // R^(-1) = D.L^(-1).L^(-1)'.D
  inverseR_ = SymmetricMatrix(dimension);
  const SquareMatrix inverseShape(inverseCholesky_.transpose() * inverseCholesky_);
  for (UnsignedLong i = 0; i < dimension; ++i)
    for (UnsignedLong j = 0; j <= i; ++j)
      inverseR_(i, j) = sigma_[i] * inverseShape(i, j) * sigma_[j];
  normalizationFactor_ = 1.0;
  for (UnsignedLong i = 0; i < dimension; ++i) normalizationFactor_ /= cholesky_(i, i);
  isAlreadyComputedMean_ = true;
}

/* Mean point accessor */
void EllipticalDistribution::setMean(const NumericalPoint & mean)
{
  if (mean.getDimension() != getDimension())
    throw InvalidArgumentException(HERE)
      << "Mean point dimension (" << mean.getDimension()
      << ") differ from distribution dimension(" << getDimension()
      << "). Unable to construct EllipticalDistribution distribution object.";
  mean_ = mean;
  isAlreadyComputedMean_ = true;
  computeRange();
}

/* Mean computation */
void EllipticalDistribution::computeMean() const
{
  // Just set the flag as it may have been changed e.g if the integration nodes number has changed
  isAlreadyComputedMean_ = true;
}

/* Covariance matrix computation */
void EllipticalDistribution::computeCovariance() const
{
  // We have to extract the implementation because we know that the result
  // is a valid covariance matrix, but it cannot be infered by the C++
  // from the operands
  covariance_ = (covarianceScalingFactor_ * shape_).getImplementation();
  isAlreadyComputedCovariance_ = true;
}

/* Sigma accessor */
void EllipticalDistribution::setSigma(const NumericalPoint & sigma)
{
  if (sigma.getDimension() != getDimension())
    throw InvalidArgumentException(HERE)
      << "Sigma dimension (" << sigma.getDimension()
      << ") differ from distribution dimension(" << getDimension()
      << "). Unable to construct elliptical distribution object.";

  // We check that the marginal standard deviations are > 0
  for(UnsignedLong i = 0; i < sigma.getDimension(); ++i)
    if (sigma[i] <= 0.0) throw InvalidArgumentException(HERE) << "The marginal standard deviations must be > 0 sigma=" << sigma;
  sigma_ = sigma;
  update();
  computeRange();
  isAlreadyComputedCovariance_ = false;
}

/* Sigma accessor */
NumericalPoint EllipticalDistribution::getSigma() const
{
  return sigma_;
}

/* Get the standard deviation of the distribution.
   Warning! This method MUST be overloaded for elliptical distributions without finite second moment:
   it is possible to have a well-defined sigma vector but no standard deviation, think about Stundent
   distribution with nu < 2 */
NumericalPoint EllipticalDistribution::getStandardDeviation() const
{
  return sqrt(covarianceScalingFactor_) * sigma_;
}

/* Correlation matrix accessor */
void EllipticalDistribution::setCorrelation(const CorrelationMatrix & R)
{
  if (R.getDimension() != getDimension())
    throw InvalidArgumentException(HERE)
      << "Correlation Matrix dimension (" << R.getDimension()
      << ") differ from distribution dimension(" << getDimension()
      << "). Unable to construct elliptical distribution object.";

  // We check that the given correlation matrix is definite positive
  if ( !const_cast<CorrelationMatrix*>(&R)->isPositiveDefinite()) throw InvalidArgumentException(HERE) << "The correlation matrix must be definite positive R=" << R;
  R_ = R;
  update();
  isAlreadyComputedCovariance_ = false;
}

/* Correlation matrix accessor */
CorrelationMatrix EllipticalDistribution::getCorrelation() const
{
  return R_;
}

/* Inverse correlation matrix accessor */
SquareMatrix EllipticalDistribution::getInverseCorrelation() const
{
  return inverseR_;
}

/* Cholesky factor of the correlation matrix accessor */
SquareMatrix EllipticalDistribution::getCholesky() const
{
  return cholesky_;
}

/* Inverse of the Cholesky factor of the correlation matrix accessor */
SquareMatrix EllipticalDistribution::getInverseCholesky() const
{
  return inverseCholesky_;
}

/* Virtual copy constructor */
EllipticalDistribution * EllipticalDistribution::clone() const
{
  return new EllipticalDistribution(*this);
}

/* Get the isoprobabilist transformation */
EllipticalDistribution::IsoProbabilisticTransformation EllipticalDistribution::getIsoProbabilisticTransformation() const
{
  IsoProbabilisticTransformation transform;
  transform.setEvaluationImplementation(new NatafEllipticalDistributionEvaluation(mean_, inverseCholesky_));
  transform.setGradientImplementation(new NatafEllipticalDistributionGradient(inverseCholesky_));
  transform.setHessianImplementation(new NatafEllipticalDistributionHessian(getDimension()));
  // Set the parameters values and descriptions
  // The result of parametersGradient is given
  // in the following form:
  // (d/dmu, d/dsigma)
  // There is no gradient according to the dependence parameters yet (28/10/2006)
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescription parameters(2 * dimension);
  Description description(parameters.getDimension());
  NumericalPointWithDescriptionCollection parametersCollection(getParametersCollection());
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const Description parametersDescription(parametersCollection[i].getDescription());
      const String marginalName(parametersCollection[i].getName());
      parameters[i] = parametersCollection[i][0];
      parameters[dimension + i] = parametersCollection[i][1];
      description[i] = OSS() << marginalName << "_" << parametersDescription[0];
      description[dimension + i] = OSS() << marginalName << "_" << parametersDescription[1];
    }
  parameters.setDescription(description);
  transform.setParameters(parameters);
  return transform;
}

/* Get the inverse isoprobabilist transformation */
EllipticalDistribution::InverseIsoProbabilisticTransformation EllipticalDistribution::getInverseIsoProbabilisticTransformation() const
{
  InverseIsoProbabilisticTransformation inverseTransform;
  inverseTransform.setEvaluationImplementation(new InverseNatafEllipticalDistributionEvaluation(mean_, cholesky_));
  inverseTransform.setGradientImplementation(new InverseNatafEllipticalDistributionGradient(cholesky_));
  inverseTransform.setHessianImplementation(new InverseNatafEllipticalDistributionHessian(getDimension()));
  // Set the parameters values and descriptions
  // The result of parametersGradient is given
  // in the following form:
  // (d/dmu, d/dsigma)
  // There is no gradient according to the dependence parameters yet (28/10/2006)
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescription parameters(2 * dimension);
  Description description(parameters.getDimension());
  NumericalPointWithDescriptionCollection parametersCollection(getParametersCollection());
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      const Description parametersDescription(parametersCollection[i].getDescription());
      const String marginalName(parametersCollection[i].getName());
      parameters[i] = parametersCollection[i][0];
      parameters[dimension + i] = parametersCollection[i][1];
      description[i] = OSS() << marginalName << "_" << parametersDescription[0];
      description[dimension + i] = OSS() << marginalName << "_" << parametersDescription[1];
    }
  parameters.setDescription(description);
  inverseTransform.setParameters(parameters);
  return inverseTransform;
}

/* Get the standard distribution, i.e. a distribution of the same kind but with zero mean,
 * unit marginal standard distribution and identity correlation */
EllipticalDistribution::Implementation EllipticalDistribution::getStandardDistribution() const
{
  EllipticalDistribution * p_standardDistribution(clone());
  const UnsignedLong dimension(getDimension());
  p_standardDistribution->setMean(NumericalPoint(dimension, 0.0));
  p_standardDistribution->setSigma(NumericalPoint(dimension, 1.0));
  p_standardDistribution->setCorrelation(CorrelationMatrix(dimension));
  return p_standardDistribution;
}

/* Parameters value and description accessor */
EllipticalDistribution::NumericalPointWithDescriptionCollection EllipticalDistribution::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  NumericalPointWithDescriptionCollection parameters(dimension + (dimension > 1 ? 1 : 0));
  // First put the marginal parameters
  const Description description(getDescription());
  for (UnsignedLong marginalIndex = 0; marginalIndex < dimension; ++marginalIndex)
    {
      NumericalPointWithDescription point(2);
      Description marginalDescription(point.getDimension());
      point[0] = mean_[marginalIndex];
      point[1] = sigma_[marginalIndex];
      marginalDescription[0] = "mean";
      marginalDescription[1] = "standard deviation";
      point.setDescription(marginalDescription);
      point.setName(description[marginalIndex]);
      parameters[marginalIndex] = point;
    } // marginalIndex
  if (dimension > 1)
    {
      // Second put the dependence parameters
      NumericalPointWithDescription point(dimension * (dimension - 1) / 2);
      Description dependenceDescription(point.getDimension());
      point.setName("dependence");
      UnsignedLong dependenceIndex(0);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          for (UnsignedLong j = 0; j < i; ++j)
            {
              point[dependenceIndex] = R_(i, j);
              dependenceDescription[dependenceIndex] = OSS() << "R_" << i << "_" << j;
              ++dependenceIndex;
            }
        }
      point.setDescription(dependenceDescription);
      parameters[dimension] = point;
    } // dimension > 1
  return parameters;
} // getParametersCollection

void EllipticalDistribution::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedLong size(parametersCollection.getSize());
  const UnsignedLong dimension(size > 1 ? size - 1 : size);
  setDimension(dimension);
  mean_ = NumericalPoint(dimension);
  sigma_ = NumericalPoint(dimension);
  R_ = CorrelationMatrix(dimension);
  if (dimension == 1)
    {
      mean_[0] = parametersCollection[0][0];
      sigma_[0] = parametersCollection[0][1];
      if (sigma_[0] <= 0.0) throw InvalidArgumentException(HERE) << "The marginal standard deviations must be > 0 sigma=" << sigma_;
    }
  else
    {
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          mean_[i] = parametersCollection[i][0];
          sigma_[i] = parametersCollection[i][1];
          if (sigma_[i] <= 0.0) throw InvalidArgumentException(HERE) << "The marginal standard deviations must be > 0 sigma=" << sigma_;
        }
      UnsignedLong parameterIndex(0);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          for (UnsignedLong j = 0; j < i; ++j)
            {
              R_(i, j) = parametersCollection[size - 1][parameterIndex];
              ++parameterIndex;
            }
        }
      if ( !R_.isPositiveDefinite()) throw InvalidArgumentException(HERE) << "The correlation matrix must be definite positive R=" << R_;
    }
  update();
  computeRange();
  isAlreadyComputedCovariance_ = false;
}

/* Method save() stores the object through the StorageManager */
void EllipticalDistribution::save(Advocate & adv) const
{
  ContinuousDistribution::save(adv);
  adv.saveAttribute( "R_", R_ );
  adv.saveAttribute( "sigma_", sigma_ );
  adv.saveAttribute( "mean_duplicate", mean_ );
  adv.saveAttribute( "shape_", shape_ );
  adv.saveAttribute( "inverseR_", inverseR_ );
  adv.saveAttribute( "cholesky_", cholesky_ );
  adv.saveAttribute( "inverseCholesky_", inverseCholesky_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void EllipticalDistribution::load(Advocate & adv)
{
  ContinuousDistribution::load(adv);
  adv.loadAttribute( "R_", R_ );
  adv.loadAttribute( "sigma_", sigma_ );
  adv.loadAttribute( "mean_duplicate", mean_ );
  adv.loadAttribute( "shape_", shape_ );
  adv.loadAttribute( "inverseR_", inverseR_ );
  adv.loadAttribute( "cholesky_", cholesky_ );
  adv.loadAttribute( "inverseCholesky_", inverseCholesky_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
}

END_NAMESPACE_OPENTURNS
