//                                               -*- C++ -*-
/**
 *  @file  SklarCopula.hxx
 *  @brief The SklarCopula distribution
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
 *  @date   2008-09-09 23:26:29 +0200 (mar, 09 sep 2008)
 *  Id      SklarCopula.hxx 924 2008-09-09 21:26:29Z lebrun
 */
#ifndef OPENTURNS_SKLARCOPULA_HXX
#define OPENTURNS_SKLARCOPULA_HXX

#include "CopulaImplementation.hxx"
#include "Distribution.hxx"
#include "PersistentCollection.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class SklarCopula
 *
 * The SklarCopula distribution.
 */
class SklarCopula
  : public CopulaImplementation
{
  CLASSNAME;
public:

  typedef PersistentCollection<Distribution>  DistributionPersistentCollection;


  /** Default constructor */
  SklarCopula();

  /** Parameters constructor */
  SklarCopula(const Distribution distribution);


  /** Comparison operator */
  Bool operator ==(const SklarCopula & other) const;

  /** String converter */
  String __repr__() const;



  /* Interface inherited from Distribution */
  /** Virtual constructor */
  virtual SklarCopula * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution */
  using CopulaImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using CopulaImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using CopulaImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the quantile of the distribution */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;

  /** Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalPDF(const NumericalScalar x,
                                        const NumericalPoint & y) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x,
                                        const NumericalPoint & y) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  using CopulaImplementation::getMarginal;
  Implementation getMarginal(const Indices & indices) const;

  /** Get the isoprobabilist transformation */
  IsoProbabilisticTransformation getIsoProbabilisticTransformation() const;

  /** Get the inverse isoprobabilist transformation */
  InverseIsoProbabilisticTransformation getInverseIsoProbabilisticTransformation() const;

  /** Get the standard distribution */
  Implementation getStandardDistribution() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /* Interface specific to SklarCopula */

  /** Distribution accessor */
  void setDistribution(const Distribution distribution);
  Distribution getDistribution() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:

  /** The distribution from which the copula is extracted */
  Distribution distribution_;

  /** The marginal distributions of the underlying distribution */
  DistributionPersistentCollection marginalCollection_;

private:

}; /* class SklarCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SKLARCOPULA_HXX */
