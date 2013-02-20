//                                               -*- C++ -*-
/**
 *  @file  AliMikhailHaqCopula.cxx
 *  @brief The AliMikhailHaqCopula distribution
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
 *  @date   2012-05-16 15:02:11 +0200 (mer. 16 mai 2012)
 */
#include <cmath>
#include "AliMikhailHaqCopula.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(AliMikhailHaqCopula);

static Factory<AliMikhailHaqCopula> RegisteredFactory("AliMikhailHaqCopula");

/* Default constructor */
AliMikhailHaqCopula::AliMikhailHaqCopula()
  : ArchimedeanCopula("AliMikhailHaqCopula")
  , theta_(0.0)
{
  setDimension( 2 );
}

/* Parameters constructor */
AliMikhailHaqCopula::AliMikhailHaqCopula(const NumericalScalar theta)
  : ArchimedeanCopula("AliMikhailHaqCopula")
  , theta_(0.0)
{
  // We set the dimension of the AliMikhailHaqCopula distribution
  setDimension( 2 );
  // Check the given theta
  setTheta(theta);
}

/* Comparison operator */
Bool AliMikhailHaqCopula::operator ==(const AliMikhailHaqCopula & other) const
{
  if (this == &other) return true;
  return theta_ == other.theta_;
}

/* String converter */
String AliMikhailHaqCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << AliMikhailHaqCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " theta=" << theta_;
  return oss;
}

String AliMikhailHaqCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(theta = " << theta_ << ")";
  return oss;
}

