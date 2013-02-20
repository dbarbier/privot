//                                               -*- C++ -*-
/**
 *  @file  Laplace.cxx
 *  @brief The Laplace distribution
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
 *  @author lebrun
 *  @date   2008-11-23 16:03:50 +0100 (dim, 23 nov 2008)
 */
#include <cmath>
#include "Laplace.hxx"
#include "SpecFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Laplace);

static Factory<Laplace> RegisteredFactory("Laplace");

/* Default constructor */
Laplace::Laplace()
  : NonEllipticalDistribution("Laplace"),
    lambda_(1.0),
    mu_(0.0)
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Laplace::Laplace(const NumericalScalar lambda,
                 const NumericalScalar mu)
  : NonEllipticalDistribution("Laplace"),
    lambda_(lambda),
    mu_(mu)
{
  // We set the dimension of the Laplace distribution
  setDimension( 1 );
  // This call set also the range.
  setLambda(lambda);
}

/* Comparison operator */
Bool Laplace::operator ==(const Laplace & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_) && (mu_ == other.mu_);
}

/* String converter */
String Laplace::__repr__() const
{
  OSS oss;
  oss << "class=" << Laplace::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " lambda=" << lambda_
      << " mu=" << mu_;
  return oss;
}

String Laplace::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(lambda = " << lambda_ << ", mu = " << mu_ << ")";
  return oss;
}

/* Virtual constructor */
Laplace * Laplace::clone() const
{
  return new Laplace(*this);
}

/* Get one realization of the distribution */
NumericalPoint Laplace::getRealization() const
{
  const NumericalScalar d(RandomGenerator::Generate() - 0.5);
  if (d < 0.0) return NumericalPoint(1, mu_ + log1p(2.0 * d) / lambda_);
  return NumericalPoint(1, mu_ - log1p(-2.0 * d) / lambda_);
}


/* Get the DDF of the distribution */
NumericalPoint Laplace::computeDDF(const NumericalPoint & point) const
{
  const NumericalScalar value(computePDF(point) * lambda_);
  return (point[0] < mu_ ? NumericalPoint(1, value) : NumericalPoint(1, -value));
}


/* Get the PDF of the distribution */
NumericalScalar Laplace::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  return 0.5 * lambda_ * exp(-lambda_ * fabs(point[0] - mu_));
}

NumericalScalar Laplace::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  return log(0.5 * lambda_) - lambda_ * fabs(point[0] - mu_);
}

/* Get the CDF of the distribution */
NumericalScalar Laplace::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar u(lambda_ * (point[0] - mu_));
  if (u < 0.0) return 0.5 * exp(u);
  return 1.0 - 0.5 * exp(-u);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Laplace::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalScalar t(x / lambda_);
  return exp(NumericalComplex(0.0, mu_ * x)) / (1.0 + t * t);
}

NumericalComplex Laplace::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalScalar t(x / lambda_);
  return NumericalComplex(0.0, mu_ * x) - log1p(t * t);
}

/* Get the PDFGradient of the distribution */
NumericalPoint Laplace::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(2, 0.0);
  const NumericalScalar factor(fabs(point[0] - mu_) * lambda_);
  const NumericalScalar expFactor(exp(-factor));
  pdfGradient[0] = 0.5 * expFactor * (1.0 - factor);
  pdfGradient[1] = (point[0] > mu_ ? 0.5 * lambda_ * lambda_ * expFactor : -0.5 * lambda_ * lambda_ * expFactor);
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Laplace::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(2, 0.0);
  const NumericalScalar factor(fabs(point[0] - mu_) * lambda_);
  const NumericalScalar expFactor(exp(-factor));
  cdfGradient[0] = 0.5 * factor / lambda_ * expFactor;
  cdfGradient[1] = -0.5 * lambda_ * expFactor;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Laplace::computeScalarQuantile(const NumericalScalar prob,
                                               const Bool tail,
                                               const NumericalScalar precision) const
{
  const NumericalScalar d(tail ? 0.5 - prob : prob - 0.5);
  if (d < 0.0) return mu_ + log1p(2.0 * d) / lambda_;
  return mu_ - log1p(-2.0 * d) / lambda_;
}

/* Compute the mean of the distribution */
void Laplace::computeMean() const
{
  mean_ = NumericalPoint(1, mu_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Laplace::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(2.0) / lambda_);
}

/* Get the skewness of the distribution */
NumericalPoint Laplace::getSkewness() const
{
  return NumericalPoint(1, 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Laplace::getKurtosis() const
{
  return NumericalPoint(1, 6.0);
}

/* Get the moments of the standardized distribution */
NumericalPoint Laplace::getStandardMoment(const UnsignedLong n) const
{
  if ((n % 2) == 1) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, SpecFunc::Gamma(n + 1));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Laplace::Implementation Laplace::getStandardRepresentative() const
{
  return Laplace(1.0, 0.0).clone();
}

/* Compute the covariance of the distribution */
void Laplace::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = 2.0 / (lambda_ * lambda_);
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Laplace::NumericalPointWithDescriptionCollection Laplace::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = lambda_;
  point[1] = mu_;
  description[0] = "lambda";
  description[1] = "mu";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Laplace::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Laplace(parametersCollection[0][0], parametersCollection[0][1]);
}

/* Mu accessor */
void Laplace::setMu(const NumericalScalar mu)
{
  if (mu != mu_)
    {
      mu_ = mu;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on mu
      computeRange();
    }
}

/* Mu accessor */
NumericalScalar Laplace::getMu() const
{
  return mu_;
}


/* Lambda accessor */
void Laplace::setLambda(const NumericalScalar lambda)
{
  if (lambda <= 0.0) throw InvalidArgumentException(HERE) << "Lambda MUST be positive";
  if (lambda != lambda_)
    {
      lambda_ = lambda;
      isAlreadyComputedCovariance_ = false;
      // The mean does not depend on lambda
      computeRange();
    }
}

/* Lambda accessor */
NumericalScalar Laplace::getLambda() const
{
  return lambda_;
}

/* Method save() stores the object through the StorageManager */
void Laplace::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
  adv.saveAttribute( "mu_", mu_ );
}

/* Method load() reloads the object from the StorageManager */
void Laplace::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  adv.loadAttribute( "mu_", mu_ );
  computeRange();
}




END_NAMESPACE_OPENTURNS
