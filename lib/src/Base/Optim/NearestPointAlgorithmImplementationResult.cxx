//                                               -*- C++ -*-
/**
 *  @file  NearestPointAlgorithmImplementationResult.cxx
 *  @brief NearestPointAlgorithmImplementationResult stores the result of a NearestPointAlgorithmImplementationResult
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
#include "NearestPointAlgorithmImplementationResult.hxx"
#include "PersistentObjectFactory.hxx"
#include "Curve.hxx"
#include "Staircase.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NearestPointAlgorithmImplementationResult);

static Factory<NearestPointAlgorithmImplementationResult> RegisteredFactory("NearestPointAlgorithmImplementationResult");

/* Default constructor */
NearestPointAlgorithmImplementationResult::NearestPointAlgorithmImplementationResult():
  PersistentObject(),
  minimizer_(NumericalPoint(1)),
  iterationsNumber_(0),
  absoluteError_(-1.),
  relativeError_(-1.),
  residualError_(-1.),
  constraintError_(-1.)
{
  // Nothing to do
}

/* Standard constructor */
NearestPointAlgorithmImplementationResult::NearestPointAlgorithmImplementationResult(const NumericalPoint & minimizer,
                                                                                     const UnsignedLong iterationsNumber,
                                                                                     const NumericalScalar absoluteError,
                                                                                     const NumericalScalar relativeError,
                                                                                     const NumericalScalar residualError,
                                                                                     const NumericalScalar constraintError):
  PersistentObject(),
  minimizer_(minimizer),
  iterationsNumber_(iterationsNumber),
  absoluteError_(-1.),
  relativeError_(-1.),
  residualError_(-1.),
  constraintError_(-1.)
{
  // Nothing to do
}

/* Virtual constructor */
NearestPointAlgorithmImplementationResult * NearestPointAlgorithmImplementationResult::clone() const
{
  return new NearestPointAlgorithmImplementationResult(*this);
}

/* Minimizer accessors */
NumericalPoint NearestPointAlgorithmImplementationResult::getMinimizer() const
{
  return minimizer_;
}

/* Minimizer accessors */
void NearestPointAlgorithmImplementationResult::setMinimizer(const NumericalPoint & minimizer)
{
  minimizer_ = minimizer;
}

/* Iterations number accessor */
UnsignedLong NearestPointAlgorithmImplementationResult::getIterationsNumber() const
{
  return iterationsNumber_;
}

/* Iterations Number accessor */
void NearestPointAlgorithmImplementationResult::setIterationsNumber(const UnsignedLong iterationsNumber)
{
  iterationsNumber_ = iterationsNumber;
}

/* Absolute error accessor */
NumericalScalar NearestPointAlgorithmImplementationResult::getAbsoluteError() const
{
  return absoluteError_;
}

NumericalSample NearestPointAlgorithmImplementationResult::getAbsoluteErrorHistory() const
{
  return absoluteErrorHistory_;
}

/* Absolute error accessor */
void NearestPointAlgorithmImplementationResult::setAbsoluteError(const NumericalScalar absoluteError)
{
  absoluteError_ = absoluteError;
}

/* Relative error accessor */
NumericalScalar NearestPointAlgorithmImplementationResult::getRelativeError() const
{
  return relativeError_;
}

NumericalSample NearestPointAlgorithmImplementationResult::getRelativeErrorHistory() const
{
  return relativeErrorHistory_;
}

/* Relative error accessor */
void NearestPointAlgorithmImplementationResult::setRelativeError(const NumericalScalar relativeError)
{
  relativeError_ = relativeError;
}

/* Residual error accessor */
NumericalScalar NearestPointAlgorithmImplementationResult::getResidualError() const
{
  return residualError_;
}

NumericalSample NearestPointAlgorithmImplementationResult::getResidualErrorHistory() const
{
  return residualErrorHistory_;
}

/* Residual error accessor */
void NearestPointAlgorithmImplementationResult::setResidualError(const NumericalScalar residualError)
{
  residualError_ = residualError;
}

/* Constraint error accessor */
NumericalScalar NearestPointAlgorithmImplementationResult::getConstraintError() const
{
  return constraintError_;
}

NumericalSample NearestPointAlgorithmImplementationResult::getConstraintErrorHistory() const
{
  return constraintErrorHistory_;
}

/* Constraint error accessor */
void NearestPointAlgorithmImplementationResult::setConstraintError(const NumericalScalar constraintError)
{
  constraintError_ = constraintError;
}

NumericalSample NearestPointAlgorithmImplementationResult::getInputSample() const
{
  return inputSample_;
}

NumericalSample NearestPointAlgorithmImplementationResult::getOutputSample() const
{
  return outputSample_;
}

