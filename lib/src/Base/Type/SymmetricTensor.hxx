//                                               -*- C++ -*-
/**
 *  @file  SymmetricTensor.hxx
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 *  Id      SymmetricTensor.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_SYMMETRICTENSOR_HXX
#define OPENTURNS_SYMMETRICTENSOR_HXX

#include "Tensor.hxx"
#include "SymmetricMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class SymmetricTensor
 *
 * SymmetricTensor implements the classical mathematical symmetric tensor
 */

class SymmetricTensor :
  public Tensor

{
  CLASSNAME;

public:
  typedef Collection<NumericalScalar>       NumericalScalarCollection;


  /** Default constructor */
  SymmetricTensor();

  /** Constructor with size (squareDim, sheetDim) */
  SymmetricTensor(const UnsignedLong squareDim,
                  const UnsignedLong sheetDim);

  /** Constructor from a range of external collection */
  template <class InputIterator>
  SymmetricTensor(const UnsignedLong squareDim,
                  const UnsignedLong sheetDim,
                  InputIterator first,
                  InputIterator last);

  /** Constructor from external collection */
  SymmetricTensor(const UnsignedLong squareDim,
                  const UnsignedLong sheetDim,
                  const NumericalScalarCollection & elementsValues);

  /** Constructor with implementation */
  SymmetricTensor(const Implementation & i);

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Check if the internal representation is really symmetric */
  void checkSymmetry() const;

#ifndef SWIG
  /** Operator () gives access to the elements of the symmetric tensor (to modify these elements and their symmetric element) */
  /** The element of the symmetric tensor is designated by its row number i, its column number j and its sheet number k*/
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j,
                                 const UnsignedLong k);

  /** Operator () gives access to the elements of the tensor (read only) */
  /** The element of the tensor is designated by its row number i and its column number j */
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j,
                                       const UnsignedLong k) const;
#endif

  /** getSheet returns the sheet specified by its sheet number k */
  SymmetricMatrix getSheet(const UnsignedLong k) const;

  /** setSheet sets matrix m as the sheet specified by its sheet number k  */
  void setSheet(const UnsignedLong k,
                const SymmetricMatrix & m);

protected:

private:
  /** Check if one needs to symmetrized the internal representation of the tensor */
  mutable Bool hasBeenSymmetrized_;

}
  ; /* class SymmetricTensor */


/** Constructor from a range of external collection */
template <class InputIterator>
SymmetricTensor::SymmetricTensor(const UnsignedLong squareDim,
                                 const UnsignedLong sheetDim,
                                 InputIterator first,
                                 InputIterator last)
  : Tensor(squareDim, squareDim, sheetDim, first, last)
{
  // Nothing to do
}




END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SYMMETRICTENSOR_HXX */
