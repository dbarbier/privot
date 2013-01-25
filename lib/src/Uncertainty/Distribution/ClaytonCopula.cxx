//                                               -*- C++ -*-
/**
 *  @file  ClaytonCopula.cxx
 *  @brief The ClaytonCopula distribution
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
#include "ClaytonCopula.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ClaytonCopula);

static Factory<ClaytonCopula> RegisteredFactory("ClaytonCopula");

/* Default constructor */
ClaytonCopula::ClaytonCopula()
  : ArchimedeanCopula("ClaytonCopula")
  , theta_(2.0)
{
  setDimension( 2 );
}

/* Parameters constructor */
ClaytonCopula::ClaytonCopula(const NumericalScalar theta)
  : ArchimedeanCopula("ClaytonCopula")
  , theta_(theta)
{
  // We set the dimension of the ClaytonCopula distribution
  setDimension( 2 );
}

/* Comparison operator */
Bool ClaytonCopula::operator ==(const ClaytonCopula & other) const
{
  if (this == &other) return true;
  return theta_ == other.theta_;
}

/* String converter */
String ClaytonCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << ClaytonCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " theta=" << theta_;
  return oss;
}

String ClaytonCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(theta = " << theta_ << ")";
  return oss;
}

/* Virtual constructor */
ClaytonCopula * ClaytonCopula::clone() const
{
  return new ClaytonCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint ClaytonCopula::getRealization() const
{
  NumericalPoint realization(2);
  // We use the general algorithm based on conditional CDF inversion
  const NumericalScalar u1(RandomGenerator::Generate());
  realization[0] = u1;
  // The inverse conditional CDF U2|U1
  const NumericalScalar u2(RandomGenerator::Generate());
  realization[1] = u1 * pow(pow(u2, -theta_ / (1.0 + theta_)) - 1.0 + pow(u1, theta_), -1.0 / theta_);
  return realization;
}

/* Get the DDF of the distribution */
NumericalPoint ClaytonCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  // Optimized version given by Maple 11, as there are a lot of pow's involved
  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // A copula has a null DDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0))
    {
      return NumericalPoint(2, 0.0);
    }
  // Independent case
  if (theta_ == 0.0) return NumericalPoint(2, 0.0);
  // General case
  const NumericalScalar powUMinusTheta(pow(u, -theta_));
  const NumericalScalar powVMinusTheta(pow(v, -theta_));
  const NumericalScalar sum1(powUMinusTheta + powVMinusTheta - 1.0);
  const NumericalScalar factor1(pow(sum1, -1.0 / theta_));
  const NumericalScalar factor2(factor1 * powUMinusTheta * powVMinusTheta * (theta_ + 1.0) / (sum1 * sum1 * sum1 * u * v));
  NumericalPoint result(2);
  result[0] =  factor2 * (theta_ * (powUMinusTheta - powVMinusTheta + 1.0) + 1.0 - powVMinusTheta) / u;
  result[1] = -factor2 * (theta_ * (powUMinusTheta - powVMinusTheta - 1.0) - 1.0 + powUMinusTheta) / v;
  return result;
}

/* Get the PDF of the distribution */
NumericalScalar ClaytonCopula::computePDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // A copula has a null PDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0))
    {
      return 0.0;
    }
  // Independent case
  if (theta_ == 0.0) return 1.0;
  // General case
  return pow(pow(u, -theta_) + pow(v, -theta_) - 1.0, -1.0 / theta_ - 2.0) * (1.0 + theta_) * pow(u * v, -theta_ - 1.0);
}

/* Get the CDF of the distribution */
NumericalScalar ClaytonCopula::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // If we are outside of the support, in the lower parts
  if ((u <= 0.0) || (v <= 0.0)) return 0.0;
  // If we are outside of the support, in the upper part
  if ((u >= 1.0) && (v >= 1.0)) return 1.0;
  // If we are outside of the support for u, in the upper part
  if (u >= 1.0) return v;
  // If we are outside of the support for v, in the upper part
  if (v >= 1.0) return u;
  // If we are in the support
  // Independent case
  if (theta_ == 0.0) return u * v;
  // General case
  return pow(pow(u, -theta_) + pow(v, -theta_) - 1.0, -1.0 / theta_);
}

/* Get the PDFGradient of the distribution */
NumericalPoint ClaytonCopula::computePDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // A copula has a null PDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0)) return NumericalPoint(1, 0.0);
  const NumericalScalar t1(pow(u, -theta_));
  const NumericalScalar t2(pow(v, -theta_));
  const NumericalScalar t3(t1 + t2 - 1.0);
  const NumericalScalar t5(pow(t3, -1.0 / theta_));
  const NumericalScalar t7(log(v));
  const NumericalScalar t8(theta_ * theta_);
  const NumericalScalar t9(t7 * t8);
  const NumericalScalar t10(log(u));
  const NumericalScalar t11(t10 * t8);
  const NumericalScalar t16(log(t3));
  const NumericalScalar t17(t16 * t1);
  const NumericalScalar t19(t16 * t2);
  const NumericalScalar t21(t8 * t1);
  const NumericalScalar t24(t8 * t2);
  const NumericalScalar t27(t8 * theta_);
  const NumericalScalar t28(t7 * t27);
  const NumericalScalar t32(t9 + t11 + theta_ * t2 * t7 + theta_ * t1 * t10 + t17 * theta_ + t19 * theta_ + 2 * t21 * t10 + 2 * t24 * t7 - t28 * t1 + t28 * t2 - t9 * t1);
  const NumericalScalar t33(t10 * t27);
  const NumericalScalar t38(-t33 * t2 - t11 * t2 - t16 - t8 + t17 + t19 - t16 * theta_ + t28 + t33 + t21 + t24 + t33 * t1);
  const NumericalScalar t43(t3 * t3);
  const NumericalScalar t51(t5 * t2 * t1 * (t32 + t38) / (t8 * t43 * t3 * v * u));
  return NumericalPoint(1, t51);
}

