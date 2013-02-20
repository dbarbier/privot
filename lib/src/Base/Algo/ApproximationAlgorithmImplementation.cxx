//                                               -*- C++ -*-
/**
 *  @file  ApproximationAlgorithmImplementation.cxx
 *  @brief Regression from a data sample upon a particular basis
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "PersistentObjectFactory.hxx"
#include "ApproximationAlgorithmImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ApproximationAlgorithmImplementation);


/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation()
  : PersistentObject(),
    isAlreadyComputedCoefficients_(false),
    residual_(0.0),
    relativeError_(0.0),
    verbose_(false)
{
  // Nothing to do
}

/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation(const NumericalSample & x,
                                                                           const NumericalSample & y,
                                                                           const Basis & psi)
  : PersistentObject(),
    x_(x),
    y_(y),
    weight_(NumericalPoint( x.getSize(), 1.0 )),
    psi_(psi),
    isAlreadyComputedCoefficients_(false),
    residual_(0.0),
    relativeError_(0.0),
    verbose_(false)
{
  const UnsignedLong dataSize(x.getSize());
  if (dataSize == 0) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on an empty sample.";
  if (dataSize != y.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on samples of different size.";
  if (y.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on output sample of dimension different from 1.";
  const UnsignedLong basisSize(psi.getSize());
  for (UnsignedLong i = 0; i < basisSize; ++i)
    if (psi[i].getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can only use basis functions with 1D values.";
}

/* Default constructor */
ApproximationAlgorithmImplementation::ApproximationAlgorithmImplementation(const NumericalSample & x,
                                                                           const NumericalSample & y,
                                                                           const NumericalPoint & weight,
                                                                           const Basis & psi)
  : PersistentObject(),
    x_(x),
    y_(y),
    weight_(weight),
    psi_(psi),
    isAlreadyComputedCoefficients_(false),
    residual_(0.0),
    relativeError_(0.0),
    verbose_(false)
{
  UnsignedLong dataSize( x.getSize() );
  if (dataSize == 0) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on an empty sample.";
  if (dataSize != y.getSize()) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on samples of different size.";
  if (y.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation based on output sample of dimension different from 1.";
  const UnsignedLong basisSize(psi.getSize());
  for (UnsignedLong i = 0; i < basisSize; ++i)
    if (psi[i].getOutputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can only use basis functions with 1D values.";
  if ( dataSize != weight.getDimension() ) throw InvalidArgumentException(HERE) << "Error: cannot perform approximation with weight of improper dimension.";
  for ( UnsignedLong i = 0; i < dataSize; ++ i )
    if ( weight[i] < 0.0 ) throw InvalidArgumentException(HERE) << "Error: can only use positive weight.";
}

/* Accessors */
NumericalSample ApproximationAlgorithmImplementation::getX() const
{
  return x_;
}

NumericalSample ApproximationAlgorithmImplementation::getY() const
{
  return y_;
}

NumericalPoint ApproximationAlgorithmImplementation::getWeight() const
{
  return weight_;
}

Basis ApproximationAlgorithmImplementation::getPsi() const
{
  return psi_;
}


/* String converter */
String ApproximationAlgorithmImplementation::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " x=" << x_
               << " y=" << y_
               << " weight=" << weight_
               << " psi=" << psi_
               << " verbose=" << verbose_ ;
}


String ApproximationAlgorithmImplementation::__str__(const String & offset) const
{
  return __repr__();
}


void ApproximationAlgorithmImplementation::setCoefficients(const NumericalPoint & coefficients)
{
  coefficients_ = coefficients;
  isAlreadyComputedCoefficients_ = true;
}


NumericalPoint ApproximationAlgorithmImplementation::getCoefficients()
{
  if ( ! isAlreadyComputedCoefficients_ ) run();
  return coefficients_;
}

void ApproximationAlgorithmImplementation::setResidual(const NumericalScalar residual)
{
  residual_ = residual;
}

NumericalScalar ApproximationAlgorithmImplementation::getResidual()
{
  if ( ! isAlreadyComputedCoefficients_ ) run();
  return residual_;
}

void ApproximationAlgorithmImplementation::setRelativeError(const NumericalScalar relativeError)
{
  relativeError_ = relativeError;
}

NumericalScalar ApproximationAlgorithmImplementation::getRelativeError()
{
  if ( ! isAlreadyComputedCoefficients_ ) run();
  return relativeError_;
}

/* Verbosity accessor */
void ApproximationAlgorithmImplementation::setVerbose(const Bool verbose)
{
  verbose_ = verbose;
}

Bool ApproximationAlgorithmImplementation::getVerbose() const
{
  return verbose_;
}

/* Method save() stores the object through the StorageManager */
void ApproximationAlgorithmImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "x_", x_ );
  adv.saveAttribute( "y_", y_ );
  adv.saveAttribute( "weight_", weight_ );
  adv.saveAttribute( "psi_", psi_ );
  adv.saveAttribute( "verbose_", verbose_ );
}


/* Method load() reloads the object from the StorageManager */
void ApproximationAlgorithmImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "x_", x_ );
  adv.loadAttribute( "y_", y_ );
  adv.loadAttribute( "weight_", weight_ );
  adv.loadAttribute( "psi_", psi_ );
  adv.loadAttribute( "verbose_", verbose_ );
  isAlreadyComputedCoefficients_ = false;
  residual_ = 0.0;
  relativeError_ = 0.0;
}


END_NAMESPACE_OPENTURNS
