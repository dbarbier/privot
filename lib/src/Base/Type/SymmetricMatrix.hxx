//                                               -*- C++ -*-
/**
 *  @file  SymmetricMatrix.hxx
 *  @brief SymmetricMatrix implements the classical mathematical symmetric matrix
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
 *  Id      SymmetricMatrix.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_SYMMETRICMATRIX_HXX
#define OPENTURNS_SYMMETRICMATRIX_HXX

#include "OTprivate.hxx"
#include "SquareMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



class IdentityMatrix;

/**
 * @class SymmetricMatrix
 *
 * SymmetricMatrix implements the classical mathematical square matrix
 */

class SymmetricMatrix :
  public SquareMatrix
{
  CLASSNAME;

#ifndef SWIG
  friend SymmetricMatrix operator * (const NumericalScalar & s,
                                     const SymmetricMatrix & m);
#endif

public:

  /** Default constructor */
  SymmetricMatrix();

  /** Constructor with size (dim, which is the same for nbRows_ and nbColumns_) */
  SymmetricMatrix(const UnsignedLong dim);
#if 0
  /** Constructor from range of external collection */
  template <class InputIterator>
  SymmetricMatrix(const UnsignedLong dim,
                  InputIterator first,
                  InputIterator last);
#endif

  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  SymmetricMatrix(const UnsignedLong dim,
                  const NumericalScalarCollection & elementsValues);

  /** Check if the internal representation is really symmetric */
  void checkSymmetry() const;

  /** Test if the matrix is diagonal */
  Bool isDiagonal() const;

  /** Row extraction */
  const Matrix getRow(const UnsignedLong rowIndex) const ;
  /** Column extration */
  const Matrix getColumn(const UnsignedLong columnIndex) const ;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

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

  /** SymmetricMatrix transpose */
  SymmetricMatrix transpose () const;

  /** SymmetricMatrix addition (must have the same dimensions) */
  Matrix operator + (const Matrix & m) const;
  SquareMatrix operator + (const SquareMatrix & m) const;
  SymmetricMatrix operator + (const SymmetricMatrix & m) const;

  /** SymmetricMatrix substraction (must have the same dimensions) */
  Matrix operator - (const Matrix & m) const;
  SquareMatrix operator - (const SquareMatrix & m) const;
  SymmetricMatrix operator - (const SymmetricMatrix & m) const;

  /** SymmetricMatrix multiplications (must have consistent dimensions) */
  Matrix operator * (const Matrix & m) const;
  SquareMatrix operator * (const SquareMatrix & m) const;
  SquareMatrix operator * (const SymmetricMatrix & m) const;
  SymmetricMatrix operator * (const IdentityMatrix & m) const;

  /** SymmetricMatrix integer power */
  SymmetricMatrix power(const UnsignedLong n) const;

  /** Multiplication with a NumericalPoint (must have consistent dimensions) */
  NumericalPoint operator * (const NumericalPoint & p) const;

  /** Multiplication with a NumericalScalar */
  SymmetricMatrix operator * (const NumericalScalar & s) const;

  /** Division by a NumericalScalar*/
  SymmetricMatrix operator / (const NumericalScalar & s) const;

  /** Resolution of a linear system */
  NumericalPoint solveLinearSystem(const NumericalPoint & b,
                                   const Bool keepIntact = true);

  Matrix solveLinearSystem(const Matrix & b,
                           const Bool keepIntact = true);

  /** Compute determinant */
  NumericalScalar computeLogAbsoluteDeterminant(NumericalScalar & sign,
                                                const Bool keepIntact = true);
  NumericalScalar computeDeterminant(const Bool keepIntact = true);

  /** Compute eigenvalues */
  NumericalPoint computeEigenValues(const Bool keepIntact = true);
  NumericalPoint computeEigenValues(SquareMatrix & v,
                                    const Bool keepIntact = true);

  /** Constructor with implementation */
  SymmetricMatrix(const Implementation & i);

protected:


private:
  /** Check if one needs to symmetrized the internal representation of the tensor */
  mutable Bool hasBeenSymmetrized_;

}; /* class SymmetricMatrix */

inline SymmetricMatrix operator * (const NumericalScalar & s,
                                   const SymmetricMatrix & m)
{
  return m.operator * (s);
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MATRIX_HXX */
