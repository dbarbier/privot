//                                               -*- C++ -*-
/**
 *  @file  LogUniform.cxx
 *  @brief The LogUniform distribution
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
 *  @date   2011-04-11 12:32:27 +0200 (lun. 11 avril 2011)
 */
#include <cmath>
#include "LogUniform.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(LogUniform);

static Factory<LogUniform> RegisteredFactory("LogUniform");

/* Default constructor */
LogUniform::LogUniform()
  : NonEllipticalDistribution("LogUniform"),
    aLog_(-1.0),
    bLog_(1.0),
    a_(exp(-1.0)),
    b_(exp(1.0))
{
  setDimension( 1 );
  computeRange();
}

/* Parameters constructor */
LogUniform::LogUniform(const NumericalScalar aLog,
                       const NumericalScalar bLog)
  : NonEllipticalDistribution("LogUniform"),
    aLog_(aLog),
    bLog_(bLog),
    a_(exp(aLog_)),
    b_(exp(bLog_))
{
  if (bLog <= aLog) throw InvalidArgumentException(HERE) << "Error the lower bound aLog of a LogUniform distribution must be lesser than its upper bound bLog, here aLog=" << aLog << " bLog=" << bLog;
  setDimension( 1 );
  computeRange();
}

/* Comparison operator */
Bool LogUniform::operator ==(const LogUniform & other) const
{
  if (this == &other) return true;
  return (aLog_ == other.aLog_) && (bLog_ == other.bLog_);
}

/* String converter */
String LogUniform::__repr__() const
{
  OSS oss;
  oss << "class=" << LogUniform::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " aLog=" << aLog_
      << " bLog=" << bLog_
      << " a=" << a_
      << " b=" << b_;
  return oss;
}

String LogUniform::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(aLog = " << aLog_ << ", bLog = " << bLog_ << ")";
  return oss;
}

/* Virtual constructor */
LogUniform * LogUniform::clone() const
{
  return new LogUniform(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void LogUniform::computeRange()
{
  setRange(Interval(a_, b_));
}


/* Get one realization of the distribution */
NumericalPoint LogUniform::getRealization() const
{
  return NumericalPoint(1, exp(aLog_ + (bLog_ - aLog_) * RandomGenerator::Generate()));
}


/* Get the DDF of the distribution */
NumericalPoint LogUniform::computeDDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if ((x < a_) || (x > b_)) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, -1.0 / (x * x * (bLog_ - aLog_)));
}


/* Get the PDF of the distribution */
NumericalScalar LogUniform::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return 0.0;
  return 1.0 / (x * (bLog_ - aLog_));
}


/* Get the CDF of the distribution */
NumericalScalar LogUniform::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 0.0;
  if (x > b_)  return 1.0;
  return (log(x) - aLog_) / (bLog_ - aLog_);
}

NumericalScalar LogUniform::computeComplementaryCDF(const NumericalPoint & point) const
{
  const NumericalScalar x(point[0]);
  if (x <= a_) return 1.0;
  if (x > b_)  return 0.0;
  return (bLog_ - log(x)) / (bLog_ - aLog_);
}

/* Get the PDFGradient of the distribution */
NumericalPoint LogUniform::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradient(2, 0.0);
  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return pdfGradient;
  const NumericalScalar value(computePDF(point) / (bLog_ - aLog_));
  pdfGradient[0] = value;
  pdfGradient[1] = -value;
  return pdfGradient;
}

/* Get the CDFGradient of the distribution */
NumericalPoint LogUniform::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradient(2, 0.0);
  const NumericalScalar x(point[0]);
  if ((x <= a_) || (x > b_)) return cdfGradient;
  const NumericalScalar denominator(pow(bLog_ - aLog_, 2));
  const NumericalScalar logX(log(x));
  cdfGradient[0] = (logX - bLog_) / denominator;
  cdfGradient[1] = (aLog_ - logX) / denominator;
  return cdfGradient;
}

/* Get the quantile of the distribution */
NumericalScalar LogUniform::computeScalarQuantile(const NumericalScalar prob,
                                                  const Bool tail,
                                                  const NumericalScalar precision) const
{
  if (tail) return exp(bLog_ - prob * (bLog_ - aLog_));
  return exp(aLog_ + prob * (bLog_ - aLog_));
}

