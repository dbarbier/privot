//                                               -*- C++ -*-
/**
 *  @file  QuasiMonteCarloResult.cxx
 *  @brief Implementation of SimulationResult
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
 *  @date   2008-05-23 13:46:12 +0200 (ven, 23 mai 2008)
 */
#include "QuasiMonteCarloResult.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS


CLASSNAMEINIT(QuasiMonteCarloResult);


static Factory<QuasiMonteCarloResult> RegisteredFactory("QuasiMonteCarloResult");

/* Default constructor */
QuasiMonteCarloResult::QuasiMonteCarloResult()
  : SimulationResultImplementation()
{
  // Nothing to do
}

/* Standard constructor */
QuasiMonteCarloResult::QuasiMonteCarloResult(const Event & event,
                                             const NumericalScalar probabilityEstimate,
                                             const NumericalScalar varianceEstimate,
                                             const UnsignedLong outerSampling,
                                             const UnsignedLong blockSize)
  : SimulationResultImplementation(event, probabilityEstimate, varianceEstimate, outerSampling, blockSize)
{
  // Nothing to do
}

/* Virtual constructor */
QuasiMonteCarloResult * QuasiMonteCarloResult::clone() const
{
  return new QuasiMonteCarloResult(*this);
}

/* Coefficient of variation estimate accessor */
NumericalScalar QuasiMonteCarloResult::getCoefficientOfVariation() const
{
  return -1.0;
}

/* Standard deviation estimate accessor */
NumericalScalar QuasiMonteCarloResult::getStandardDeviation() const
{
  return -1.0;
}

/* Confidence length */
NumericalScalar QuasiMonteCarloResult::getConfidenceLength(const NumericalScalar level) const
{
  throw NotYetImplementedException(HERE) << "cannot compute confidence interval for QMC sampling";
}

/* String converter */
String QuasiMonteCarloResult::__repr__() const
{
  OSS oss;
  oss.setPrecision(6);
  oss << std::scientific
      << "probabilityEstimate=" << probabilityEstimate_
      << " varianceEstimate=" << varianceEstimate_
      << " outerSampling=" << outerSampling_
      << " blockSize=" << blockSize_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void QuasiMonteCarloResult::save(Advocate & adv) const
{
  SimulationResultImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void QuasiMonteCarloResult::load(Advocate & adv)
{
  SimulationResultImplementation::load(adv);
}



END_NAMESPACE_OPENTURNS
