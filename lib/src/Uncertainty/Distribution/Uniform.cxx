//                                               -*- C++ -*-
/**
 *  @file  Uniform.cxx
 *  @brief The Uniform distribution
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
#include "Uniform.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(Uniform);

static Factory<Uniform> RegisteredFactory("Uniform");

/* Default constructor */
Uniform::Uniform()
  : NonEllipticalDistribution("Uniform"),
    a_(-1.0), b_(1.0)
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
Uniform::Uniform(const NumericalScalar a,
                 const NumericalScalar b)
  : NonEllipticalDistribution("Uniform"),
    a_(a), b_(b)
{
  if (b_ <= a_) throw InvalidArgumentException(HERE) << "Error the lower bound a of a Uniform distribution must be lesser than its upper bound b, here a=" << a << " b=" << b;
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool Uniform::operator ==(const Uniform & other) const
{
  if (this == &other) return true;
  return (a_ == other.a_) && (b_ == other.b_);
}

/* String converter */
String Uniform::__repr__() const
{
  OSS oss;
  oss << "class=" << Uniform::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " a=" << a_
      << " b=" << b_;
  return oss;
}

String Uniform::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(a = " << a_ << ", b = " << b_ << ")";
  return oss;
}

/* Virtual constructor */
Uniform * Uniform::clone() const
{
  return new Uniform(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Uniform::computeRange()
{
  setRange(Interval(a_, b_));
}


/* Get one realization of the distribution */
NumericalPoint Uniform::getRealization() const
{
  return NumericalPoint(1, a_ + (b_ - a_) * RandomGenerator::Generate());
}


/* Get the DDF of the distribution */
NumericalPoint Uniform::computeDDF(const NumericalPoint & point) const
{
  return NumericalPoint(1, 0.0);
}


/* Get the PDF of the distribution */
NumericalScalar Uniform::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return 0.0;
  return 1.0 / (b_ - a_);
}


/* Get the CDF of the distribution */
NumericalScalar Uniform::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 0.0;
  if (x > b_)  return 1.0;
  return (x - a_) / (b_ - a_);
}

NumericalScalar Uniform::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 1.0;
  if (x > b_)  return 0.0;
  return (b_ - x) / (b_ - a_);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Uniform::computeCharacteristicFunction(const NumericalScalar x) const
{
  NumericalComplex result;
  const NumericalScalar ax(a_ * x);
  const NumericalScalar bx(b_ * x);
  if (fabs(x) <= 1.0e-10) result = NumericalComplex(1.0, 0.5 * (ax + bx));
  else
    {
      const NumericalScalar idenom(1.0 / (bx - ax));
      result = NumericalComplex(idenom * (sin(bx) - sin(ax)), idenom * (cos(ax) - cos(bx)));
    }
  return result;
}

/* Get the PDFGradient of the distribution */
NumericalPoint Uniform::computePDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  NumericalPoint pdfGradient(2, 0.0);
  if ((x <= a_) || (x > b_)) return pdfGradient;
  const NumericalScalar iAB(1.0 / (b_ - a_));
  const NumericalScalar iAB2(iAB * iAB);
  pdfGradient[0] = iAB2;
  pdfGradient[1] = -iAB2;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Uniform::computeCDFGradient(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  NumericalPoint cdfGradient(2, 0.0);
  if ((x <= a_) || (x > b_)) return cdfGradient;
  const NumericalScalar iAB(1.0 / (b_ - a_));
  const NumericalScalar iAB2(iAB * iAB);
  cdfGradient[0] = (x - b_) * iAB2;
  cdfGradient[1] = (a_ - x) * iAB2;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Uniform::computeScalarQuantile(const NumericalScalar prob,
                                               const Bool tail,
                                               const NumericalScalar precision) const
{
  if (tail) return b_ - prob * (b_ - a_);
  return a_ + prob * (b_ - a_);
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar Uniform::getRoughness() const
{
  return 1.0 / (b_ - a_);
}

/* Compute the mean of the distribution */
void Uniform::computeMean() const
{
  mean_ = NumericalPoint(1, 0.5 * (a_ + b_));
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Uniform::getStandardDeviation() const
{
  return NumericalPoint(1, (b_ - a_) / sqrt(12.0));
}

/* Get the skewness of the distribution */
NumericalPoint Uniform::getSkewness() const
{
  return NumericalPoint(1, 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Uniform::getKurtosis() const
{
  // 1.8 = 9/5
  return NumericalPoint(1, 1.8);
}

/* Compute the covariance of the distribution */
void Uniform::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar eta(b_ - a_);
  covariance_(0, 0) = eta * eta / 12.0;
  isAlreadyComputedCovariance_ = true;
}

/* Get the moments of the standardized distribution */
NumericalPoint Uniform::getStandardMoment(const UnsignedLong n) const
{
  if (n % 2 == 1) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, 1.0 / (n + 1));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Uniform::Implementation Uniform::getStandardRepresentative() const
{
  return Uniform(-1.0, 1.0).clone();
}

/* Parameters value and description accessor */
Uniform::NumericalPointWithDescriptionCollection Uniform::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = a_;
  point[1] = b_;
  description[0] = "a";
  description[1] = "b";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Uniform::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Uniform(parametersCollection[0][0], parametersCollection[0][1]);
}

/* Check if the distribution is elliptical */
Bool Uniform::isElliptical() const
{
  return getDimension() == 1;
}



/* A accessor */
void Uniform::setA(const NumericalScalar a)
{
  if (a != a_)
    {
      a_ = a;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Uniform::getA() const
{
  return a_;
}


/* B accessor */
void Uniform::setB(const NumericalScalar b)
{
  if (b != b_)
    {
      b_ = b;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Uniform::getB() const
{
  return b_;
}

/* Method save() stores the object through the StorageManager */
void Uniform::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "a_", a_ );
  adv.saveAttribute( "b_", b_ );
}

/* Method load() reloads the object from the StorageManager */
void Uniform::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "a_", a_ );
  adv.loadAttribute( "b_", b_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
