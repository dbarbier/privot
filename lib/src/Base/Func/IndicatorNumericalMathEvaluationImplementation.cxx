//                                               -*- C++ -*-
/**
 * @file  IndicatorNumericalMathEvaluationImplementation.cxx
 * @brief The class that implements the composition between numerical
 *        math functions implementations
 *
 * Copyright (C) 2005-2013 EDF-EADS-Phimeca
 *
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 *
 *
 * @author lebrun
 * @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
 */

#include "IndicatorNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Less.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(IndicatorNumericalMathEvaluationImplementation);

static Factory<IndicatorNumericalMathEvaluationImplementation> RegisteredFactory("IndicatorNumericalMathEvaluationImplementation");

/* Default constructor */
IndicatorNumericalMathEvaluationImplementation::IndicatorNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    p_evaluation_(),
    comparisonOperator_(),
    threshold_(0.0)
{
  // Nothing to do
} // IndicatorNumericalMathEvaluationImplementation

/* Default constructor */
IndicatorNumericalMathEvaluationImplementation::IndicatorNumericalMathEvaluationImplementation(const EvaluationImplementation & p_evaluation,
                                                                                               const ComparisonOperator & comparisonOperator,
                                                                                               const NumericalScalar threshold)
  : NumericalMathEvaluationImplementation(),
    p_evaluation_(),
    comparisonOperator_(comparisonOperator),
    threshold_(threshold)
{
  setEvaluationImplementation(p_evaluation);
  setDescription(p_evaluation->getDescription());
} // IndicatorNumericalMathEvaluationImplementation

/* Virtual constructor */
IndicatorNumericalMathEvaluationImplementation * IndicatorNumericalMathEvaluationImplementation::clone() const
{
  return new IndicatorNumericalMathEvaluationImplementation(*this);
}


/* Comparison operator */
Bool IndicatorNumericalMathEvaluationImplementation::operator ==(const IndicatorNumericalMathEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String IndicatorNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << IndicatorNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName()
      << " evaluation=" << p_evaluation_->__repr__()
      << " comparisonOperator=" << comparisonOperator_
      << " threshold=" << threshold_;
  return oss;
}

/* Operator () */
NumericalPoint IndicatorNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  const NumericalPoint result(1, (comparisonOperator_.compare(p_evaluation_->operator()(inP)[0], threshold_) ? 1.0 : 0.0));
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong IndicatorNumericalMathEvaluationImplementation::getInputDimension() const
{
  return p_evaluation_->getInputDimension();
}

/* Accessor for output point dimension */
UnsignedLong IndicatorNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return 1;
}

/* Accessor for the underlying evaluation */
IndicatorNumericalMathEvaluationImplementation::EvaluationImplementation IndicatorNumericalMathEvaluationImplementation::getEvaluationImplementation() const
{
  return p_evaluation_;
}

void IndicatorNumericalMathEvaluationImplementation::setEvaluationImplementation(const EvaluationImplementation & p_evaluation)
{
  if (p_evaluation->getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot use an evaluation implementation with output dimension not equal to 1";
  p_evaluation_ = p_evaluation;
}

/* Accessor for the comparison operator */
ComparisonOperator IndicatorNumericalMathEvaluationImplementation::getComparisonOperator() const
{
  return comparisonOperator_;
}

void IndicatorNumericalMathEvaluationImplementation::setComparisonOperator(const ComparisonOperator & comparisonOperator)
{
  comparisonOperator_ = comparisonOperator;
}

/* Accessor for the threshold */
NumericalScalar IndicatorNumericalMathEvaluationImplementation::getThreshold() const
{
  return threshold_;
}

void IndicatorNumericalMathEvaluationImplementation::setThreshold(const NumericalScalar threshold)
{
  threshold_ = threshold;
}

/* Method save() stores the object through the StorageManager */
void IndicatorNumericalMathEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "evaluation_", *p_evaluation_ );
  adv.saveAttribute( "comparisonOperator_", comparisonOperator_ );
  adv.saveAttribute( "threshold_", threshold_ );
}

/* Method load() reloads the object from the StorageManager */
void IndicatorNumericalMathEvaluationImplementation::load(Advocate & adv)
{
  TypedInterfaceObject<NumericalMathEvaluationImplementation> evaluation;
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "evaluation_", evaluation );
  p_evaluation_ = evaluation.getImplementation();
  adv.loadAttribute( "comparisonOperator_", comparisonOperator_ );
  adv.loadAttribute( "threshold_", threshold_ );
}

END_NAMESPACE_OPENTURNS
