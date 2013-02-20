//                                               -*- C++ -*-
/**
 * @file  NoNumericalMathEvaluationImplementation.cxx
 * @brief Class for unavailable evaluation implementations
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
 * @author schueller
 * @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "NoNumericalMathEvaluationImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NoNumericalMathEvaluationImplementation);

static Factory<NoNumericalMathEvaluationImplementation> RegisteredFactory("NoNumericalMathEvaluationImplementation");

/* Default constructor */
NoNumericalMathEvaluationImplementation::NoNumericalMathEvaluationImplementation()
  : NumericalMathEvaluationImplementation()
{
  // Nothing to do
}

/* Virtual constructor */
NoNumericalMathEvaluationImplementation * NoNumericalMathEvaluationImplementation::clone() const
{
  return new NoNumericalMathEvaluationImplementation(*this);
}

/* Comparison operator */
Bool NoNumericalMathEvaluationImplementation::operator ==(const NoNumericalMathEvaluationImplementation & other) const
{
  return true;
}

/* String converter */
String NoNumericalMathEvaluationImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << NoNumericalMathEvaluationImplementation::GetClassName()
      << " name=" << getName();
  return oss;
}

/* Test for actual implementation */
Bool NoNumericalMathEvaluationImplementation::isActualImplementation() const
{
  return false;
}






/* Here is the interface that all derived class must implement */

/* Operator () */
NumericalPoint NoNumericalMathEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(getInputDimension());
  if (inP.getDimension() != inputDimension) throw InvalidArgumentException(HERE) << "Error: the given point has an invalid dimension. Expect a dimension " << inputDimension << ", got " << inP.getDimension();
  ++callsNumber_;
  const NumericalPoint result;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong NoNumericalMathEvaluationImplementation::getInputDimension() const
{
  return 0;
}

/* Accessor for output point dimension */
UnsignedLong NoNumericalMathEvaluationImplementation::getOutputDimension() const
{
  return 0;
}

END_NAMESPACE_OPENTURNS
