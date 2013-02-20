//                                               -*- C++ -*-
/**
 *  @file  GumbelCopula.cxx
 *  @brief The GumbelCopula distribution
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
#include "GumbelCopula.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "DistFunc.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(GumbelCopula);

static Factory<GumbelCopula> RegisteredFactory("GumbelCopula");

/* Default constructor */
GumbelCopula::GumbelCopula()
  : ArchimedeanCopula("GumbelCopula")
  , theta_(2.0)
{
  // We set the dimension of the GumbelCopula distribution
  setDimension( 2 );
}

/* Parameters constructor */
GumbelCopula::GumbelCopula(const NumericalScalar theta)
  : ArchimedeanCopula("GumbelCopula")
  , theta_(0.0)
{
  // We set the dimension of the GumbelCopula distribution
  setDimension( 2 );
  // Check the value of theta
  setTheta(theta);
}

/* Comparison operator */
Bool GumbelCopula::operator ==(const GumbelCopula & other) const
{
  if (this == &other) return true;
  return theta_ == other.theta_;
}

/* String converter */
String GumbelCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << GumbelCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " theta=" << theta_;
  return oss;
}

String GumbelCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(theta = " << theta_ << ")";
  return oss;
}

/* Virtual constructor */
GumbelCopula * GumbelCopula::clone() const
{
  return new GumbelCopula(*this);
}

/* Get one realization of the distribution
   Algorithm using stable distribution, see:
   Kjersti Aas, "Modelling the dependence structure of financial assets: a survey of four copulas",
   Norwegian Computing Center report nr. SAMBA/22/04, December 2004.
*/
NumericalPoint GumbelCopula::getRealization() const
{
  NumericalPoint realization(2);
  NumericalScalar u((RandomGenerator::Generate() - 0.5) * M_PI);
  NumericalScalar u2(u + M_PI_2);
  NumericalScalar e(-log(RandomGenerator::Generate()));
  NumericalScalar inverseTheta(1.0 / theta_);
  NumericalScalar t(cos(u - u2 * inverseTheta) / e);
  NumericalScalar gamma(pow(sin(u2 * inverseTheta) / t, inverseTheta) * t / cos(u));
  realization[0] = exp(-pow(-log(RandomGenerator::Generate()), inverseTheta) / gamma);
  realization[1] = exp(-pow(-log(RandomGenerator::Generate()), inverseTheta) / gamma);
  return realization;
}

/* Get the DDF of the distribution */
NumericalPoint GumbelCopula::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar u(point[0]);
  NumericalScalar v(point[1]);
  // A copula has a null PDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0)) return NumericalPoint(2, 0.0);
  NumericalPoint result(2);
  const NumericalScalar t1(log(u));
  const NumericalScalar t2(pow(-t1, theta_));
  const NumericalScalar t3(log(v));
  const NumericalScalar t4(pow(-t3, theta_));
  const NumericalScalar t5(t2 + t4);
  const NumericalScalar t7(pow(t5, 1.0 / theta_));
  const NumericalScalar t8(t2 * t2);
  const NumericalScalar t10(pow(u, 2.0));
  const NumericalScalar t11(1.0 / t10);
  const NumericalScalar t12(t1 * t1);
  const NumericalScalar t13(1.0 / t12);
  const NumericalScalar t14(t11 * t13);
  const NumericalScalar t15(t5 * t5);
  const NumericalScalar t17(1.0 / t15 / t5);
  const NumericalScalar t20(exp(-t7));
  const NumericalScalar t22(-1.0 + theta_ + t7);
  const NumericalScalar t23(1.0 / v);
  const NumericalScalar t25(1.0 / t3);
  const NumericalScalar t26(t22 * t23 * t25);
  const NumericalScalar t29(t7 * t4);
  const NumericalScalar t35(t23 * t25);
  const NumericalScalar t36(1.0 / t15);
  const NumericalScalar t40(t7 * t7);
  const NumericalScalar t41(t40 * t4);
  const NumericalScalar t48(t8 * t20);
  const NumericalScalar t51(t23 * t17 * t25);
  const NumericalScalar t62(t29 * t2 * t20 * t22);
  const NumericalScalar t63(t36 * t11);
  result[0] = t7 * t8 * t14 * t17 * t4 * t20 * t26 + t29 * t2 * theta_ * t11 * t13 * t20 * t22 * t35 * t36 - t41 * t8 * t11 * t13 * t17 * t20 * t26 + t41 * t48 * t14 * t51 - 2.0 * t29 * t48 * t22 * t51 * t14 * theta_ - t62 * t35 * t63 / t1 - t62 * t35 * t63 * t13;

  const NumericalScalar t8b(t4 * t4);
  const NumericalScalar t9b(t7 * t8b);
  const NumericalScalar t10b(pow(v, 2.0));
  const NumericalScalar t11b(1.0 / t10b);
  const NumericalScalar t12b(t3 * t3);
  const NumericalScalar t13b(1.0 / t12b);
  const NumericalScalar t14b(t11b * t13b);
  const NumericalScalar t18(t14b * t17);
  const NumericalScalar t21(t2 * t20);
  const NumericalScalar t23b(1.0 / u);
  const NumericalScalar t26b(t22 * t23b * t25);
  const NumericalScalar t33(t21 * t22);
  const NumericalScalar t36b(1.0 / t15 * t23b * t25);
  const NumericalScalar t40b(t40 * t8b);
  const NumericalScalar t52(t29 * t33);
  result[1] = t9b * t18 * t21 * t26b + t29 * theta_ * t11b * t13b * t33 * t36b - t40b * t11b * t2 * t13b * t17 * t20 * t26b + t40b * t21 * t14b * t23b * t17 * t25 - t52 * t11b / t3 * t36b - t52 * t14b * t36b - 2.0 * t9b * t33 * t18 * t23b * t25 * theta_;
  return result;
}

