//                                               -*- C++ -*-
/**
 *  @file  TensorImplementation.hxx
 *  @brief TensorImplementation implements the Tensor classes
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
 *  Id      TensorImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_TENSORIMPLEMENTATION_HXX
#define OPENTURNS_TENSORIMPLEMENTATION_HXX

#include "PersistentCollection.hxx"
#include "NumericalPoint.hxx"
#include "Matrix.hxx"
#include "SymmetricMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class TensorImplementation
 *
 * TensorImplementation implements the Tensor classes
 */

class TensorImplementation
  : public PersistentCollection<NumericalScalar>

{
  CLASSNAME;

public:


  /** Default constructor */
  TensorImplementation();

  /** Constructor with size (rowDim, colDim, sheetDim) */
  TensorImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim,
                       const UnsignedLong sheetDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  TensorImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim,
                       const UnsignedLong sheetDim,
                       InputIterator first,
                       InputIterator last);

  /** Constructor from external collection */
  /** If the dimensions don't correspond with the size of the collection, */
  /** the collection is either truncated or completed with zeros*/
  TensorImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim,
                       const UnsignedLong sheetDim,
                       const Collection<NumericalScalar> & elementsValues);


  /** Virtual constructor */
  virtual TensorImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () gives access to the elements of the tensor (to modify these elements) */
  /** The element of the tensor is designated by its row number i, its column number j and its sheet number k */
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j,
                                 const UnsignedLong k);

  /** Operator () gives access to the elements of the tensor (read only) */
  /** The element of the tensor is designated by its row number i, its column number j and its sheet number k */
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j,
                                       const UnsignedLong k) const;

  /** getSheet returns the sheet specified by its sheet number k */
  Matrix getSheet(const UnsignedLong k) const;

  /** setSheet sets matrix m as the sheet specified by its sheet number k  */
  void setSheet(const UnsignedLong k,
                const Matrix & m);

  /** getSheetSym returns the symmetric sheet specified by its sheet number k */
  SymmetricMatrix getSheetSym(const UnsignedLong k) const;

  /** setSheetSym sets symmetric matrix m as the sheet specified by its sheet number k  */
  void setSheetSym(const UnsignedLong k,
                   const SymmetricMatrix & m);

  /** Get the dimensions of the tensor */
  /** Number of rows */
  const UnsignedLong getNbRows() const ;
  /** Number of columns */
  const UnsignedLong getNbColumns() const ;
  /** Number of sheets */
  const UnsignedLong getNbSheets() const ;

  /** Check for symmetry */
  Bool isSymmetric() const;

  /** Symmetrize TensorImplementation in case it is a symmetric tensor (stored as a set of triangular matrix sheets) */
  void symmetrize() const;

  /** Comparison operators */
  Bool operator == (const TensorImplementation & rhs) const ;

  /** Empty returns true if there is no element in the tensor */
  const Bool isEmpty() const ;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalScalar * __baseaddress__ () const;
  UnsignedLong __elementsize__ () const;
  UnsignedLong __stride__ (UnsignedLong dim) const;

protected:

  /** TensorImplementation Dimensions */
  UnsignedLong nbRows_;
  UnsignedLong nbColumns_;
  UnsignedLong nbSheets_;

  /** Position conversion function : the indices i & j are used to compute the actual position of the element in the collection */
  inline UnsignedLong convertPosition (const UnsignedLong i,
                                       const UnsignedLong j,
                                       const UnsignedLong k) const ;

}; /* class TensorImplementation */

/** Constructor from range of external collection */
template <class InputIterator>
TensorImplementation::TensorImplementation(const UnsignedLong rowDim,
                                           const UnsignedLong colDim,
                                           const UnsignedLong sheetDim,
                                           InputIterator first,
                                           InputIterator last)
  : PersistentCollection<NumericalScalar>(rowDim * colDim * sheetDim),
    nbRows_(rowDim),
    nbColumns_(colDim),
    nbSheets_(sheetDim)
{
  this->assign(first, last);
}

/** Inline functions */

inline UnsignedLong TensorImplementation::convertPosition (const UnsignedLong i,
                                                           const UnsignedLong j,
                                                           const UnsignedLong k) const
{
  return i + nbRows_ * (j + nbColumns_ * k) ;
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_TENSORIMPLEMENTATION_HXX */
