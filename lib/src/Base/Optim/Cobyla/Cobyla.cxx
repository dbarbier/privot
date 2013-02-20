//                                               -*- C++ -*-
/**
 *  @file  Cobyla.cxx
 *  @brief Cobyla is an actual implementation for
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
#include "Cobyla.hxx"
#include "algocobyla.h"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(Cobyla);

/* Default constructor */
Cobyla::Cobyla():
  NearestPointAlgorithmImplementation()
{
  // Nothing to do
}

/*
 * @brief  Standard constructor: the problem is defined by a scalar valued function  (in fact, a 1-D vector valued function)
 *         and a level value
 */
Cobyla::Cobyla(const NumericalMathFunction & levelFunction,
               const Bool verbose):
  NearestPointAlgorithmImplementation(levelFunction, verbose),
  specificParameters_()
{
  // Nothing to do
}

/*
 * @brief  Standard constructor: the problem is defined by a scalar valued function  (in vact, a 1-D vector valued fnction)
 *         and a level value
 */
Cobyla::Cobyla(const CobylaSpecificParameters & specificParameters,
               const NumericalMathFunction & levelFunction,
               const Bool verbose):
  NearestPointAlgorithmImplementation(levelFunction, verbose),
  specificParameters_(specificParameters)
{
  // Nothing to do
}

/* Virtual constructor */
Cobyla * Cobyla::clone() const
{
  return new Cobyla(*this);
}

/* Performs the actual computation by calling the Cobyla algorithm
 */
void Cobyla::run()
{
  int n(getStartingPoint().getDimension());
  int m(2);
  NumericalPoint x(getStartingPoint());
  NumericalScalar rhoBeg(specificParameters_.getRhoBeg());
  NumericalScalar rhoEnd(getMaximumAbsoluteError());
  int maxFun(getMaximumIterationsNumber() * x.getDimension());
  cobyla_message message((getVerbose() ? COBYLA_MSG_INFO : COBYLA_MSG_NONE));

  NumericalScalar absoluteError = -1.0;
  NumericalScalar relativeError = -1.0;
  NumericalScalar residualError = -1.0;
  NumericalScalar constraintError = -1.0;

  // clear result
  setResult(NearestPointAlgorithmImplementationResult(x, 0, absoluteError, relativeError, residualError, constraintError));

  // enable history
  Bool historyEnabled = getLevelFunction().isHistoryEnabled();
  getLevelFunction().enableHistory();
  UnsignedLong historyOffset = getLevelFunction().getInputHistory().getSample().getSize();

  /*
   * cobyla : minimize a function subject to constraints
   *
   * n         : number of variables (>=0)
   * m         : number of constraints (>=0)
   * x         : on input, initial estimate ; on output, the solution
   * rhobeg    : a reasonable initial change to the variables
   * rhoend    : the required accuracy for the variables
   * message   : see the cobyla_message enum
   * maxfun    : on input, the maximum number of function evaluations
   *             on output, the number of function evaluations done
   * calcfc    : the function to minimize (see cobyla_function)
   * state     : used by function (see cobyla_function)
   *
   * The cobyla function returns a code defined in the cobyla_rc enum.
   *
   * extern int cobyla(int n, int m, double *x, double rhobeg, double rhoend,
   *  int message, int *maxfun, cobyla_function *calcfc, void *state);
   */
  int returnCode = cobyla(n, m, &x[0], rhoBeg, rhoEnd, message, &maxFun, Cobyla::ComputeObjectiveAndConstraint, (void*) this);

  // Update the result
  result_.update(x, maxFun / x.getDimension());

  NumericalScalar levelValue = getLevelValue();
  UnsignedLong size = getLevelFunction().getInputHistory().getSample().getSize();

  NumericalSample historyInputSample( getLevelFunction().getInputHistory().getSample() );
  NumericalSample historyOutputSample( getLevelFunction().getInputHistory().getSample() );

  for ( UnsignedLong i = historyOffset + 1; i < size; ++ i )
    {
      NumericalPoint inPM( historyInputSample[i - 1] );
      NumericalPoint inP( historyInputSample[i] );
      NumericalPoint outP( historyOutputSample[i] );
      NumericalPoint outPM( historyOutputSample[i - 1] );
      absoluteError = (inP - inPM).norm();
      relativeError = absoluteError / inP.norm();
      residualError = (outP - outPM).norm();
      constraintError = fabs( outP[0] - levelValue );
      result_.store( inP, outP, absoluteError, relativeError, residualError, constraintError );
    }

  // check the convergence criteria
  Bool convergence = ((absoluteError < getMaximumAbsoluteError()) && (relativeError < getMaximumRelativeError())) || ((residualError < getMaximumResidualError()) && (constraintError < getMaximumConstraintError()));

  if (returnCode != 0)
    {
      LOGWARN(OSS() << "Warning! The Cobyla algorithm failed to converge. The error message is " << cobyla_rc_string[returnCode - COBYLA_MINRC]);
    }
  else if ( ! convergence )
    {
      LOGWARN(OSS() << "Warning! The Cobyla algorithm could not enforce the convergence criteria");
    }

  // revert history state
  if ( ! historyEnabled )
    getLevelFunction().disableHistory();
}

/* Specific parameters accessor */
CobylaSpecificParameters Cobyla::getSpecificParameters() const
{
  return specificParameters_;
}

/* Specific parameters accessor */
void Cobyla::setSpecificParameters(const CobylaSpecificParameters & specificParameters)
{
  specificParameters_ = specificParameters;
}

/* String converter */
String Cobyla::__repr__() const
{
  OSS oss;
  oss << "class=" << Cobyla::GetClassName()
      << " " << NearestPointAlgorithmImplementation::__repr__()
      << " specificParameters=" << getSpecificParameters();
  return oss;
}

/*
 * Wrapper of the NumericalMathFunction operator() compatible with
 * cobyla signature
 */
int Cobyla::ComputeObjectiveAndConstraint(int n,
                                          int m,
                                          double *x,
                                          double *f,
                                          double *con,
                                          void *state)
{
  NearestPointAlgorithmImplementation *algorithm = static_cast<NearestPointAlgorithmImplementation *>(state);

  /* Convert the input vector in OpenTURNS format */
  NumericalPoint inPoint((UnsignedLong)n);
  for(UnsignedLong index = 0; index < (UnsignedLong)(n); ++index) inPoint[index] = x[index];
  /* Compute the level function at inPoint */
  *f = 0.5 * inPoint.norm2();
  /* Compute the constraints at inPoint */
  NumericalPoint constraintValue;
  try
    {
      constraintValue = algorithm->getLevelFunction().operator()(inPoint);
    }
  catch (InternalException & ex)
    {
      return 1;
    }
  catch (InvalidArgumentException & ex)
    {
      return 1;
    }

  con[0] = constraintValue[0] + algorithm->getMaximumConstraintError() - algorithm->getLevelValue();
  con[1] = algorithm->getLevelValue() + algorithm->getMaximumConstraintError() - constraintValue[0];

  return 0;
}

END_NAMESPACE_OPENTURNS
