//                                               -*- C++ -*-
/**
 *  @file  InverseNatafEllipticalCopulaEvaluation.hxx
 *  @brief Class for the InverseNataf transformation evaluation for elliptical
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      InverseNatafEllipticalCopulaEvaluation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_INVERSENATAFELLIPTICALCOPULAEVALUATION_HXX
#define OPENTURNS_INVERSENATAFELLIPTICALCOPULAEVALUATION_HXX

#include "OTprivate.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "Matrix.hxx"
#include "SquareMatrix.hxx"
#include "NumericalPoint.hxx"
#include "Distribution.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class InverseNatafEllipticalCopulaEvaluation
 *
 * This class offers an interface for the InverseNataf function for elliptical distributions
 */
class InverseNatafEllipticalCopulaEvaluation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  InverseNatafEllipticalCopulaEvaluation();

  /** Parameter constructor */
  InverseNatafEllipticalCopulaEvaluation(const Distribution & standardDistribution,
                                         const SquareMatrix & cholesky);


  /** Virtual constructor */
  virtual InverseNatafEllipticalCopulaEvaluation * clone() const;

  /** Evaluation */
  NumericalPoint operator () (const NumericalPoint & inP) const;

  /** Gradient according to the marginal parameters */
  virtual Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  virtual UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  virtual UnsignedLong getOutputDimension() const;

  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  Distribution standardDistribution_;
  Matrix cholesky_;

}; /* InverseNatafEllipticalCopulaEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_INVERSENATAFELLIPTICALCOPULAEVALUATION_HXX */
