//                                               -*- C++ -*-
/**
 *  @file  Beta.cxx
 *  @brief The Beta distribution
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
#include "Beta.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Beta);

static Factory<Beta> RegisteredFactory("Beta");


/* Default constructor */
Beta::Beta()
  : NonEllipticalDistribution("Beta"),
    r_(2.0),
    t_(4.0),
    a_(-1.0),
    b_(1.0),
    normalizationFactor_(log(0.75))
{
  setDimension(1);
  computeRange();
}

/* Parameters constructor */
Beta::Beta(const NumericalScalar arg1,
           const NumericalScalar arg2,
           const NumericalScalar a,
           const NumericalScalar b,
           const ParameterSet set)
  : NonEllipticalDistribution("Beta"),
    r_(0.0),
    t_(0.0),
    a_(a),
    b_(b),
    normalizationFactor_(0.0)
{
  if (b <= a) throw InvalidArgumentException(HERE) << "Error: the upper bound must be greater than the lower bound, here a=" << a << " and b=" << b;
  switch (set)
    {
    case RT:
      setRT(arg1, arg2);
      break;

    case MUSIGMA:
      setMuSigma(arg1, arg2);
      break;

    default:
      throw InvalidArgumentException(HERE) << "Invalid parameter set argument";

    } /* end switch */
  setDimension(1);
  computeRange();
}

/* Comparison operator */
Bool Beta::operator ==(const Beta & other) const
{
  if (this == &other) return true;
  return (r_ == other.r_) && (t_ == other.t_) &&
    (a_ == other.a_) && (b_ == other.b_);
}

/* String converter */
String Beta::__repr__() const
{
  OSS oss;
  oss << "class=" << Beta::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " r=" << r_
      << " t=" << t_
      << " a=" << a_
      << " b=" << b_;
  return oss;
}

String Beta::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(r = " << r_ << ", t = " << t_ << ", a = " << a_ << ", b = " << b_ << ")";
  return oss;
}

/* Virtual constructor */
Beta * Beta::clone() const
{
  return new Beta(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Beta::computeRange()
{
  setRange(Interval(a_, b_));
}

/* Update the derivative attributes */
void Beta::update()
{
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  normalizationFactor_ = (1.0 - t_) * log(b_ - a_) - SpecFunc::LnBeta(r_, t_ - r_);
}


/* Get one realization of the distribution */
NumericalPoint Beta::getRealization() const
{
  return NumericalPoint(1, a_ + (b_ - a_) * DistFunc::rBeta(r_, t_ - r_));
}

/* Get the DDF of the distribution */
NumericalPoint Beta::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, ((r_ - 1.0) / (x - a_) - (t_ - r_ - 1.0) / (b_ - x)) * computePDF(point));
}


/* Get the PDF of the distribution */
NumericalScalar Beta::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0]);
  if ((x == b_) && (t_ - r_ == 1.0)) return 1.0;
  if ((x <= a_) || (x >= b_)) return 0.0;
  return exp(computeLogPDF(point));
}

NumericalScalar Beta::computeLogPDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0]);
  if ((x == b_) && (t_ - r_ == 1.0)) return 0.0;
  if ((x <= a_) || (x >= b_)) return -SpecFunc::MaxNumericalScalar;
  return normalizationFactor_ + (r_ - 1.0) * log(x - a_) + (t_ - r_ - 1.0) * log(b_ - x);
}


/* Get the CDF of the distribution */
NumericalScalar Beta::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if (x <= a_) return 0.0;
  if (x > b_) return 1.0;
  return DistFunc::pBeta(r_, t_ - r_, (x - a_) / (b_ - a_));
}

