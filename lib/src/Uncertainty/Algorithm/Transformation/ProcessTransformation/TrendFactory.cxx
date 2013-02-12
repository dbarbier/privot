//                                               -*- C++ -*-
/**
 *  @file  TrendFactory.cxx
 *  @brief
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
 *  @author haddad
 *  @date   2011-12-19 14:30:01 +0100 (lun. 19 déc. 2011)
 */
#include "TrendFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "LeastSquaresMetaModelSelection.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class TrendFactory
 */

CLASSNAMEINIT(TrendFactory);

// static Factory<TrendFactory> RegisteredFactory("TrendFactory");

/* Constructor with parameters */
TrendFactory::TrendFactory(const BasisSequenceFactory & basisSequenceFactory,
                           const FittingAlgorithm & fittingAlgorithm,
                           const String & name)
  : PersistentObject(name),
    basisSequenceFactory_(basisSequenceFactory),
    fittingAlgorithm_(fittingAlgorithm)
{
  // Nothing to do
}

/* Virtual constructor */
TrendFactory * TrendFactory::clone() const
{
  return new TrendFactory(*this);
}

/* BasisSequenceFactory get accessor */
BasisSequenceFactory TrendFactory::getBasisSequenceFactory() const
{
  return basisSequenceFactory_;
}

/* BasisSequenceFactory set accessor */
void TrendFactory::setBasisSequenceFactory(const BasisSequenceFactory & basisSequenceFactory)
{
  basisSequenceFactory_ = basisSequenceFactory;
}

/* FittingAlgorithm get accessor */
FittingAlgorithm TrendFactory::getFittingAlgorithm() const
{
  return fittingAlgorithm_;
}

/* FittingAlgorithm set accessor */
void TrendFactory::setFittingAlgorithm(const FittingAlgorithm & fittingAlgorithm)
{
  fittingAlgorithm_ = fittingAlgorithm;
}

/* Build the factory from data */
TrendTransform TrendFactory::build(const TimeSeries & timeSeries,
				   const Basis & basis) const
{
  // size of the collection
  const UnsignedLong N(basis.getSize());

  // Dimension and size of the time series
  const UnsignedLong dimension(timeSeries.getDimension());
  NumericalSample inputSample(timeSeries.getSize(), 1);

  // We get the time grid parameters
  const RegularGrid timeGrid(timeSeries.getTimeGrid());

  // We fill the inputSample with time values as we are looking for trend
  for (UnsignedLong k = 0; k < timeGrid.getN(); ++k)
    {
      inputSample[k][0] =  timeGrid.getValue(k);
    }

  // We need a sample to get coefficients result
  NumericalSample coefficients(N, dimension);

  for (UnsignedLong d = 0; d < dimension; ++d)
    {
      // We look for best coefficients by marginal
      const NumericalSample outputSample (timeSeries.getSample().getMarginal(d));
      LeastSquaresMetaModelSelection selectionAlgo(inputSample, outputSample, basis, basisSequenceFactory_, fittingAlgorithm_);
      selectionAlgo.run();
      NumericalPoint localCoefficients(selectionAlgo.getCoefficients());
      for (UnsignedLong k = 0; k < N ; ++k)
        {
          coefficients[k][d] =  localCoefficients[k];
        }
    }
  const NumericalMathFunction trendFunction(basis, coefficients);

  return TrendTransform(trendFunction);
}

/* String converter */
String TrendFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << TrendFactory::GetClassName()
      << " fittingAlgorithm = " << fittingAlgorithm_
      << " basisSequenceFactory = " << basisSequenceFactory_;
  return oss;
}

/* String converter */
String TrendFactory::__str__(const String & offset) const
{
  return __repr__();
}


/* Method save() stores the object through the StorageManager */
void TrendFactory::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("fittingAlgorithm_", fittingAlgorithm_);
  adv.saveAttribute("basisSequenceFactory_", basisSequenceFactory_);
}

/* Method load() reloads the object from the StorageManager */
void TrendFactory::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("fittingAlgorithm_", fittingAlgorithm_);
  adv.loadAttribute("basisSequenceFactory_", basisSequenceFactory_);
}

END_NAMESPACE_OPENTURNS
