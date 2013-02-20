//                                               -*- C++ -*-
/**
 *  @file  FarlieGumbelMorgensternCopula.hxx
 *  @brief The FarlieGumbelMorgensternCopula distribution
 *
 *  (C) Copyright 2005-2010 EDF-EADS-Phimeca
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
 *  @date   2011-02-26 21:12:17 +0100 (sam. 26 févr. 2011)
 *  Id      FarlieGumbelMorgensternCopula.hxx 1813 2011-02-26 20:12:17Z lebrun
 */
#ifndef OPENTURNS_FARLIEGUMBELMORGENSTERNCOPULA_HXX
#define OPENTURNS_FARLIEGUMBELMORGENSTERNCOPULA_HXX

#include "CopulaImplementation.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FarlieGumbelMorgensternCopula
 *
 * The FarlieGumbelMorgensternCopula distribution.
 */
class FarlieGumbelMorgensternCopula
  : public CopulaImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  FarlieGumbelMorgensternCopula();

  /** Parameters constructor */
  FarlieGumbelMorgensternCopula(const NumericalScalar theta);

  /** Comparison operator */
  Bool operator ==(const FarlieGumbelMorgensternCopula & other) const;

  /** String converter */
  String __repr__() const;



  /* Interface inherited from Distribution */
  /** Virtual constructor */
  virtual FarlieGumbelMorgensternCopula * clone() const;

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

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using CopulaImplementation::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /* Interface specific to FarlieGumbelMorgensternCopula */

  /** Theta accessor */
  void setTheta(const NumericalScalar theta);
  NumericalScalar getTheta() const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  using CopulaImplementation::getMarginal;
  Implementation getMarginal(const Indices & indices) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  /** The parameter of the FarlieGumbelMorgensternCopula distribution */
  NumericalScalar theta_;

}; /* class FarlieGumbelMorgensternCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FARLIEGUMBELMORGENSTERNCOPULA_HXX */
