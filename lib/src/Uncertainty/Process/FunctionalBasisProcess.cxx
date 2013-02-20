//                                               -*- C++ -*-
/**
 *  @file  FunctionalBasisProcess.cxx
 *  @brief A class which implements the FunctionalBasisProcess process
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
 *  @date   2012-04-18 17:56:46 +0200 (mer. 18 avril 2012)
 */

#include "FunctionalBasisProcess.hxx"
#include "PersistentObjectFactory.hxx"
#include "Normal.hxx"
#include "ResourceMap.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(FunctionalBasisProcess);

static Factory<FunctionalBasisProcess> RegisteredFactory("FunctionalBasisProcess");

/* Standard constructor */
FunctionalBasisProcess::FunctionalBasisProcess(const String & name)
  : ProcessImplementation(name),
    distribution_(Normal()),
    basis_(Collection<NumericalMathFunction>(1, NumericalMathFunction("t", "t"))),
    state_(1, 0.0)
{
  // Set the dimension of the process
  setDimension(1);
}

/* Standard constructor */
FunctionalBasisProcess::FunctionalBasisProcess(const Distribution & distribution,
                                               const Basis & basis,
                                               const String & name)
  /* throw (InvalidArgumentException) */
  : ProcessImplementation(name),
    distribution_(distribution),
    basis_(),
    state_(distribution.getDimension(), 0.0)
{
  // Check the given basis against the distribution
  setBasis(basis);
}

/* Standard constructor */
FunctionalBasisProcess::FunctionalBasisProcess(const Distribution & distribution,
                                               const Basis & basis,
                                               const RegularGrid & timeGrid,
                                               const String & name)
  : ProcessImplementation(name),
    distribution_(distribution),
    basis_(),
    state_(distribution.getDimension(), 0.0)
{
  // Check the given basis against the distribution
  setBasis(basis);
  setTimeGrid(timeGrid);
}

/* Virtual constructor */
FunctionalBasisProcess * FunctionalBasisProcess::clone() const
{
  return new FunctionalBasisProcess(*this);
}

/* String converter */
String FunctionalBasisProcess::__repr__() const
{
  OSS oss;
  oss << "class=" << FunctionalBasisProcess::GetClassName()
      << " distribution=" << distribution_
      << " basis=" << basis_;
  return oss;
}

/* Is the underlying stationary ? */
Bool FunctionalBasisProcess::isStationary() const
{
  return false;
}

/* Is the underlying gaussian ? */
Bool FunctionalBasisProcess::isNormal() const
{
  return (distribution_.getImplementation()->getClassName() == "Normal");
}

/* Realization accessor */
TimeSeries FunctionalBasisProcess::getRealization() const
{
  state_ = distribution_.getRealization();
  const UnsignedLong timeGridSize(timeGrid_.getN());
  const UnsignedLong basisSize(basis_.getSize());
  // Loop over the time stamps
  NumericalSample result(timeGridSize, NumericalPoint(dimension_, 0.0));
  for (UnsignedLong i = 0; i  < timeGridSize; ++i)
    {
      const NumericalPoint t(1, timeGrid_.getValue(i));
      // Loop over the basis
      for (UnsignedLong j = 0; j < basisSize; ++j) result[i] += basis_[j](t) * state_[j];
    }
  return TimeSeries(timeGrid_, result);
}

/* Compute the next steps of a random walk */
TimeSeries FunctionalBasisProcess::getFuture(const UnsignedLong stepNumber) const
{
  if (stepNumber == 0) return TimeSeries(0, dimension_);
  /* TimeGrid associated with the possible future */
  const NumericalScalar timeStep(timeGrid_.getStep());
  const RegularGrid futureTimeGrid(timeGrid_.getEnd(), timeStep, stepNumber);
  const UnsignedLong basisSize(basis_.getSize());
  // Loop over the time stamps
  NumericalSample result(stepNumber, NumericalPoint(dimension_, 0.0));
  for (UnsignedLong i = 0; i  < stepNumber; ++i)
    {
      const NumericalPoint t(1, futureTimeGrid.getValue(i));
      // Loop over the basis using the previous state
      for (UnsignedLong j = 0; j < basisSize; ++j) result[i] += basis_[j](t) * state_[j];
    }
  return TimeSeries(futureTimeGrid, result);
}

/* Get the marginal process corresponding to the i-th marginal component */
FunctionalBasisProcess::Implementation FunctionalBasisProcess::getMarginalProcess(const UnsignedLong i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal process must be in the range [0, dim-1]";
  return getMarginalProcess(Indices(1, i));
}

/* Get the marginal random vector corresponding to indices components */
FunctionalBasisProcess::Implementation FunctionalBasisProcess::getMarginalProcess(const Indices & indices) const
{
  if (!indices.check(getDimension() - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal process must be in the range [0, dim-1] and  must be different";
  // First the marginal distribution
  Distribution marginalDistribution(distribution_.getMarginal(indices));
  // Second the marginal basis
  const UnsignedLong basisSize(basis_.getSize());
  Basis marginalBasis(basisSize);
  for (UnsignedLong i = 0; i < basisSize; ++i) marginalBasis[i] = basis_[i].getMarginal(indices);
  // Return the associated FunctionalBasisProcess
  return FunctionalBasisProcess(marginalDistribution, marginalBasis, timeGrid_).clone();
}

/* Distribution accessor */
Distribution FunctionalBasisProcess::getDistribution() const
{
  return distribution_;
}

/* Distribution accessor */
void FunctionalBasisProcess::setDistribution(const Distribution & distribution)
{
  // Check the distribution against the basis
  if (distribution.getDimension() != basis_.getSize()) throw InvalidArgumentException(HERE) << "Error: the given distribution has a dimension=" << distribution.getDimension() << " that does not match the basis size=" << basis_.getSize();
  distribution_ = distribution;
}

/* Basis accessor */
Basis FunctionalBasisProcess::getBasis() const
{
  return basis_;
}

/* Basis accessor */
void FunctionalBasisProcess::setBasis(const Basis & basis)
{
  const UnsignedLong size(basis.getSize());
  // Check if the basis is not empty
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: the given basis is empty.";
  // Check the basis against the distribution
  if (size != distribution_.getDimension() ) throw InvalidArgumentException(HERE) << "Error: the given basis has a size=" << size << " that does not match the distribution dimension=" << distribution_.getDimension();
  // Check if the functions in the basis are from R to R^n for the same n
  dimension_ = basis[0].getOutputDimension();
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // Check the input dimension
      if (basis[i].getInputDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the function at index=" << i << " has an input dimension=" << basis[i].getInputDimension() << " which is not equal to 1.";
      // Check the output dimension
      if (basis[i].getOutputDimension() != dimension_) throw InvalidArgumentException(HERE) << "Error: the function at index=" << i << " has an output dimension=" << basis[i].getOutputDimension() << " which is not equal to the process dimension=" << dimension_;
    }
  basis_ = basis;
}

/* Method save() stores the object through the StorageManager */
void FunctionalBasisProcess::save(Advocate & adv) const
{
  ProcessImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
  adv.saveAttribute( "basis_", basis_ );
  adv.saveAttribute( "state_", state_ );
}

/* Method load() reloads the object from the StorageManager */
void FunctionalBasisProcess::load(Advocate & adv)
{
  ProcessImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
  adv.loadAttribute( "basis_", basis_ );
  adv.loadAttribute( "state_", state_ );
}
END_NAMESPACE_OPENTURNS
