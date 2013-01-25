//                                               -*- C++ -*-
/**
 *  @file  InverseNormal.cxx
 *  @brief The InverseNormal distribution
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include "InverseNormal.hxx"
#include "DistFunc.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#include "AnalyticalNumericalMathGradientImplementation.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(InverseNormal);

static Factory<InverseNormal> RegisteredFactory("InverseNormal");

/* Default constructor */
InverseNormal::InverseNormal()
  : NonEllipticalDistribution("InverseNormal"),
    lambda_(1.0),
    mu_(1.0)
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
InverseNormal::InverseNormal(const NumericalScalar lambda,
                             const NumericalScalar mu)
  : NonEllipticalDistribution("InverseNormal"),
    lambda_(0.0),
    mu_(0.0)
{
  // This call set also the range
  setLambdaMu(lambda, mu);
  setDimension( 1 );
}

/* Comparison operator */
Bool InverseNormal::operator ==(const InverseNormal & other) const
{
  if (this == &other) return true;
  return (lambda_ == other.lambda_) && (mu_ == other.mu_);
}

/* String converter */
String InverseNormal::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseNormal::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " lambda=" << lambda_
      << " mu=" << mu_;
  return oss;
}

String InverseNormal::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(lambda = " << lambda_ << ", mu = " << mu_ << ")";
  return oss;
}

/* Virtual constructor */
InverseNormal * InverseNormal::clone() const
{
  return new InverseNormal(*this);
}

/* Get one realization of the distribution */
NumericalPoint InverseNormal::getRealization() const
{
  const NumericalScalar nu(DistFunc::rNormal());
  const NumericalScalar y(nu * nu);
  const NumericalScalar x(mu_ * (1.0 + (1.0 / (2.0 * lambda_)) * (mu_ * y - sqrt(4.0 * mu_ * lambda_ * y + mu_ * mu_ * y * y))));
  const NumericalScalar z(RandomGenerator::Generate());
  if (z * (mu_ + x) <= mu_) return NumericalPoint(1, x);
  return NumericalPoint(1, mu_ * mu_ / x);
}


/* Get the PDF of the distribution */
NumericalScalar InverseNormal::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  return sqrt(lambda_ / (2.0 * M_PI * x * x * x)) * exp(- lambda_ * (x - mu_) * (x - mu_) / (2.0 * x * mu_ * mu_));
}

NumericalScalar InverseNormal::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= 0.0) return -SpecFunc::MaxNumericalScalar;
  return 0.5 * ( log(lambda_) - log (2.0 * M_PI * x * x * x)) - lambda_ * (x - mu_) * (x - mu_) / (2.0 * x * mu_ * mu_);
}

/* Get the CDF of the distribution */
NumericalScalar InverseNormal::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if (x <= 0.0) return 0.0;
  const NumericalScalar lx(sqrt(lambda_ / x));
  const NumericalScalar phiArg1( lx * ( x / mu_ - 1.0));
  const NumericalScalar phiArg2(-lx * ( x / mu_ + 1.0));
  const NumericalScalar result(DistFunc::pNormal(phiArg1) + exp(2.0 * lambda_ / mu_) * DistFunc::pNormal(phiArg2));
  return result;
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex InverseNormal::computeCharacteristicFunction(const NumericalScalar x) const
{
  if (fabs(x) < pdfEpsilon_) return 1.0;
  return exp(computeLogCharacteristicFunction(x));
}

NumericalComplex InverseNormal::computeLogCharacteristicFunction(const NumericalScalar x) const
{
  if (fabs(x) < pdfEpsilon_) return 0.0;
  const NumericalComplex logCF(lambda_ * (1.0 - sqrt(NumericalComplex(1.0, - 2.0 * mu_ * mu_ * x / lambda_))) / mu_);
  return logCF;
}

/* Compute the numerical range of the distribution given the parameters values */
void InverseNormal::computeRange()
{
  const NumericalPoint lowerBound(1, 0.0);
  const NumericalScalar q(DistFunc::qNormal(cdfEpsilon_, true));
  const NumericalPoint upperBound(1, 2.0 * lambda_ * mu_ / (2.0 * lambda_ + mu_ * q * q - q * sqrt(mu_ * (q * q * mu_ + 4.0 * lambda_))));
  const Interval::BoolCollection finiteLowerBound(1, true);
  const Interval::BoolCollection finiteUpperBound(1, false);
  setRange(Interval(lowerBound, upperBound, finiteLowerBound, finiteUpperBound));
}

/* Compute the mean of the distribution */
void InverseNormal::computeMean() const
{
  mean_ = NumericalPoint(1, mu_);
  isAlreadyComputedMean_ = true;
}

/* Compute the mean of the distribution */
void InverseNormal::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = mu_ * mu_ * mu_ / lambda_;
  isAlreadyComputedCovariance_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint InverseNormal::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance()(0, 0)));
}

/* Get the skewness of the distribution */
NumericalPoint InverseNormal::getSkewness() const
{
  return NumericalPoint(1, 3.0 * sqrt(mu_ / lambda_));
}

/* Get the kurtosis of the distribution */
NumericalPoint InverseNormal::getKurtosis() const
{
  return NumericalPoint(1, 3.0 + 15.0 * mu_ / lambda_);
}


/* Get the moments of the standardized distribution */
NumericalPoint InverseNormal::getStandardMoment(const UnsignedLong n) const
{
  if (n == 0) return NumericalPoint(1, 1.0);
  String formula(OSS() << "exp((" << lambda_ / mu_ << ") * (1 - sqrt(1 - 2 * (" << mu_ * mu_ / lambda_ << ") * t)))");
  for (UnsignedLong i = 0; i < n; ++i)
    {
      const AnalyticalNumericalMathGradientImplementation gradient(AnalyticalNumericalMathEvaluationImplementation(Description(1, "t"), Description(1, "y"), Description(1, formula)));
      formula = gradient.getFormula(0, 0);
    }
  return AnalyticalNumericalMathEvaluationImplementation(Description(1, "t"), Description(1, "y"), Description(1, formula))(NumericalPoint(1, 0.0));
}

/* Interface specific to InverseNormal */

void InverseNormal::setLambdaMu(const NumericalScalar lambda,
                                const NumericalScalar mu)
{
  if ( (lambda <= 0.0) || (mu <= 0.0) ) throw InvalidArgumentException(HERE) << "lambda and mu MUST be positive";
  if ((lambda_ != lambda) || (mu_ != mu))
    {
      lambda_ = lambda;
      mu_ = mu;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

/* lambda accessor */
NumericalScalar InverseNormal::getLambda() const
{
  return lambda_;
}


/* mu accessor */
NumericalScalar InverseNormal::getMu() const
{
  return mu_;
}

/* Parameters value and description accessor */
InverseNormal::NumericalPointWithDescriptionCollection InverseNormal::getParametersCollection() const
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

void InverseNormal::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = InverseNormal(parametersCollection[0][0], parametersCollection[0][1]);
}

/* Method save() stores the object through the StorageManager */
void InverseNormal::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
  adv.saveAttribute( "mu_", mu_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseNormal::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
  adv.loadAttribute( "mu_", mu_ );
  computeRange();
}


END_NAMESPACE_OPENTURNS
