//                                               -*- C++ -*-
/**
 *  @file  SymmetricTensor.cxx
 *  @brief SymmetricTensor implements the classical mathematical symmetric tensor
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "SymmetricTensor.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(SymmetricTensor);

/* Default constructor */
SymmetricTensor::SymmetricTensor()
  : Tensor(),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}

/* Constructor with size (squareDim and sheetDim) */
/* The SymmetricTensor is made up of a collection of squareDim*squareDim*sheetDim elements */
/* The SymmetricTensor is viewed as a set of column vectors read one after another, one sheet after another */
SymmetricTensor::SymmetricTensor(const UnsignedLong squareDim,
                                 const UnsignedLong sheetDim)
  : Tensor(squareDim, squareDim, sheetDim),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}

/* Constructor from external collection */
SymmetricTensor::SymmetricTensor(const UnsignedLong squareDim,
                                 const UnsignedLong sheetDim,
                                 const Collection<NumericalScalar> & elementsValues)
  : Tensor(squareDim, squareDim, sheetDim, elementsValues),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}

/* Constructor with implementation */
SymmetricTensor::SymmetricTensor(const Implementation & i)
  : Tensor(i),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}


/* String converter */
String SymmetricTensor::__repr__() const
{
  return Tensor::__repr__();
}

/* String converter */
String SymmetricTensor::__str__(const String & offset) const
{
  checkSymmetry();
  return Tensor::__str__( offset );
}

/* Check if the internal representation is really symmetric */
void SymmetricTensor::checkSymmetry() const
{
  if (!hasBeenSymmetrized_)
    {
      getImplementation()->symmetrize();
      hasBeenSymmetrized_ = true;
    }
}

/* Operator () gives access to the elements of the symmetric tensor (to modify these elements and their symmetric element) */
/* The element of the symmetric tensor is designated by its row number i, its column number j and its sheet number k*/
NumericalScalar & SymmetricTensor::operator () (const UnsignedLong i,
                                                const UnsignedLong j,
                                                const UnsignedLong k)
{
  copyOnWrite();
  hasBeenSymmetrized_ = false;
  return ( (i > j) ? (*getImplementation())(i, j, k) : (*getImplementation())(j, i, k) );
}

/* Operator () gives access to the elements of the tensor (read only) */
/* The element of the tensor is designated by its row number i and its column number j */
const NumericalScalar & SymmetricTensor::operator () (const UnsignedLong i,
                                                      const UnsignedLong j,
                                                      const UnsignedLong k) const
{
  return ( (i > j) ? (*getImplementation())(i, j, k) : (*getImplementation())(j, i, k) );
}

/* getSheet returns the sheet specified by its sheet number k */
SymmetricMatrix SymmetricTensor::getSheet(const UnsignedLong k) const
{
  return getImplementation()->getSheetSym(k);
}

/* setSheet sets matrix m as the sheet specified by its sheet number k  */
void SymmetricTensor::setSheet(const UnsignedLong k,
                               const SymmetricMatrix & m)
{
  hasBeenSymmetrized_ = false;
  getImplementation()->setSheetSym(k, m);
}

END_NAMESPACE_OPENTURNS