/* Get the PDFGradient of the distribution */
NumericalPoint Beta::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(4, 0.0);
  NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return pdfGradient;
  NumericalScalar pdf(computePDF(point));
  NumericalScalar psiTR(SpecFunc::Psi(t_ - r_));
  NumericalScalar iBA(1.0 / (b_ - a_));
  NumericalScalar BX(b_ - x);
  NumericalScalar iBX(1.0 / BX);
  NumericalScalar XA(x - a_);
  NumericalScalar iXA(1.0 / XA);
  pdfGradient[0] = pdf * (log(XA * iBX) + psiTR - SpecFunc::Psi(r_));
  pdfGradient[1] = pdf * (log(BX * iBA) - psiTR + SpecFunc::Psi(t_));
  pdfGradient[2] = pdf * ((t_ - 1.0) * iBA - (r_ - 1.0) * iXA);
  pdfGradient[3] = pdf * ((t_ - 1.0) * XA * iBA * iBX - r_ * iBX);
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint Beta::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(4, 0.0);
  NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return cdfGradient;
  NumericalScalar cdf(computeCDF(point));
  NumericalScalar iBA(1.0 / (b_ - a_));
  NumericalScalar cdfShift(DistFunc::pBeta(r_ + 1.0, t_ - r_ - 1.0, (x - a_) * iBA));
  NumericalScalar cdfDiff(cdfShift - cdf);
  NumericalScalar factor(r_ * iBA);
  static const NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
  static const NumericalScalar i2Eps(0.5 / eps);
  cdfGradient[0] = i2Eps * (DistFunc::pBeta(r_ + eps, t_ - r_ - eps, (x - a_) / (b_ - a_)) - DistFunc::pBeta(r_ - eps, t_ - r_ + eps, (x - a_) / (b_ - a_)));
  cdfGradient[1] = i2Eps * (DistFunc::pBeta(r_, t_ - r_ + eps, (x - a_) / (b_ - a_)) - DistFunc::pBeta(r_, t_ - r_ - eps, (x - a_) / (b_ - a_)));
  cdfGradient[3] = factor * cdfDiff;
  cdfGradient[2] = cdfGradient[3] * (b_ - x) / (x - a_);
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar Beta::computeScalarQuantile(const NumericalScalar prob,
                                            const Bool tail,
                                            const NumericalScalar precision) const
{
  return a_ + (b_ - a_) * DistFunc::qBeta(r_, t_ - r_, prob, tail);
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Beta::computeCharacteristicFunction(const NumericalScalar x) const
{
  const NumericalComplex result(SpecFunc::HyperGeom_1_1(r_, t_, NumericalComplex(0.0, (b_ - a_) * x)) * exp(NumericalComplex(0.0, a_)));
  return result;
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar Beta::getRoughness() const
{
  NumericalScalar den(SpecFunc::Beta(r_, t_ - r_));
  return SpecFunc::Beta(2.0 * r_ - 1.0, 2.0 * (t_ - r_) - 1.0) / (den * den * (b_ - a_));
}

/* Compute the mean of the distribution */
void Beta::computeMean() const
{
  mean_ = NumericalPoint(1, a_ + (b_ - a_) * r_ / t_);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Beta::getStandardDeviation() const
{
  return NumericalPoint(1, getSigma());
}

/* Get the skewness of the distribution */
NumericalPoint Beta::getSkewness() const
{
  return NumericalPoint(1, 2.0 * (t_ - 2.0 * r_) / (t_ + 2.0) * sqrt((t_ + 1.0) / (r_ * (t_ - r_))));
}

/* Get the kurtosis of the distribution */
NumericalPoint Beta::getKurtosis() const
{
  return NumericalPoint(1, 3.0 * (1.0 + t_) * (2.0 * t_ * t_ + r_ * (t_ - 6.0) * (t_ - r_)) / (r_ * (t_ - r_) * (3.0 + t_) * (2.0 + t_)));
}

/* Compute the covariance of the distribution */
void Beta::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar eta((b_ - a_) / t_);
  covariance_(0, 0) = eta * eta * r_ * (t_ - r_) / (t_ + 1.0);
  isAlreadyComputedCovariance_ = true;
}

/* Get the moments of the standardized distribution */
NumericalPoint Beta::getStandardMoment(const UnsignedLong n) const
{
  if (n == 0) return NumericalPoint(1, 1.0);
  // Here we have to convert n to a signed type else -n will produce an overflow
  const NumericalScalar value((n % 2 == 0 ? 1.0 : -1.0) * SpecFunc::HyperGeom_2_1(r_, -static_cast<NumericalScalar>(n), t_, 2.0));
  return NumericalPoint(1, value);
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Beta::Implementation Beta::getStandardRepresentative() const
{
  return Beta(r_, t_, -1.0, 1.0).clone();
}

/* Parameters value and description accessor */
Beta::NumericalPointWithDescriptionCollection Beta::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(4);
  Description description(point.getDimension());
  point[0] = r_;
  point[1] = t_;
  point[2] = a_;
  point[3] = b_;
  description[0] = "r";
  description[1] = "t";
  description[2] = "a";
  description[3] = "b";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void Beta::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = Beta(parametersCollection[0][0], parametersCollection[0][1], parametersCollection[0][2], parametersCollection[0][3]);
}

/* Check if the distribution is elliptical */
Bool Beta::isElliptical() const
{
  return t_ == 2.0 * r_;
}



/* R accessor */
void Beta::setR(const NumericalScalar r)
{
  if (r <= 0.) throw InvalidArgumentException(HERE) << "R MUST be positive";
  if (r != r_)
    {
      r_ = r;
      update();
    }
}

NumericalScalar Beta::getR() const
{
  return r_;
}


/* T accessor */
void Beta::setT(const NumericalScalar t)
{
  if (t <= r_) throw InvalidArgumentException(HERE) << "T MUST be greater than r, here t=" << t << " and r=" << r_;
  if (t != t_)
    {
      t_ = t;
      update();
    }
}

NumericalScalar Beta::getT() const
{
  return t_;
}


/* RT accessor */
void Beta::setRT(const NumericalScalar r,
                 const NumericalScalar t)
{
  if (r <= 0.) throw InvalidArgumentException(HERE) << "R MUST be positive";
  if (t <= 0.) throw InvalidArgumentException(HERE) << "T MUST be positive";
  if ((r != r_) || (t != t_))
    {
      r_ = r;
      t_ = t;
      update();
    }
}

/* Mu accessor */
void Beta::setMuSigma(const NumericalScalar mu,
                      const NumericalScalar sigma)
{
  const NumericalScalar t((b_ - mu) * (mu - a_) / (sigma * sigma) - 1);
  const NumericalScalar r(t * (mu - a_) / (b_ - a_));
  setRT(r, t);
}

NumericalScalar Beta::getMu() const
{
  return a_ + (b_ - a_) * r_ / t_;
}

NumericalScalar Beta::getSigma() const
{
  return (b_ - a_) / t_ * sqrt(r_ * (t_ - r_) / (t_ + 1.0));
}


/* A accessor */
void Beta::setA(const NumericalScalar a)
{
  if (a != a_)
    {
      a_ = a;
      computeRange();
      update();
    }
}

NumericalScalar Beta::getA() const
{
  return a_;
}


/* B accessor */
void Beta::setB(const NumericalScalar b)
{
  if (b != b_)
    {
      b_ = b;
      computeRange();
      update();
    }
}

NumericalScalar Beta::getB() const
{
  return b_;
}

/* Method save() stores the object through the StorageManager */
void Beta::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "r_", r_ );
  adv.saveAttribute( "t_", t_ );
  adv.saveAttribute( "a_", a_ );
  adv.saveAttribute( "b_", b_ );
  adv.saveAttribute( "normalizationFactor_", normalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void Beta::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "r_", r_ );
  adv.loadAttribute( "t_", t_ );
  adv.loadAttribute( "a_", a_ );
  adv.loadAttribute( "b_", b_ );
  adv.loadAttribute( "normalizationFactor_", normalizationFactor_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