/* Get the PDF of the distribution */
NumericalScalar GumbelCopula::computePDF(const NumericalPoint & point) const
{
  NumericalScalar u(point[0]);
  NumericalScalar v(point[1]);
  // A copula has a null PDF outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0)) return 0.0;
  NumericalScalar logU(log(u));
  NumericalScalar logV(log(v));
  NumericalScalar minusLogUPowerTheta(pow(-logU, theta_));
  NumericalScalar minusLogVPowerTheta(pow(-logV, theta_));
  NumericalScalar sum1(minusLogUPowerTheta + minusLogVPowerTheta);
  NumericalScalar pow1(pow(sum1, 1.0 / theta_));
  return pow1 * minusLogUPowerTheta * minusLogVPowerTheta * exp(-pow1) * (pow1 + theta_ - 1.0) / (u * v * logU * logV * sum1 * sum1);
}

/* Get the CDF of the distribution */
NumericalScalar GumbelCopula::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar u(point[0]);
  NumericalScalar v(point[1]);
  // If we are outside of the support, in the lower parts
  if ((u <= 0.0) || (v <= 0.0)) return 0.0;
  // If we are outside of the support, in the upper part
  if ((u >= 1.0) && (v >= 1.0)) return 1.0;
  // If we are outside of the support for u, in the upper part
  if (u >= 1.0) return v;
  // If we are outside of the support for v, in the upper part
  if (v >= 1.0) return u;
  // If we are in the support
  return exp(-pow(pow(-log(u), theta_) + pow(-log(v), theta_), 1.0 / theta_));
}

/* Get the PDFGradient of the distribution */
NumericalPoint GumbelCopula::computePDFGradient(const NumericalPoint & point) const
{
  NumericalScalar u(point[0]);
  NumericalScalar v(point[1]);
  // A copula has a null PDF gradient outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0)) return NumericalPoint(1, 0.0);
  const NumericalScalar epsilon(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"));
  return NumericalPoint(1, (GumbelCopula(theta_ + epsilon).computePDF(point) - GumbelCopula(theta_ - epsilon).computePDF(point)) / (2.0 * epsilon));
}

/* Get the CDFGradient of the distribution */
NumericalPoint GumbelCopula::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalScalar u(point[0]);
  NumericalScalar v(point[1]);
  // A copula has a null CDF gradient outside of ]0, 1[^2
  if ((u <= 0.0) || (u >= 1.0) || (v <= 0.0) || (v >= 1.0)) return NumericalPoint(1, 0.0);
  // If we are in the support
  NumericalScalar logU(log(u));
  NumericalScalar logV(log(v));
  NumericalScalar minusLogUPowerTheta(pow(-logU, theta_));
  NumericalScalar minusLogVPowerTheta(pow(-logV, theta_));
  NumericalScalar sum1(minusLogUPowerTheta + minusLogVPowerTheta);
  NumericalScalar inverseTheta(1.0 / theta_);
  NumericalScalar pow1(pow(sum1, inverseTheta));
  return NumericalPoint(1, pow1 * exp(-pow1) * inverseTheta * (log(sum1) * inverseTheta - (minusLogUPowerTheta * log(-logU) + minusLogVPowerTheta * log(-logV)) / sum1));
}

