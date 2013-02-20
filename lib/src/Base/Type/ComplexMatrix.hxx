//                                               -*- C++ -*-
/**
 *  @file  ComplexMatrix.hxx
 *  @brief ComplexMatrix implements the classical mathematical matrix with complex values
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
 *  Id      ComplexMatrix.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_COMPLEXMATRIX_HXX
#define OPENTURNS_COMPLEXMATRIX_HXX

#include "ComplexMatrixImplementation.hxx"
#include "TypedInterfaceObject.hxx"
#include "Matrix.hxx"
#include "SymmetricMatrix.hxx"
#include "IdentityMatrix.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

class HermitianMatrix;
class TriangularComplexMatrix;

/**
 * @class ComplexMatrix
 *
 * ComplexMatrix implements the classical mathematical matrix with complex values
 */

class ComplexMatrix :
  public TypedInterfaceObject<ComplexMatrixImplementation>
{
  CLASSNAME;

#ifndef SWIG
  /** Declaration of friend operators */
  friend ComplexMatrix operator * (const NumericalComplex s,
                                   const ComplexMatrix & m);
#endif


public:

  typedef Collection<NumericalComplex>                       NumericalComplexCollection;
  typedef Collection<NumericalScalar>                        NumericalScalarCollection;
  typedef TypedInterfaceObject<ComplexMatrixImplementation>::Implementation     Implementation ;

  /** Default constructor */
  ComplexMatrix();

  /** Constructor with implementation */
  ComplexMatrix(const Implementation & i);

  /** Constructor with implementation */
  ComplexMatrix(const ComplexMatrixImplementation & i);

  /** Constructor with size (rowDim and colDim) */
  ComplexMatrix(const UnsignedLong rowDim,
                const UnsignedLong colDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  ComplexMatrix(const UnsignedLong rowDim,
                const UnsignedLong colDim,
                const InputIterator first,
                const InputIterator last);

  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  ComplexMatrix(const UnsignedLong rowDim,
                const UnsignedLong colDim,
                const NumericalComplexCollection & elementsValues);


  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  ComplexMatrix(const UnsignedLong rowDim,
                const UnsignedLong colDim,
                const NumericalScalarCollection & elementsValues);


  /** Set small elements to zero */
  virtual ComplexMatrix clean(const NumericalScalar threshold) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

#ifndef SWIG
  /** Operator () gives access to the elements of the matrix (to modify these elements) */
  /** The element of the matrix is designated by its row number i and its column number j */
  NumericalComplex & operator () (const UnsignedLong i,
                                  const UnsignedLong j);

  /** Operator () gives access to the elements of the matrix (read only) */
  /** The element of the matrix is designated by its row number i and its column number j */
  const NumericalComplex & operator () (const UnsignedLong i,
                                        const UnsignedLong j) const;
#endif

  /** Get the dimensions of the matrix */
  /** Number of rows */
  const UnsignedLong getNbRows() const ;
  /** Number of columns */
  const UnsignedLong getNbColumns() const ;

  /** ComplexMatrix transpose */
  ComplexMatrix transpose () const ;

  /** ComplexMatrix conjugate */
  ComplexMatrix conjugate () const ;

  /** ComplexMatrix conjugate and transpose */
  ComplexMatrix conjugateTranspose () const ;

  /** Get the real part of the matrix */
  Matrix real() const;

  /** Get the imaginary part of the matrix */
  Matrix imag() const;

  /** ComplexMatrix additions (must have the same dimensions) */
  ComplexMatrix operator + (const ComplexMatrix & m) const;

  /** ComplexMatrix additions with Matrix */
  ComplexMatrix operator + (const Matrix & m) const;

  /** ComplexMatrix substractions (must have the same dimensions) */
  ComplexMatrix operator - (const ComplexMatrix & m) const;

  /** ComplexMatrix substractions with Matrix */
  ComplexMatrix operator - (const Matrix & m) const;

  /** ComplexMatrix multiplications (must have consistent dimensions) */
  ComplexMatrix operator * (const ComplexMatrix & m) const;

  /** ComplexMatrix multiplications with Hermitian Matrix */
  ComplexMatrix operator * (const HermitianMatrix & m) const;

  /** ComplexMatrix multiplications with Triangular Matrix */
  ComplexMatrix operator * (const TriangularComplexMatrix & m) const;

  /** ComplexMatrix multiplication with Matrix argument */
  virtual ComplexMatrix operator * (const Matrix & m) const;

  /** ComplexMatrix multiplication with SymmetricMatrix argument */
  ComplexMatrix operator * (const SymmetricMatrix & m) const;

  /** ComplexMatrix multiplication with IdentityMatrix argument */
  ComplexMatrix operator * (const IdentityMatrix & m) const;

  /** Multiplication with a NumericalPoint  */
  virtual NumericalComplexCollection operator* (const NumericalPoint & point) const;

  /** Multiplication with a NumericalScalarCollection  */
  virtual  NumericalComplexCollection operator* (const NumericalScalarCollection & collection) const;

  /** Multiplication with a NumericalComplexCollection (must have consistent dimensions) */
  virtual NumericalComplexCollection operator* (const NumericalComplexCollection & collection) const;

  /** Multiplication with a NumericalComplex */
  ComplexMatrix operator * (const NumericalComplex s) const ;

  /** Division by a NumericalComplex*/
  ComplexMatrix operator / (const NumericalComplex s) const ;

  /** Comparison operators */
  Bool operator == (const ComplexMatrix & rhs) const ;

  /** Empty returns true if there is no element in the matrix */
  const Bool isEmpty() const;

  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalComplex * __baseaddress__ () const;
  UnsignedLong __elementsize__ () const;
  UnsignedLong __stride__ (UnsignedLong dim) const;

}; /* class ComplexMatrix */

/** Declaration of friend operators */
inline ComplexMatrix operator * (const NumericalComplex s,
                                 const ComplexMatrix & m)
{
  return m.operator * (s);
}


/** Constructor from range of external collection */
template <class InputIterator>
ComplexMatrix::ComplexMatrix(const UnsignedLong rowDim,
                             const UnsignedLong colDim,
                             const InputIterator first,
                             const InputIterator last)
  : TypedInterfaceObject<ComplexMatrixImplementation>(new ComplexMatrixImplementation(rowDim, colDim, first, last))
{
  // Nothing to do
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPLEXMATRIX_HXX */
