//                                               -*- C++ -*-
/**
 *  @file  CenteredFiniteDifferenceGradient.cxx
 *  @brief Class for the creation of a numerical math gradient implementation
 *         form a numerical math evaluation implementation by using centered
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

#include "CenteredFiniteDifferenceGradient.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(CenteredFiniteDifferenceGradient);


static Factory<CenteredFiniteDifferenceGradient> RegisteredFactory("CenteredFiniteDifferenceGradient");

/* Default constructor */
CenteredFiniteDifferenceGradient::CenteredFiniteDifferenceGradient() :
  FiniteDifferenceGradient()
{
  // Nothing to do
}

/* Parameter constructor */
CenteredFiniteDifferenceGradient::CenteredFiniteDifferenceGradient(const NumericalPoint & epsilon,
                                                                   const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(epsilon, p_evaluation)

{
}

/* Parameter constructor */
CenteredFiniteDifferenceGradient::CenteredFiniteDifferenceGradient(const NumericalScalar epsilon,
                                                                   const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(epsilon, p_evaluation)
{
}

/* Parameter constructor */
CenteredFiniteDifferenceGradient::CenteredFiniteDifferenceGradient(const FiniteDifferenceStep & step,
                                                                   const EvaluationImplementation & p_evaluation)
  : FiniteDifferenceGradient(step, p_evaluation)
{
  // Nothing to do
}

/* Virtual constructor */
CenteredFiniteDifferenceGradient * CenteredFiniteDifferenceGradient::clone() const
{
  return new CenteredFiniteDifferenceGradient(*this);
}


/* String converter */
String CenteredFiniteDifferenceGradient::__repr__() const
{
  OSS oss;
  oss << "class=" << CenteredFiniteDifferenceGradient::GetClassName()
      << " name=" << getName()
      << " epsilon=" << getEpsilon().__repr__()
      << " evaluation=" << p_evaluation_->__repr__();
  return oss;
}

/* String converter */
String CenteredFiniteDifferenceGradient::__str__(const String & offset) const
{
  return OSS(false) << offset << "CenteredFiniteDifferenceGradient epsilon : " << getEpsilon();
}

/* Here is the interface that all derived class must implement */

/* Gradient () */
Matrix CenteredFiniteDifferenceGradient::gradient(const NumericalPoint & inP) const
{
  const UnsignedLong inputDimension(inP.getDimension());
  NumericalPoint step(finiteDifferenceStep_.operator()(inP));
  if (inputDimension != step.getDimension()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  /* At which points do we have to compute the evaluation for the centered finite difference. We need 2*dim points. */
  NumericalSample gridPoints(2 * inputDimension, inP);
  for(UnsignedLong i = 0; i < inputDimension; ++i)
    {
      gridPoints[2 * i][i] += step[i];
      gridPoints[2 * i + 1][i] -= step[i];
    } // For i
  /* Evaluate the evaluation */
  NumericalSample gridValues(p_evaluation_->operator()(gridPoints));
  /* Compute the gradient */
  Matrix result(p_evaluation_->getInputDimension(), p_evaluation_->getOutputDimension());
  for (UnsignedLong i = 0; i < result.getNbRows(); ++i)
    for (UnsignedLong j = 0; j < result.getNbColumns(); ++j)
      /* result(i, j) = (f_j(x + e_i) - f_j(x - e_i)) / (2 * e_i) ~ df_j / dx_i */
      result(i, j) = (gridValues[2 * i][j] - gridValues[2 * i + 1][j]) / (2.0 * step[i]);
  return result;
}

END_NAMESPACE_OPENTURNS
