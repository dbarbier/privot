//                                               -*- C++ -*-
/**
 *  @file  MinCopula.hxx
 *  @brief A class that implements a min copula
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
 *  @date   2011-11-30 16:59:07 +0100 (mer. 30 nov. 2011)
 *  Id      MinCopula.hxx 2222 2011-11-30 15:59:07Z lebrun
 */
#ifndef OPENTURNS_MINCOPULA_HXX
#define OPENTURNS_MINCOPULA_HXX

#include "OTprivate.hxx"
#include "CopulaImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class MinCopula
 *
 * The class implements a min copula
 */
class MinCopula
  : public CopulaImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  MinCopula(const UnsignedLong dim = 1);

  /** Comparison operator */
  Bool operator ==(const MinCopula & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /* Interface inherited from Distribution */

  /** Virtual constructor */
  virtual MinCopula * clone() const;

  /** Get one realization of the MinCopula distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the MinCopula distribution */
  using CopulaImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the MinCopula distribution */
  using CopulaImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the MinCopula distribution */
  using CopulaImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the quantile of the distribution */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  using CopulaImplementation::getMarginal;
  Implementation getMarginal(const Indices & indices) const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is constinuous */
  Bool isContinuous() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using CopulaImplementation::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & setParametersCollection);

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

private:

  /** Compute the covariance of the distribution */
  virtual void computeCovariance() const;

}; /* class MinCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MINCOPULA_HXX */
