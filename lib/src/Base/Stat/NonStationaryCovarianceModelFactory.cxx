//                                               -*- C++ -*-
/**
 *  @file  NonStationaryCovarianceModelFactory.cxx
 *  @brief Top-level class for all spectral model factories
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
 *  @author: $LastChangedBy$
 *  @author: $LastChangedBy$
 *  @date:   $LastChangedDate$
 *  Id:      $Id$
 */
#include "NonStationaryCovarianceModelFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"
#include "CovarianceMatrix.hxx"
#include "NumericalPoint.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(NonStationaryCovarianceModelFactory);
static Factory<NonStationaryCovarianceModelFactory> RegisteredFactory("NonStationaryCovarianceModelFactory");

typedef Collection<CovarianceMatrix>  CovarianceMatrixCollection;

/* Default constructor */
NonStationaryCovarianceModelFactory::NonStationaryCovarianceModelFactory(const String & name)
  : CovarianceModelFactoryImplementation(name)
{
  // Nothing to do
}

/* Virtual constructor */
NonStationaryCovarianceModelFactory * NonStationaryCovarianceModelFactory::clone() const
{
  return new NonStationaryCovarianceModelFactory(*this);
}

/* String converter */
String NonStationaryCovarianceModelFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << NonStationaryCovarianceModelFactory::GetClassName();
  return oss;
}

/* String converter */
String NonStationaryCovarianceModelFactory::__str__(const String & offset) const
{
  return __repr__();
}


UserDefinedCovarianceModel * NonStationaryCovarianceModelFactory::build(const ProcessSample & sample) const
{
  // Get the time grid
  const RegularGrid timeGrid(sample.getTimeGrid());
  const UnsignedLong sampleSize(sample.getSize());
  const UnsignedLong N(timeGrid.getN());
  const UnsignedLong dimension(sample.getDimension());

  // Create a collection of null CovarianceMatrix
  UnsignedLong size(static_cast<UnsignedLong>((N * (N + 1) / 2)));
  CovarianceMatrixCollection collection(size);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      collection[i] = CovarianceMatrix(SquareMatrix(dimension).getImplementation());
    }

  // Special case for a sample of size 1
  if (size == 1) return UserDefinedCovarianceModel(timeGrid, collection).clone();

  const TimeSeries mean(sample.computeMean());
  const NumericalScalar alpha(1.0 / (sampleSize - 1));
  UnsignedLong index(0);
  for (UnsignedLong i = 0; i < N; ++i)
    {
      for (UnsignedLong j = i; j < N; ++j)
        {
          CovarianceMatrix & matrix = collection[index];
          for (UnsignedLong k = 0; k < dimension; ++k)
            {
              for (UnsignedLong l = 0; l <= k; ++l)
                {
                  NumericalScalar coef(0.0);
                  for (UnsignedLong sampleIndex = 0; sampleIndex < sampleSize; ++sampleIndex)
                    {
                      coef += (sample[sampleIndex].getValueAtIndex(i)[k] - mean.getValueAtIndex(i)[k])
                        * (sample[sampleIndex].getValueAtIndex(j)[l] - mean.getValueAtIndex(j)[l]);
                    }
                  matrix(k, l) = coef * alpha;
                }
            }
          index += 1;
        }
    }

  return UserDefinedCovarianceModel(timeGrid, collection).clone();
}

UserDefinedCovarianceModel * NonStationaryCovarianceModelFactory::build(const TimeSeries & timeSeries) const
{
  return UserDefinedCovarianceModel().clone();
}

/* Method save() stores the object through the StorageManager */
void NonStationaryCovarianceModelFactory::save(Advocate & adv) const
{
  CovarianceModelFactoryImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void NonStationaryCovarianceModelFactory::load(Advocate & adv)
{
  CovarianceModelFactoryImplementation::load(adv);
}
END_NAMESPACE_OPENTURNS