/* Compute the mean of the distribution */
void LogUniform::computeMean() const
{
  mean_ = NumericalPoint(1, (b_ - a_) / (bLog_ - aLog_));
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint LogUniform::getStandardDeviation() const
{
  return NumericalPoint(1, sqrt(getCovariance()(0, 0)));
}

/* Get the skewness of the distribution */
NumericalPoint LogUniform::getSkewness() const
{
  NumericalScalar t1 = sqrt(2.0);
  NumericalScalar t2 = a_ * a_;
  NumericalScalar t4 = b_ * b_;
  NumericalScalar t6 = bLog_ * aLog_;
  NumericalScalar t9 = bLog_ * bLog_;
  NumericalScalar t14 = aLog_ * aLog_;
  NumericalScalar t30 = a_ * b_;
  NumericalScalar t39 = 12.0 * t2 + 12.0 * t4 - 4.0 * t4 * t6 + 2.0 * t4 * t9 - 9.0 * t4 * bLog_ + 2.0 * t4 * t14 + 9.0 * t4 *
    aLog_ - 9.0 * t2 * aLog_ + 2.0 * t2 * t9 + 2.0 * b_ * a_ * t9 + 9.0 * t2 * bLog_ - 4.0 * t2 * t6 - 4.0 * t30 * t6 + 2.0 * b_ * a_ *
    t14 - 24.0 * t30 + 2.0 * t2 * t14;
  NumericalScalar t42 = sqrt(b_ - a_);
  NumericalScalar t45 = -bLog_ + aLog_;
  NumericalScalar t46 = t45 * t45;
  NumericalScalar t54 = -2.0 * a_ + aLog_ * b_ + 2.0 * b_ + aLog_ * a_ - bLog_ * b_ - bLog_ * a_;
  NumericalScalar t56 = sqrt(-t54 / t46);
  NumericalScalar t63 = 1 / t45 / t54 / t56 / t42 * t39 * t1 / 3.0;
  return NumericalPoint(1, t63);
}

/* Get the kurtosis of the distribution */
NumericalPoint LogUniform::getKurtosis() const
{
  NumericalScalar t5 = a_ * a_;
  NumericalScalar t7 = b_ * b_;
  NumericalScalar t9 = bLog_ * aLog_;
  NumericalScalar t12 = bLog_ * bLog_;
  NumericalScalar t17 = aLog_ * aLog_;
  NumericalScalar t33 = a_ * b_;
  NumericalScalar t42 = 12.0 * t5 + 12.0 * t7 - 4.0 * t7 * t9 + 2.0 * t7 * t12 - 9.0 * t7 * bLog_ + 2.0 * t7 * t17 + 9.0 * t7 *
    aLog_ - 9.0 * t5 * aLog_ + 2.0 * t5 * t12 + 2.0 * b_ * a_ * t12 + 9.0 * t5 * bLog_ - 4.0 * t5 * t9 - 4.0 * t33 * t9 + 2.0 * b_ *
    a_ * t17 - 24.0 * t33 + 2.0 * t5 * t17;
  NumericalScalar t50 = pow(-2.0 * a_ + aLog_ * b_ + 2.0 * b_ + aLog_ * a_ - bLog_ * b_ - bLog_ * a_, 2.0);
  NumericalScalar t54 = 2.0 / 3.0 / t50 * t42 * (-bLog_ + aLog_) / (-b_ + a_);
  return NumericalPoint(1, t54);
}

/* Compute the covariance of the distribution */
void LogUniform::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar aLogbLog(bLog_ - aLog_);
  covariance_(0, 0) = 0.5 * (b_ - a_) * (b_ * (aLogbLog - 2.0) + a_ * (aLogbLog + 2.0)) / pow(aLogbLog, 2);
  isAlreadyComputedCovariance_ = true;
}

/* Get the moments of the standardized distribution */
NumericalPoint LogUniform::getStandardMoment(const UnsignedLong n) const
{
  if (n == 0) return NumericalPoint(1, 1.0);
  return NumericalPoint(1, (pow(b_, n) - pow(a_, n)) / (n * (bLog_ - aLog_)));
}

/* Parameters value and description accessor */
LogUniform::NumericalPointWithDescriptionCollection LogUniform::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(2);
  Description description(point.getDimension());
  point[0] = aLog_;
  point[1] = bLog_;
  description[0] = "aLog";
  description[1] = "bLog";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void LogUniform::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = LogUniform(parametersCollection[0][0], parametersCollection[0][1]);
}



/* A accessor */
void LogUniform::setALog(const NumericalScalar aLog)
{
  if (aLog != aLog_)
    {
      aLog_ = aLog;
      a_ = exp(aLog_);
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar LogUniform::getALog() const
{
  return aLog_;
}


/* B accessor */
void LogUniform::setBLog(const NumericalScalar bLog)
{
  if (bLog != bLog_)
    {
      bLog_ = bLog;
      b_ = exp(bLog);
      isAlreadyComputedMean_ = false;
      isAlreadyComputedCovariance_ = false;
      computeRange();
    }
}

NumericalScalar LogUniform::getBLog() const
{
  return bLog_;
}

/* Method save() stores the object through the StorageManager */
void LogUniform::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "aLog_", aLog_ );
  adv.saveAttribute( "bLog_", bLog_ );
  adv.saveAttribute( "a_", a_ );
  adv.saveAttribute( "b_", b_ );
}

/* Method load() reloads the object from the StorageManager */
void LogUniform::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "aLog_", aLog_ );
  adv.loadAttribute( "bLog_", bLog_ );
  adv.loadAttribute( "a_", a_ );
  adv.loadAttribute( "b_", b_ );
  computeRange();
}

END_NAMESPACE_OPENTURNS
