//                                               -*- C++ -*-
/**
 *  @file  NonCenteredFiniteDifferenceGradient.cxx
 *  @brief Class for the creation of a numerical math gradient implementation
 *         form a numerical math evaluation implementation by using noncentered
 *         finite difference formula.
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
 */
#include <cstdlib>

#include "NonCenteredFiniteDifferenceGradient.hxx"
#include "PersistentObjectFactory.hxx"


BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NonCenteredFiniteDifferenceGradient);


static Factory<NonCenteredFiniteDifferenceGradient> RegisteredFactory("NonCenteredFiniteDifferenceGradient");

/* Default constructor */
NonCenteredFiniteDifferenceGradient::NonCenteredFiniteDifferenceGradient() :
  FiniteDifferenceGradient()
{
  // Nothing to do
}

/* Parameter constructor */
NonCenteredFiniteDifferenceGradient::NonCenteredFiniteDifferenceGradient(const NumericalPoint & epsilon,
                                                                         const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(epsilon, p_evaluation)
{
}

/* Parameter constructor */
NonCenteredFiniteDifferenceGradient::NonCenteredFiniteDifferenceGradient(const NumericalScalar epsilon,
                                                                         const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(epsilon, p_evaluation)
{
}

/* Parameter constructor */
NonCenteredFiniteDifferenceGradient::NonCenteredFiniteDifferenceGradient(const FiniteDifferenceStep & step,
                                                                         const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(step, p_evaluation)
{
  // Nothing to do
}

/* Virtual constructor */
NonCenteredFiniteDifferenceGradient * NonCenteredFiniteDifferenceGradient::clone() const
{
  return new NonCenteredFiniteDifferenceGradient(*this);
}


/* String converter */
String NonCenteredFiniteDifferenceGradient::__repr__() const
{
  OSS oss;
  oss << "class=" << NonCenteredFiniteDifferenceGradient::GetClassName()
      << " name=" << getName()
      << " epsilon=" << getEpsilon()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String NonCenteredFiniteDifferenceGradient::__str__(const String & offset) const
{
  return OSS(false) << offset << "NonCenteredFiniteDifferenceGradient epsilon : " << getEpsilon();
}

/* Here is the interface that all derived class must implement */

/* Gradient () */
Matrix NonCenteredFiniteDifferenceGradient::gradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(inP.getDimension());
  NumericalPoint step(finiteDifferenceStep_.operator()(inP));
  if (inputDimension != step.getDimension()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  /* At which points do we have to compute the evaluation for the decentered finite difference. We need 1+dim pionts. */
  NumericalSample gridPoints(inputDimension + 1, inP);
  for(UnsignedLong i = 0; i < inputDimension; ++i) gridPoints[i][i] += step[i];
  /* Evaluate the evaluation */
  NumericalSample gridValues(p_evaluation_->operator()(gridPoints));
  /* Get the value at the center of the grid */
  NumericalPoint center(gridValues[inputDimension]);
  /* Compute the gradient */
  Matrix result(p_evaluation_->getInputDimension(), p_evaluation_->getOutputDimension());
  for (UnsignedLong i = 0; i < result.getNbRows(); ++i)
    for (UnsignedLong j = 0; j < result.getNbColumns(); ++j)
      /* result(i, j) = (f_j(x + e_i) - f_j(x)) / e_i ~ df_j / dx_i */
      result(i, j) = (gridValues[i][j] - center[j]) / step[i];
  return result;
}

END_NAMESPACE_OPENTURNS
