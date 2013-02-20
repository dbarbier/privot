//                                               -*- C++ -*-
/**
 *  @file  Matrix.hxx
 *  @brief Matrix implements the classical mathematical matrix
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      Matrix.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_MATRIX_HXX
#define OPENTURNS_MATRIX_HXX

#include "MatrixImplementation.hxx"
#include "TypedInterfaceObject.hxx"




BEGIN_NAMESPACE_OPENTURNS



class IdentityMatrix;
class SymmetricMatrix;

/**
 * @class Matrix
 *
 * Matrix implements the classical mathematical matrix
 */

class Matrix :
  public TypedInterfaceObject<MatrixImplementation>
{
  CLASSNAME;

#ifndef SWIG
  /** Declaration of friend operators */
  friend Matrix operator * (const NumericalScalar s,
                            const Matrix & m);
#endif


public:

  typedef Collection<NumericalScalar>       NumericalScalarCollection;
  typedef Collection<NumericalComplex>      NumericalComplexCollection;

  /** Default constructor */
  Matrix();

  /** Constructor with implementation */
  Matrix(const Implementation & i);

  /** Constructor with implementation */
  Matrix(const MatrixImplementation & i);

  /** Constructor with size (rowDim and colDim) */
  Matrix(const UnsignedLong rowDim,
         const UnsignedLong colDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  Matrix(const UnsignedLong rowDim,
         const UnsignedLong colDim,
         const InputIterator first,
         const InputIterator last);

  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  Matrix(const UnsignedLong rowDim,
         const UnsignedLong colDim,
         const NumericalScalarCollection & elementsValues);

  /** Set small elements to zero */
  virtual Matrix clean(const NumericalScalar threshold) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

#ifndef SWIG
  /** Operator () gives access to the elements of the matrix (to modify these elements) */
  /** The element of the matrix is designated by its row number i and its column number j */
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j);

  /** Operator () gives access to the elements of the matrix (read only) */
  /** The element of the matrix is designated by its row number i and its column number j */
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j) const;
#endif

  /** Get the dimensions of the matrix */
  /** Number of rows */
  const UnsignedLong getNbRows() const;
  /** Number of columns */
  const UnsignedLong getNbColumns() const;

  /** Matrix transpose */
  Matrix transpose () const;

  /** Row extraction */
  const Matrix getRow(const UnsignedLong rowIndex) const ;
  /** Column extration */
  const Matrix getColumn(const UnsignedLong columnIndex) const ;

  /** Matrix additions (must have the same dimensions) */
  Matrix operator + (const Matrix & m) const;
  Matrix operator + (const SymmetricMatrix & m) const;

  /** Matrix substractions (must have the same dimensions) */
  Matrix operator - (const Matrix & m) const;
  Matrix operator - (const SymmetricMatrix & m) const;

  /** Matrix multiplications (must have consistent dimensions) */
  Matrix operator * (const Matrix & m) const;
  Matrix operator * (const SymmetricMatrix & m) const;
  Matrix operator * (const IdentityMatrix & m) const;

  /** Multiplication with a NumericalPoint (must have consistent dimensions) */
  NumericalPoint operator * (const NumericalPoint & pt) const;

  /** Multiplication with a NumericalScalar */
  Matrix operator * (const NumericalScalar s) const;

  /** Division by a NumericalScalar*/
  Matrix operator / (const NumericalScalar s) const;

  /** Resolution of a linear system */
  NumericalPoint solveLinearSystem(const NumericalPoint & b,
                                   const Bool keepIntact = true);
  Matrix solveLinearSystem(const Matrix & b,
                           const Bool keepIntact = true);

  /** Compute singular values */
  NumericalPoint computeSingularValues(const Bool keepIntact = true);

  NumericalPoint computeSingularValues(Matrix & u,
                                       Matrix & vT,
                                       const Bool fullSVD = false,
                                       const Bool keepIntact = true);

  /** Comparison operators */
  Bool operator == (const Matrix & rhs) const;

  /** Empty returns true if there is no element in the matrix */
  const Bool isEmpty() const;

  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalScalar * __baseaddress__ () const;
  UnsignedLong __elementsize__ () const;
  UnsignedLong __stride__ (UnsignedLong dim) const;

}; /* class Matrix */

/** Declaration of friend operators */
inline Matrix operator * (const NumericalScalar s,
                          const Matrix & m)
{
  return m.operator * (s);
}


/** Constructor from range of external collection */
template <class InputIterator>
Matrix::Matrix(const UnsignedLong rowDim,
               const UnsignedLong colDim,
               const InputIterator first,
               const InputIterator last)
  : TypedInterfaceObject<MatrixImplementation>(new MatrixImplementation(rowDim, colDim, first, last))
{
  // Nothing to do
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MATRIX_HXX */
