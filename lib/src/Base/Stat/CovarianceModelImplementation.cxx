//                                               -*- C++ -*-
/**
 *  @file  CovarianceModelImplementation.cxx
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
 *  @author schueller
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "CovarianceModelImplementation.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class CovarianceModelImplementation
 */

CLASSNAMEINIT(CovarianceModelImplementation);

static Factory<CovarianceModelImplementation> RegisteredFactory("CovarianceModelImplementation");

/* Constructor without parameters */
CovarianceModelImplementation::CovarianceModelImplementation(const String & name)
  : PersistentObject(name),
    dimension_(0)
{
  // Nothing to do
}

/* Constructor with dimension parameter */
CovarianceModelImplementation::CovarianceModelImplementation(const UnsignedLong dimension,
                                                             const String & name)
  : PersistentObject(name),
    dimension_(dimension)
{
  // Nothing to do
}

/* Virtual constructor */
CovarianceModelImplementation * CovarianceModelImplementation::clone() const
{
  return new CovarianceModelImplementation(*this);
}

/* Dimension accessor */
UnsignedLong CovarianceModelImplementation::getDimension() const
{
  return dimension_;
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModelImplementation::computeCovariance(const NumericalScalar s,
                                                                  const NumericalScalar t) const
{
  throw NotYetImplementedException(HERE);
}

/* Computation of the covariance function */
CovarianceMatrix CovarianceModelImplementation::computeCovariance(const NumericalScalar tau) const
{
  return computeCovariance(0.0, tau);
}

/* Discretize the covariance function on a given TimeGrid */
CovarianceMatrix CovarianceModelImplementation::discretizeCovariance(const RegularGrid & timeGrid) const
{
  const UnsignedLong size(timeGrid.getN());
  const UnsignedLong fullSize(size * dimension_);
  const NumericalScalar timeStep(timeGrid.getStep());
  CovarianceMatrix covarianceMatrix(fullSize);

  // Fill-in the matrix by blocks
  for (UnsignedLong rowIndex = 0; rowIndex < size; ++rowIndex)
    {
      // Only the lower part has to be filled-in
      for (UnsignedLong columnIndex = 0; columnIndex < rowIndex; ++columnIndex)
        {
          const CovarianceMatrix localCovarianceMatrix(computeCovariance( rowIndex * timeStep,  columnIndex * timeStep) );
          // We fill the covariance matrix using the previous local one
          // The full local covariance matrix has to be copied as it is
          // not copied on a symmetric position
          for (UnsignedLong rowIndexLocal = 0; rowIndexLocal < dimension_; ++rowIndexLocal)
            {
              for (UnsignedLong columnIndexLocal = 0; columnIndexLocal < dimension_; ++columnIndexLocal)
                {
                  covarianceMatrix(columnIndex + columnIndexLocal * size, rowIndex + rowIndexLocal * size ) = localCovarianceMatrix(rowIndexLocal, columnIndexLocal) ;
                } // column index within the block
            } // row index within the block
        } // column index of the block
    } // row index of the block
  return covarianceMatrix;
}

/* Is it a stationary model ? */
Bool CovarianceModelImplementation::isStationary() const
{
  return false;
}

/* String converter */
String CovarianceModelImplementation::__repr__() const
{
  OSS oss;
  oss << "class= " << CovarianceModelImplementation::GetClassName();
  return oss;
}

/* String converter */
String CovarianceModelImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << "class= " << CovarianceModelImplementation::GetClassName();
  return oss;
}

/* Method save() stores the object through the StorageManager */
void CovarianceModelImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);

  adv.saveAttribute("dimension_", dimension_);
}

/* Method load() reloads the object from the StorageManager */
void CovarianceModelImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);

  adv.loadAttribute("dimension_", dimension_);

}

END_NAMESPACE_OPENTURNS