/* Virtual constructor */
AliMikhailHaqCopula * AliMikhailHaqCopula::clone() const
{
  return new AliMikhailHaqCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint AliMikhailHaqCopula::getRealization() const
{
  NumericalPoint realization(2);
  // We use the general algorithm based on conditional CDF inversion
  const NumericalScalar u(RandomGenerator::Generate());
  realization[0] = u;
  const NumericalScalar v(RandomGenerator::Generate());
  const NumericalScalar theta2(theta_ * theta_);
  const NumericalScalar a(1.0 - u);
  const NumericalScalar b(-theta_ * (2.0 * a * v + 1.0) + 2.0 * theta2 * a * a * v + 1.0);
  const NumericalScalar delta(4.0 * v * (a - 1.0));
  const NumericalScalar c(theta2 * (a * delta + 1.0) - theta_ * (delta + 2.0) + 1.0);
  realization[1] = 2.0 * v * pow(a * theta_ - 1.0, 2) / (b + sqrt(c));
  return realization;
}

/* Get the DDF of the distribution */
NumericalPoint AliMikhailHaqCopula::computeDDF(const NumericalPoint & point) const
{
  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // A copula has a null DDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0))
    {
      return NumericalPoint(2, 0.0);
    }
  NumericalPoint result(2);
  const NumericalScalar t1(theta_ * theta_);
  const NumericalScalar t2(t1 * v);
  const NumericalScalar t3(theta_ * v);
  const NumericalScalar t5(theta_ * u);
  const NumericalScalar t6(t5 * v);
  const NumericalScalar t7(-1 + theta_ - t3 - t5 + t6);
  const NumericalScalar t8(t7 * t7);
  const NumericalScalar t12(t1 * u);
  const NumericalScalar t16(t8 * t8);
  result[0] = -(theta_ - t1 + t2 + t3) / t8 / t7 + 3 * (1 + t3 + t5 - t2 - t12 + t12 * v + t6 + t1 - 2 * theta_) / t16 * (-theta_ + t3);
  result[1] = -(theta_ - t1 + t12 + t5) / t8 / t7 + 3 * (1 + t3 + t5 - t12 - t1 * v + t12 * v + t6 + t1 - 2 * theta_) / t16 * (-theta_ + t5);
  return result;
}

/* Get the PDF of the distribution */
NumericalScalar AliMikhailHaqCopula::computePDF(const NumericalPoint & point) const
{
  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // A copula has a null PDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0))
    {
      return 0.0;
    }
  const NumericalScalar prod(theta_ * (1.0 - u) * (1.0 - v));
  return (1.0 + theta_ * ((1.0 + u) * (1.0 + v) - 3.0 + prod)) / pow(1.0 - prod, 3);
}

/* Get the CDF of the distribution */
NumericalScalar AliMikhailHaqCopula::computeCDF(const NumericalPoint & point) const
{
  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // If we are outside of the support, in the lower parts
  if ((u <= 0.0) || (v <= 0.0))
    {
      return 0.0;
    }
  // If we are outside of the support, in the upper part
  if ((u >= 1.0) && (v >= 1.0))
    {
      return 1.0;
    }
  // If we are outside of the support for u, in the upper part
  if (u >= 1.0)
    {
      return v;
    }
  // If we are outside of the support for v, in the upper part
  if (v >= 1.0)
    {
      return u;
    }
  // If we are in the support
  return u * v / (1.0 - theta_ * (1.0 - u) * (1.0 - v));
}

/** Get the PDFGradient of the distribution */
NumericalPoint AliMikhailHaqCopula::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/** Get the CDFGradient of the distribution */
NumericalPoint AliMikhailHaqCopula::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the quantile of the distribution */
NumericalPoint AliMikhailHaqCopula::computeQuantile(const NumericalScalar prob,
                                                    const Bool tail) const
{
  return NumericalPoint(2, (prob * theta_ + sqrt(prob * (1.0 - theta_ + prob * theta_))) / (1.0 + prob * theta_));
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar AliMikhailHaqCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return x;
  const NumericalScalar u(y[0]);
  const NumericalScalar v(x);
  // If we are in the support
  return v * (1.0 - theta_ * (1.0 - v)) / pow(1.0 - theta_ * (1.0 - u) * (1.0 - v), 2);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar AliMikhailHaqCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  if (q == 0.0) return 0.0;
  if (q == 1.0) return 1.0;
  // Initialize the conditional quantile with the quantile of the i-th marginal distribution
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return q;
  // Optimized code given by Maple 13
  const NumericalScalar u(y[0]);
  const NumericalScalar qTheta(q * theta_);
  const NumericalScalar theta2(theta_ * theta_);
  const NumericalScalar qTheta2(q * theta2);
  const NumericalScalar u2(u * u);
  const NumericalScalar qTheta2U2(qTheta2 * u2);
  const NumericalScalar qThetaU(qTheta * u);
  const NumericalScalar tmp1(2.0 * qThetaU);
  const NumericalScalar tmp2(4.0 * qTheta2 * u);
  const NumericalScalar tmp3(sqrt(1.0 + theta2 + 4.0 * qThetaU - tmp2 + 4.0 * qTheta2U2 - 2.0 * theta_));
  return -0.5 * (theta_ + 2.0 * qTheta - 2.0 * qTheta2 - 2.0 * qTheta2U2 - tmp1 - 1.0 + tmp2 + tmp3) / (theta_ * (-1.0 + qTheta - tmp1 + qTheta * u2));
}

/* Compute the archimedean generator of the archimedean copula, i.e.
 * the function phi such that the CDF of the copula can
 * be written as CDF(t) = phi^{-1}(phi(u)+phi(v))
 */
NumericalScalar AliMikhailHaqCopula::computeArchimedeanGenerator(const NumericalScalar t) const
{
  return log1p(-theta_ * (1.0 - t)) / t;
}

/* Compute the inverse of the archimedean generator */
NumericalScalar AliMikhailHaqCopula::computeInverseArchimedeanGenerator(const NumericalScalar t) const
{
  const NumericalScalar y(t * (theta_ - 1.0) / theta_);
  return (y - SpecFunc::LambertW(-t * exp(y) / theta_)) / t;
}

/* Compute the derivative of the density generator */
NumericalScalar AliMikhailHaqCopula::computeArchimedeanGeneratorDerivative(const NumericalScalar t) const
{
  const NumericalScalar u(1.0 - theta_ * (1.0 - t));
  return (theta_ / u - log(u) / t) / t;
}

/* Compute the seconde derivative of the density generator */
NumericalScalar AliMikhailHaqCopula::computeArchimedeanGeneratorSecondDerivative(const NumericalScalar t) const
{
  const NumericalScalar u(1.0 - theta_ * (1.0 - t));
  return (-theta_ * theta_ / (u * u) - 2.0 * theta_ / (t * u) + 2.0 * log(u) / (t * t)) / t;
}

/** Parameters value and description accessor */
AliMikhailHaqCopula::NumericalPointWithDescriptionCollection AliMikhailHaqCopula::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  NumericalPointWithDescription point(1);
  Description description(point.getDimension());
  point[0] = theta_;
  description[0] = "theta";
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}

void AliMikhailHaqCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = AliMikhailHaqCopula(parametersCollection[0][0]);
}

/* Tell if the distribution has independent copula */
Bool AliMikhailHaqCopula::hasIndependentCopula() const
{
  return (theta_ == 0.0);
}

/* Theta accessor */
void AliMikhailHaqCopula::setTheta(const NumericalScalar theta)
{
  if ((theta <= -1.0) || (theta >= 1.0)) throw InvalidArgumentException(HERE) << "Theta MUST be in (-1, 1), here theta=" << theta;
  theta_ = theta;
}

/* Theta accessor */
NumericalScalar AliMikhailHaqCopula::getTheta() const
{
  return theta_;
}

/* Method save() stores the object through the StorageManager */
void AliMikhailHaqCopula::save(Advocate & adv) const
{
  ArchimedeanCopula::save(adv);
  adv.saveAttribute( "theta_", theta_ );
}

/* Method load() reloads the object from the StorageManager */
void AliMikhailHaqCopula::load(Advocate & adv)
{
  ArchimedeanCopula::load(adv);
  adv.loadAttribute( "theta_", theta_ );
}

END_NAMESPACE_OPENTURNS
