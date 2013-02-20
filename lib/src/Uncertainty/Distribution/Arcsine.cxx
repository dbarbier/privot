//                                               -*- C++ -*-
/**
 *  @file  Arcsine.cxx
 *  @brief The Arcsine distribution
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
 *  @date   2009-10-27 17:42:46 +0100 (mar. 27 oct. 2009)
 */
#include <cmath>
#include "Arcsine.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Arcsine);

static Factory<Arcsine> RegisteredFactory("Arcsine");


/* Default constructor */
Arcsine::Arcsine()
  : NonEllipticalDistribution("Arcsine"),
    a_(-1.0),
    b_(1.0)
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
Arcsine::Arcsine(const NumericalScalar arg1,
                 const NumericalScalar arg2,
                 const ParameterSet set)
  : NonEllipticalDistribution("Arcsine"),
    a_(-1.0),
    b_(1.0)
{
  switch (set)
    {
    case AB:
      setAB(arg1, arg2);
      break;

    case MUSIGMA:
      setMuSigma(arg1, arg2);
      break;

    default:
      throw InvalidArgumentException(HERE) << "Invalid parameter set argument";
    }
  setDimension(1);
}

/* Comparison operator */
Bool Arcsine::operator ==(const Arcsine & other) const
{
  if (this == &other) return true;
  return (a_ == other.a_) && (b_ == other.b_);
}

/* String converter */
String Arcsine::__repr__() const
{
  OSS oss;
  oss << "class=" << Arcsine::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " a=" << a_
      << " b=" << b_;
  return oss;
}

String Arcsine::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(a = " << a_ << ", b = " << b_ << ")";
  return oss;
}

/* Virtual constructor */
Arcsine * Arcsine::clone() const
{
  return new Arcsine(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Arcsine::computeRange()
{
  setRange(Interval(a_, b_));
}

/* Get one realization of the distribution */
NumericalPoint Arcsine::getRealization() const
{
  return NumericalPoint(1, computeScalarQuantile(RandomGenerator::Generate()));
}

/* Get the DDF of the distribution */
NumericalPoint Arcsine::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  NumericalScalar ddf = 0.0;
  if ((a_ < x) && (x <= b_))
    {
      ddf = (x - 0.5 * (a_ + b_)) / (M_PI * (x - a_) * (b_ - x) * sqrt((x - a_) * (b_ - x)));
    }
  return NumericalPoint(1, ddf);
}


/* Get the PDF of the distribution */
NumericalScalar Arcsine::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x >= b_)) return 0.0;
  return exp(computeLogPDF(point));
}

NumericalScalar Arcsine::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x >= b_)) return -SpecFunc::MaxNumericalScalar;
  return -log(M_PI) - 0.5 * (log(b_ - x) + log(x - a_));
}

/* Get the CDF of the distribution */
NumericalScalar Arcsine::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 0.0;
  if (x > b_) return 1.0;
  const NumericalScalar value(asin((x - 0.5 * (a_ + b_)) / (0.5 * (b_ - a_))) / M_PI);
  return 0.5 + value;
}

NumericalScalar Arcsine::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 1.0;
  if (x > b_) return 0.0;
  const NumericalScalar value(asin((x - 0.5 * (a_ + b_)) / (0.5 * (b_ - a_))) / M_PI);
  return 0.5 - value;
}

/* Get the PDFGradient of the distribution */
NumericalPoint Arcsine::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(2);
  NumericalPoint pdfGradient2(2);
  NumericalScalar x(point[0]);

  if ((a_ < x) && (x <= b_))
    {
      pdfGradient[0] = -0.5 * (b_ - a_) * sqrt((b_ - x) * (x - a_)) / (M_PI * (x - b_) * (x - b_) * (x - a_) * (x - a_));
      pdfGradient[1] = (0.5 * (a_ + b_) - x) * sqrt((b_ - x) * (x - a_)) / (M_PI * (x - a_) * (x - a_) * (b_ - x) * (b_ - x));
      pdfGradient2[0] = -0.5 * pdfGradient[0] + 0.5 * pdfGradient[1];
      pdfGradient2[1] =  0.5 * pdfGradient[0] + 0.5 * pdfGradient[1];
    }
  return pdfGradient2;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Arcsine::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(2);
  NumericalPoint cdfGradient2(2);
  NumericalScalar x(point[0]);
  if ((a_ < x) && (x <= b_))
    {
      cdfGradient[0] = (0.5 * (a_ + b_) - x) / (M_PI * 0.5 * (b_ - a_) * sqrt((b_ - x) * (x - a_)));
      cdfGradient[1]  = -1.0 / (M_PI * sqrt((b_ - x) * (x - a_)));
      cdfGradient2[0] = -0.5 * cdfGradient[0] + 0.5 * cdfGradient[1];
      cdfGradient2[1] =  0.5 * cdfGradient[0] + 0.5 * cdfGradient[1];
    }
  return cdfGradient2;
}