/* Get the CDFGradient of the distribution */
NumericalPoint ClaytonCopula::computeCDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  const NumericalScalar u(point[0]);
  const NumericalScalar v(point[1]);
  // If we are outside of the support, in the lower parts
  if ((u <= 0.0) || (v <= 0.0)) return NumericalPoint(1, 0.0);
  // If we are outside of the support, in the upper part
  if ((u >= 1.0) && (v >= 1.0)) return NumericalPoint(1, 0.0);
  // If we are outside of the support for u, in the upper part
  if (u >= 1.0) return NumericalPoint(1, 0.0);
  // If we are outside of the support for v, in the upper part
  if (v >= 1.0) return NumericalPoint(1, 0.0);
  // If we are in the support
  const NumericalScalar powUMinusTheta(pow(u, -theta_));
  const NumericalScalar powVMinusTheta(pow(v, -theta_));
  const NumericalScalar sum1(powUMinusTheta + powVMinusTheta - 1.0);
  const NumericalScalar factor1(pow(sum1, -1.0 / theta_));
  return NumericalPoint(1, factor1 * (log(sum1) * (powUMinusTheta + powVMinusTheta - 1) + theta_ * (powUMinusTheta * log(u) + powVMinusTheta * log(v))) / (theta_ * theta_ * sum1));
}

/* Get the quantile of the distribution */
NumericalPoint ClaytonCopula::computeQuantile(const NumericalScalar prob,
                                              const Bool tail) const
{
  if ((prob < 0.0) || (prob > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  // Special case for boarding values
  if (prob == 0.0) return getRange().getLowerBound();
  if (prob == 1.0) return getRange().getUpperBound();
  // Independent case
  if (theta_ == 0.0) return NumericalPoint(2, sqrt(prob));
  // General case
  return NumericalPoint(2, exp((M_LN2 - log1p(pow(prob, -theta_))) / theta_));
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ClaytonCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return x;
  const NumericalScalar u(y[0]);
  const NumericalScalar v(x);
  // If we are in the support
  return pow(pow(u, -theta_) + pow(v, -theta_) - 1.0, -1.0 - 1.0 / theta_) * pow(u, -1.0 - theta_);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ClaytonCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  // Special case for no conditioning or independent copula
  if ((q == 0.0) || (q == 1.0)) return q;
  // Initialize the conditional quantile with the quantile of the i-th marginal distribution
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return q;
  const NumericalScalar z(y[0]);
  return z * pow(pow(q, -theta_ / (1.0 + theta_)) - 1.0 + pow(z, theta_), -1.0 / theta_);
}

/* Tell if the distribution has independent copula */
Bool ClaytonCopula::hasIndependentCopula() const
{
  return (theta_ == 0.0);
}

/* Compute the archimedean generator of the archimedean copula, i.e.
 * the function phi such that the CDF of the copula can
 * be written as CDF(t) = phi^{-1}(phi(u)+phi(v))
 */
NumericalScalar ClaytonCopula::computeArchimedeanGenerator(const NumericalScalar t) const
{
  if (theta_ == 0.0) return 0.0;
  return pow(t, theta_) -  1.0;
}

/* Compute the inverse of the archimedean generator */
NumericalScalar ClaytonCopula::computeInverseArchimedeanGenerator(const NumericalScalar t) const
{
  if (theta_ == 0.0) return 0.0;
  return pow(t + 1.0, 1.0 / theta_);
}

/* Compute the derivative of the density generator */
NumericalScalar ClaytonCopula::computeArchimedeanGeneratorDerivative(const NumericalScalar t) const
{
  if (theta_ == 0.0) return 0.0;
  return theta_ * pow(t, theta_ - 1.0);
}

/* Compute the seconde derivative of the density generator */
NumericalScalar ClaytonCopula::computeArchimedeanGeneratorSecondDerivative(const NumericalScalar t) const
{
  if (theta_ == 0.0) return 0.0;
  return theta_ * (theta_ - 1) * pow(t, theta_ - 2.0);
}

/* Parameters value and description accessor */
ClaytonCopula::NumericalPointWithDescriptionCollection ClaytonCopula::getParametersCollection() const
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

void ClaytonCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = ClaytonCopula(parametersCollection[0][0]);
}

/* Theta accessor */
void ClaytonCopula::setTheta(const NumericalScalar theta)
{
  if (theta < 0.0) throw InvalidArgumentException(HERE) << "Theta MUST be nonnegative";
  theta_ = theta;
}

/* Theta accessor */
NumericalScalar ClaytonCopula::getTheta() const
{
  return theta_;
}

/* Method save() stores the object through the StorageManager */
void ClaytonCopula::save(Advocate & adv) const
{
  ArchimedeanCopula::save(adv);
  adv.saveAttribute( "theta_", theta_ );
}

/* Method load() reloads the object from the StorageManager */
void ClaytonCopula::load(Advocate & adv)
{
  ArchimedeanCopula::load(adv);
  adv.loadAttribute( "theta_", theta_ );
}

END_NAMESPACE_OPENTURNS
