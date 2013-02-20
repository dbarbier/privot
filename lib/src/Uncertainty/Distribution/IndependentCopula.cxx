//                                               -*- C++ -*-
/**
 *  @file  IndependentCopula.cxx
 *  @brief A class that implements an independent copula
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cmath>
#include "IndependentCopula.hxx"
#include "Normal.hxx"
#include "IdentityMatrix.hxx"
#include "NatafIndependentCopulaEvaluation.hxx"
#include "NatafIndependentCopulaGradient.hxx"
#include "NatafIndependentCopulaHessian.hxx"
#include "InverseNatafIndependentCopulaEvaluation.hxx"
#include "InverseNatafIndependentCopulaGradient.hxx"
#include "InverseNatafIndependentCopulaHessian.hxx"
#include "PersistentObjectFactory.hxx"
#include "RandomGenerator.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(IndependentCopula);

static Factory<IndependentCopula> RegisteredFactory("IndependentCopula");

/* Default constructor */
IndependentCopula::IndependentCopula(const UnsignedLong dimension)
  : CopulaImplementation("IndependentCopula")
{
  setDimension(dimension);
}

/* Comparison operator */
Bool IndependentCopula::operator ==(const IndependentCopula & other) const
{
  if (this == &other) return true;
  return getDimension() == other.getDimension();
}

/* String converter */
String IndependentCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << IndependentCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  return oss;
}

/* String converter */
String IndependentCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(dimension = " << getDimension() << ")";
  return oss;
}

/* Virtual constructor */
IndependentCopula * IndependentCopula::clone() const
{
  return new IndependentCopula(*this);
}

/* Get one realization of the distribution */
NumericalPoint IndependentCopula::getRealization() const
{
  return RandomGenerator::Generate(getDimension());
}

/* Get the DDF of the distribution */
NumericalPoint IndependentCopula::computeDDF(const NumericalPoint & point) const
{
  return NumericalPoint(getDimension(), 0.0);
}

/* Compute the probability content of an interval */
NumericalScalar IndependentCopula::computeProbability(const Interval & interval) const
{
  const UnsignedLong dimension(getDimension());
  // Reduce the given interval to the support of the distribution, which is the nD unit cube
  const Interval intersect(interval.intersect(Interval(dimension)));
  // If the intersection is empty
  if (intersect.isNumericallyEmpty()) return 0.0;
  const NumericalPoint lower(intersect.getLowerBound());
  const NumericalPoint upper(intersect.getUpperBound());
  NumericalScalar value(1.0);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      value *= upper[i] - lower[i];
    }
  return value;
}

/* Get the PDF of the distribution */
NumericalScalar IndependentCopula::computePDF(const NumericalPoint & point) const
{
  UnsignedLong dimension(getDimension());
  for (UnsignedLong i = 0; i < dimension; i++)
    {
      NumericalScalar x(point[i]);
      // If one component is outside of the support, the PDF is null
      if ((x <= 0.0) || (x >= 1.0)) return 0.0;
    }
  // The point is in the support
  return 1.0;
}

/* Get the CDF of the distribution */
NumericalScalar IndependentCopula::computeCDF(const NumericalPoint & point) const
{
  UnsignedLong dimension(getDimension());
  NumericalScalar value(1.0);
  for (UnsignedLong i = 0; i < dimension; i++)
    {
      NumericalScalar x(point[i]);
      // If one component is at the left of the support of its matginal distribution, the CDF is null
      if (x <= 0.0) return 0.0;
      // If the component is inside of the support, multiply the value of the CDF by x
      // FIXME
      if (x < 1.0) value *= x;
    }
  return value;
}

/* Get the PDF gradient of the distribution */
NumericalPoint IndependentCopula::computePDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0, 0.0);
}

/* Get the CDF gradient of the distribution */
NumericalPoint IndependentCopula::computeCDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0, 0.0);
}


/* Get the quantile of the distribution */
NumericalPoint IndependentCopula::computeQuantile(const NumericalScalar prob,
                                                  const Bool tail) const
{
  if (prob < 0.0 || prob > 1.0) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  if (prob == 0.0) return getRange().getLowerBound();
  if (prob == 1.0) return getRange().getUpperBound();
  return NumericalPoint(getDimension(), pow(prob, 1.0 / getDimension()));
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
IndependentCopula::Implementation IndependentCopula::getMarginal(const Indices & indices) const
{
  UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // General case
  return new IndependentCopula(indices.getSize());
}

/* Compute the covariance of the distribution */
void IndependentCopula::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  covariance_ = CovarianceMatrix(dimension);
  const NumericalScalar twelveInverse(1.0 / 12.0);
  for (UnsignedLong i = 0; i < dimension; i++)
    covariance_(i, i) = twelveInverse;
  isAlreadyComputedCovariance_ = true;
}

/* Compute the DDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar IndependentCopula::computeConditionalDDF(const NumericalScalar x, const NumericalPoint & y) const
{
  return 0.0;
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar IndependentCopula::computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  if (x < 0.0) return 0.0;
  if (x < 1.0) return 1.0;
  return 0.0;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar IndependentCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if (x < 0.0) return 0.0;
  if (x < 1.0) return x;
  return 1.0;
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar IndependentCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  return q;
}

/* Get the isoprobabilist transformation */
IndependentCopula::IsoProbabilisticTransformation IndependentCopula::getIsoProbabilisticTransformation() const
{
  IsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new NatafIndependentCopulaEvaluation(getDimension()));
  transformation.setGradientImplementation(new NatafIndependentCopulaGradient(getDimension()));
  transformation.setHessianImplementation(new NatafIndependentCopulaHessian(getDimension()));

  return transformation;
}

/* Get the inverse isoprobabilistic transformation */
IndependentCopula::InverseIsoProbabilisticTransformation IndependentCopula::getInverseIsoProbabilisticTransformation() const
{
  InverseIsoProbabilisticTransformation transformation;
  transformation.setEvaluationImplementation(new InverseNatafIndependentCopulaEvaluation(getDimension()));
  transformation.setGradientImplementation(new InverseNatafIndependentCopulaGradient(getDimension()));
  transformation.setHessianImplementation(new InverseNatafIndependentCopulaHessian(getDimension()));

  return transformation;
}

/* Parameters value and description accessor */
IndependentCopula::NumericalPointWithDescriptionCollection IndependentCopula::getParametersCollection() const
{
  // No parameter: empty NumericalPointWithDescription
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

void IndependentCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  if (parametersCollection.getSize() != 1) throw InvalidArgumentException(HERE) << "Error: the given collection is of size=" << parametersCollection.getSize() << ", expected a size=1.";
  if (parametersCollection[0].getDimension() != 0) throw InvalidArgumentException(HERE) << "Error: expected no parameters for the IndependentCopula, got " << parametersCollection[0].getDimension() << " parameters instead.";
}

/* Method save() stores the object through the StorageManager */
void IndependentCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void IndependentCopula::load(Advocate & adv)
{
  CopulaImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
