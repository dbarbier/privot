//                                               -*- C++ -*-
/**
 *  @file  AliMikhailHaqCopula.hxx
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
 *  @author schueller
 *  @date   2012-02-17 19:35:43 +0100 (ven. 17 févr. 2012)
 *  Id      AliMikhailHaqCopula.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_ALIMIKHAILHAQCOPULA_HXX
#define OPENTURNS_ALIMIKHAILHAQCOPULA_HXX

#include "ArchimedeanCopula.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class AliMikhailHaqCopula
 *
 * The AliMikhailHaqCopula distribution.
 */
class AliMikhailHaqCopula
  : public ArchimedeanCopula
{
  CLASSNAME;
public:

  /** Default constructor */
  AliMikhailHaqCopula();

  /** Parameters constructor */
  AliMikhailHaqCopula(const NumericalScalar theta);

  /** Comparison operator */
  Bool operator ==(const AliMikhailHaqCopula & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /* Interface inherited from Distribution */
  /** Virtual constructor */
  virtual AliMikhailHaqCopula * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the DDF of the distribution */
  using ArchimedeanCopula::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  using ArchimedeanCopula::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using ArchimedeanCopula::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the quantile of the distribution */
  NumericalPoint computeQuantile(const NumericalScalar prob,
                                 const Bool tail = false) const;

  /** Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const;

  /** Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
  NumericalScalar computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const;

  /** Compute the archimedean generator of the archimedean copula, i.e.
   *  the function phi such that the CDF of the copula can
   *  be written as CDF(t) = phi^{-1}(phi(u)+phi(v))
   */
  NumericalScalar computeArchimedeanGenerator(const NumericalScalar t) const;

  /** Compute the inverse of the archimedean generator */
  NumericalScalar computeInverseArchimedeanGenerator(const NumericalScalar t) const;

  /** Compute the derivative of the archimedean generator */
  NumericalScalar computeArchimedeanGeneratorDerivative(const NumericalScalar t) const;

  /** Compute the second derivative of the archimedean generator */
  NumericalScalar computeArchimedeanGeneratorSecondDerivative(const NumericalScalar t) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using ArchimedeanCopula::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /* Interface specific to AliMikhailHaqCopula */

  /** Theta accessor */
  void setTheta(const NumericalScalar theta);
  NumericalScalar getTheta() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

  /** The parameter of the AliMikhailHaqCopula distribution */
  NumericalScalar theta_;

}; /* class AliMikhailHaqCopula */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ALIMIKHAILHAQCOPULA_HXX */