/* Get the quantile of the distribution */
NumericalPoint GumbelCopula::computeQuantile(const NumericalScalar prob,
                                             const Bool tail) const
{
  if ((prob < 0.0) || (prob > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  if (prob == 0.0) return getRange().getLowerBound();
  if (prob == 1.0) return getRange().getUpperBound();
  return NumericalPoint(2, exp(-exp(log(-log(prob)) - M_LN2 / theta_)));
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar GumbelCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return x;
  const NumericalScalar u(y[0]);
  const NumericalScalar v(x);
  // If we are in the support
  const NumericalScalar minusLogU(-log(u));
  const NumericalScalar minusLogUPowTheta(pow(minusLogU, theta_));
  const NumericalScalar minusLogVPowTheta(pow(-log(v), theta_));
  const NumericalScalar sum(minusLogUPowTheta + minusLogVPowTheta);
  return pow(sum, -1.0 + 1.0 / theta_) * minusLogUPowTheta * exp(-pow(sum, 1.0 / theta_)) / (u * minusLogU);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar GumbelCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q <= 0.0) || (q >= 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  if (q == 0.0) return 0.0;
  if (q == 1.0) return 1.0;
  // Initialize the conditional quantile with the quantile of the i-th marginal distribution
  // Special case when no contitioning or independent copula
  if ((conditioningDimension == 0) || hasIndependentCopula()) return q;
  const NumericalScalar u(y[0]);
  const NumericalScalar inverseThetaMinusOne(1.0 / (theta_ - 1.0));
  const NumericalScalar minusLogU(-log(u));
  const NumericalScalar minusLogUPowTheta(pow(minusLogU, theta_));
  const NumericalScalar factor(minusLogUPowTheta / (u * q * minusLogU));
  return exp(-pow(exp(theta_ * (log(factor) / (theta_ - 1.0) - SpecFunc::LambertW(pow(factor, inverseThetaMinusOne) * inverseThetaMinusOne))) - minusLogUPowTheta, 1.0 / theta_));
}

/* Parameters value and description accessor */
GumbelCopula::NumericalPointWithDescriptionCollection GumbelCopula::getParametersCollection() const
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

void GumbelCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  *this = GumbelCopula(parametersCollection[0][0]);
}

/* Compute the archimedean generator of the archimedean copula, i.e.
 * the function phi such that the CDF of the copula can
 * be written as CDF(t) = phi^{-1}(phi(u)+phi(v))
 */
NumericalScalar GumbelCopula::computeArchimedeanGenerator(const NumericalScalar t) const
{
  return pow(-log(t), theta_);
}

/* Compute the inverse of the archimedean generator */
NumericalScalar GumbelCopula::computeInverseArchimedeanGenerator(const NumericalScalar t) const
{
  return exp(-pow(t, 1.0 / theta_));
}

/* Compute the derivative of the density generator */
NumericalScalar GumbelCopula::computeArchimedeanGeneratorDerivative(const NumericalScalar t) const
{
  return -theta_ * pow(-log(t), theta_ - 1.0) / t;
}

/* Compute the seconde derivative of the density generator */
NumericalScalar GumbelCopula::computeArchimedeanGeneratorSecondDerivative(const NumericalScalar t) const
{
  NumericalScalar logT(log(t));
  return theta_ * (theta_ - logT - 1.0) * pow(-logT, theta_ - 2.0) / (t * t);
}

/* Tell if the distribution has independent copula */
Bool GumbelCopula::hasIndependentCopula() const
{
  return (theta_ == 1.0);
}

/* Theta accessor */
void GumbelCopula::setTheta(const NumericalScalar theta)
{
  if (theta <= 1.0) throw InvalidArgumentException(HERE) << "Theta MUST be strictly greater than 1";
  theta_ = theta;
}

/* Theta accessor */
NumericalScalar GumbelCopula::getTheta() const
{
  return theta_;
}

/* Method save() stores the object through the StorageManager */
void GumbelCopula::save(Advocate & adv) const
{
  ArchimedeanCopula::save(adv);
  adv.saveAttribute( "theta_", theta_ );
}

/* Method load() reloads the object from the StorageManager */
void GumbelCopula::load(Advocate & adv)
{
  ArchimedeanCopula::load(adv);
  adv.loadAttribute( "theta_", theta_ );
}




END_NAMESPACE_OPENTURNS