/* String converter */
String NearestPointAlgorithmImplementationResult::__repr__() const
{
  OSS oss;
  oss << "class=" << NearestPointAlgorithmImplementationResult::GetClassName()
      << " minimizer=" << minimizer_
      << " iterationsNumber=" << iterationsNumber_
      << " absoluteError=" << getAbsoluteError()
      << " relativeError=" << getRelativeError()
      << " residualError=" << getResidualError()
      << " constraintError=" << getConstraintError();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void NearestPointAlgorithmImplementationResult::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "minimizer_", minimizer_ );
  adv.saveAttribute( "iterationsNumber_", iterationsNumber_ );
  adv.saveAttribute( "absoluteError_", absoluteError_ );
  adv.saveAttribute( "relativeError_", relativeError_ );
  adv.saveAttribute( "residualError_", residualError_ );
  adv.saveAttribute( "constraintError_", constraintError_ );

  adv.saveAttribute( "absoluteErrorHistory_", absoluteErrorHistory_ );
  adv.saveAttribute( "relativeErrorHistory_", relativeErrorHistory_ );
  adv.saveAttribute( "residualErrorHistory_", residualErrorHistory_ );
  adv.saveAttribute( "constraintErrorHistory_", constraintErrorHistory_ );

  adv.saveAttribute( "inputSample_", inputSample_ );
  adv.saveAttribute( "outputSample_", outputSample_ );
}

/* Method load() reloads the object from the StorageManager */
void NearestPointAlgorithmImplementationResult::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "minimizer_", minimizer_ );
  adv.loadAttribute( "iterationsNumber_", iterationsNumber_ );
  adv.loadAttribute( "absoluteError_", absoluteError_ );
  adv.loadAttribute( "relativeError_", relativeError_ );
  adv.loadAttribute( "residualError_", residualError_ );
  adv.loadAttribute( "constraintError_", constraintError_ );

  adv.loadAttribute( "absoluteErrorHistory_", absoluteErrorHistory_ );
  adv.loadAttribute( "relativeErrorHistory_", relativeErrorHistory_ );
  adv.loadAttribute( "residualErrorHistory_", residualErrorHistory_ );
  adv.loadAttribute( "constraintErrorHistory_", constraintErrorHistory_ );

  adv.loadAttribute( "inputSampleHistory_", inputSample_ );
  adv.loadAttribute( "outputSampleHistory_", outputSample_ );
}

/* Update current state */
void NearestPointAlgorithmImplementationResult::update(const NumericalPoint & minimizer, UnsignedLong iterationNumber)
{
  setMinimizer( minimizer );
  setIterationsNumber( iterationNumber );
}

/* Incremental history storage */
void NearestPointAlgorithmImplementationResult::store(const NumericalPoint& x,
                                                      const NumericalPoint& y,
                                                      const NumericalScalar absoluteError,
                                                      const NumericalScalar relativeError,
                                                      const NumericalScalar residualError,
                                                      const NumericalScalar constraintError)
{
  // assume the last point stored is the optimum
  minimizer_ = x;

  // update values
  absoluteError_ = absoluteError;
  relativeError_ = relativeError;
  residualError_ = residualError;
  constraintError_ = constraintError;

  // append values
  absoluteErrorHistory_.add(NumericalPoint(1, absoluteError));
  relativeErrorHistory_.add(NumericalPoint(1, relativeError));
  residualErrorHistory_.add(NumericalPoint(1, residualError));
  constraintErrorHistory_.add(NumericalPoint(1, constraintError));

  // initialize sample dimension first
  if ( inputSample_.getSize() == 0 )
    {
      inputSample_ = NumericalSample(0, x.getDimension());
    }
  if ( outputSample_.getSize() == 0 )
    {
      outputSample_ = NumericalSample(0, y.getDimension());
    }
  inputSample_.add(x);
  outputSample_.add(y);
}

Graph NearestPointAlgorithmImplementationResult::getErrorHistory() const
{
  Graph result;
  result.setTitle("Error history");
  result.setXTitle("Iteration number");
  result.setYTitle("Error value");
  result.setLegendPosition("topright");

  // create a sample with the iterations number to be plotted as x data
  UnsignedLong size = getAbsoluteErrorHistory().getSize();
  NumericalSample iterationIndex(size, 1);
  for ( UnsignedLong i = 0; i < size; ++ i )
    {
      iterationIndex[i][0] = i + 1;
    }

  Curve absoluteErrorCurve( iterationIndex, getAbsoluteErrorHistory(), "absolute error" );
  absoluteErrorCurve.setLegendName("absolute error");
  absoluteErrorCurve.setColor("red");
  result.add( absoluteErrorCurve );

  Curve relativeErrorCurve( iterationIndex, getRelativeErrorHistory(), "relative error" );
  result.add( relativeErrorCurve );

  Curve residualErrorCurve( iterationIndex, getResidualErrorHistory(), "residual error" );
  residualErrorCurve.setColor("green");
  result.add( residualErrorCurve );

  Curve constraintErrorCurve( iterationIndex, getConstraintErrorHistory(), "constraint error" );
  constraintErrorCurve.setColor("yellow");
  result.add(constraintErrorCurve );

  return result;
}


END_NAMESPACE_OPENTURNS
