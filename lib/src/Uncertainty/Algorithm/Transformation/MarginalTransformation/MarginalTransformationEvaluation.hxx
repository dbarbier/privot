//                                               -*- C++ -*-
/**
 *  @file  MarginalTransformationEvaluation.hxx
 *  @brief Class for the Nataf transformationEvaluation evaluation for elliptical
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
 *  Id      MarginalTransformationEvaluation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_MARGINALTRANSFORMATIONEVALUATION_HXX
#define OPENTURNS_MARGINALTRANSFORMATIONEVALUATION_HXX

#include "OTprivate.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalMathFunction.hxx"
#include "Matrix.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "Distribution.hxx"
#include "StorageManager.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class MarginalTransformationEvaluation
 *
 * This class offers an interface for the Nataf function for elliptical distributions
 */
class MarginalTransformationEvaluation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  typedef Collection<Distribution>              DistributionCollection;
  typedef PersistentCollection<Distribution>    DistributionPersistentCollection;

  enum TranformationDirection { FROM, TO, FROMTO };

  /** Default constructor */
  MarginalTransformationEvaluation();

  /** Parameter constructor */
  MarginalTransformationEvaluation(const DistributionCollection & distributionCollection,
                                   const UnsignedLong direction = FROM);

  /** Parameter constructor */
  MarginalTransformationEvaluation(const DistributionCollection & inputDistributionCollection,
                                   const DistributionCollection & outputDistributionCollection,
                                   const Bool simplify = true);

  /** Virtual constructor */
  virtual MarginalTransformationEvaluation * clone() const;

  /** Evaluation */
  NumericalPoint operator () (const NumericalPoint & inP) const;

  /** Gradient according to the marginal parameters */
  Matrix parametersGradient(const NumericalPoint & inP) const;

  /** Accessor for input point dimension */
  UnsignedLong getInputDimension() const;

  /** Accessor for output point dimension */
  UnsignedLong getOutputDimension() const;

  /** Direction accessor */
  void setDirection(const TranformationDirection direction);
  UnsignedLong getDirection() const;

  /** Input distribution collection accessor */
  void setInputDistributionCollection(const DistributionCollection & inputDistributionCollection);
  DistributionCollection getInputDistributionCollection() const;

  /** Output distribution collection accessor */
  void setOutputDistributionCollection(const DistributionCollection & outputDistributionCollection);
  DistributionCollection getOutputDistributionCollection() const;

  /** Simplifications accessor */
  Collection<UnsignedLong> getSimplifications() const;

  /** Expressions accessor */
  Collection<NumericalMathFunction> getExpressions() const;

  /** String converter */
  String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:
  // Make the gradient and the hessian friend classes of the evaluation in order to share the input and output distribution collections
  friend class MarginalTransformationGradient;
  friend class MarginalTransformationHessian;

  // marginal distributions of the input
  DistributionPersistentCollection inputDistributionCollection_;

  // Marginal distributions of the output
  DistributionPersistentCollection outputDistributionCollection_;

  // Direction of the transformation
  UnsignedLong direction_;

  // Flag to tell if simpler expressions are available
  Collection<UnsignedLong> simplifications_;

  // Collection of simpler expressions
  Collection<NumericalMathFunction> expressions_;
}; /* MarginalTransformationEvaluation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MARGINALTRANSFORMATIONEVALUATION_HXX */