/* Get the quantile of the distribution */
NumericalScalar Arcsine::computeScalarQuantile(const NumericalScalar prob,
                                               const Bool tail,
                                               const NumericalScalar precision) const
{
  const NumericalScalar proba(tail ? 1.0 - prob : prob);
  const NumericalScalar quantile(0.5 * (b_ - a_) * sin(M_PI * (proba - 0.5)) + 0.5 * (a_ + b_));
  return quantile;
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar Arcsine::getRoughness() const
{
  throw NotDefinedException(HERE) << "roughness of Arcsine distribution is not L2-integrable";
}

/* Compute the mean of the distribution */
void Arcsine::computeMean() const
{
  mean_ = NumericalPoint(1, getMu());
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Arcsine::getStandardDeviation() const /*throw(NotDefinedException)*/
{
  return NumericalPoint(1, getSigma());
}

/* Get the skewness of the distribution */
NumericalPoint Arcsine::getSkewness() const /*throw(NotDefinedException)*/
{
  return NumericalPoint(1, 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Arcsine::getKurtosis() const /*throw(NotDefinedException)*/
{
  NumericalScalar standardDeviation4(getSigma());
  standardDeviation4 *= standardDeviation4;
  standardDeviation4 *= standardDeviation4;
  NumericalScalar a4(0.5 * (b_ - a_) * 0.5 * (b_ - a_));
  a4 *= a4;
  return NumericalPoint(1, (3.0 / 8.0) * a4 / standardDeviation4);
}

/* Get the moments of the standardized distribution */
NumericalPoint Arcsine::getStandardMoment(const UnsignedLong n) const
{
  if (n % 2 == 1) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, exp(SpecFunc::LogGamma(0.5 * n + 0.5) - SpecFunc::LogGamma(0.5 * n + 1)) / sqrt(M_PI));
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Arcsine::Implementation Arcsine::getStandardRepresentative() const
{
  return Arcsine(-1.0, 1.0).clone();
}

/* Compute the covariance of the distribution */
void Arcsine::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar eta(getSigma());
  covariance_(0, 0) = eta * eta;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Arcsine::NumericalPointWithDescriptionCollection Arcsine::getParametersCollection() const
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

void Arcsine::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Arcsine(parametersCollection[0][0], parametersCollection[0][1]);
}


/* A accessor */
void Arcsine::setA(const NumericalScalar a)
{
  if (a >= b_) throw InvalidArgumentException(HERE) << "in Arcsine : a must be smaller than b";
  if (a != a_)
    {
      a_ = a;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Arcsine::getA() const
{
  return a_;
}


/* B accessor */
void Arcsine::setB(const NumericalScalar b)
{
  if (a_ >= b) throw InvalidArgumentException(HERE) << "in Arcsine : a must be smaller than b";
  if (b != b_)
    {
      b_ = b;
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar Arcsine::getB() const
{
  return b_;
}

void Arcsine::setAB(const NumericalScalar a,
                    const NumericalScalar b)
{
  if (a >= b)
    throw InvalidArgumentException(HERE) << "in Arcsine : a must be smaller than b";
  a_ = a;
  b_ = b;
  computeRange();
}

/* Mu accessor */
void Arcsine::setMu(const NumericalScalar mu)
{
  const NumericalScalar sigma(getSigma());
  setAB(mu - sigma * M_SQRT1_2, mu + sigma * M_SQRT1_2);
}

NumericalScalar Arcsine::getMu() const
{
  return 0.5 * (a_ + b_);
}


/* Sigma accessor */
void Arcsine::setSigma(const NumericalScalar sigma)
{
  const NumericalScalar mu(getMu());
  setAB(mu - sigma * M_SQRT2, mu + sigma * M_SQRT2);
}

NumericalScalar Arcsine::getSigma() const
{
  return 0.5 * (b_ - a_) * M_SQRT1_2;
}

void Arcsine::setMuSigma(const NumericalScalar mu,
                         const NumericalScalar sigma)
{
  if (sigma <= 0.0) throw InvalidArgumentException(HERE) << "in Arcsine : sigma must be positive";
  setAB(mu - sigma * sqrt(2.0), mu + sigma * sqrt(2.0));
}

/* Method save() stores the object through the StorageManager */
void Arcsine::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "a_", a_ );
  adv.saveAttribute( "b_", b_ );
}

/* Method load() reloads the object from the StorageManager */
void Arcsine::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "a_", a_ );
  adv.loadAttribute( "b_", b_ );
  computeRange();
}


END_NAMESPACE_OPENTURNS
