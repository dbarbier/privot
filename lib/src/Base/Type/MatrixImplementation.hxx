//                                               -*- C++ -*-
/**
 *  @file  MatrixImplementation.hxx
 *  @brief MatrixImplementation implements the classical mathematical Matrix
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
 *  Id      MatrixImplementation.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_MATRIXIMPLEMENTATION_HXX
#define OPENTURNS_MATRIXIMPLEMENTATION_HXX

#include "PersistentCollection.hxx"
#include "Description.hxx"
#include "NumericalPoint.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class MatrixImplementation
 *
 * MatrixImplementation implements the classical mathematical MatrixImplementation
 */

// Forward declaration of ComplexMatrixImplementation
class ComplexMatrixImplementation;

class MatrixImplementation
  : public PersistentCollection<NumericalScalar>

{
  CLASSNAME;

#ifndef SWIG
  /** Declaration of friend operators */
  friend MatrixImplementation operator * (const NumericalScalar s,
                                          const MatrixImplementation & matrix)
  {
    return matrix.operator * (s);
  }
#endif


public:

  typedef Collection<NumericalScalar>       NumericalScalarCollection;
  typedef Collection<NumericalComplex>      NumericalComplexCollection;

  /** Default constructor */
  MatrixImplementation();

  /** Constructor with size (rowDim and colDim) */
  MatrixImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  MatrixImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim,
                       const InputIterator first,
                       const InputIterator last);

  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  MatrixImplementation(const UnsignedLong rowDim,
                       const UnsignedLong colDim,
                       const NumericalScalarCollection & elementsValues);

  /** Virtual constructor */
  virtual MatrixImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () gives access to the elements of the MatrixImplementation (to modify these elements) */
  /** The element of the MatrixImplementation is designated by its row number i and its column number j */
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j);

  /** Operator () gives access to the elements of the MatrixImplementation (read only) */
  /** The element of the MatrixImplementation is designated by its row number i and its column number j */
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j) const;

  /** Get the dimensions of the MatrixImplementation */
  /** Number of rows */
  const UnsignedLong getNbRows() const ;
  /** Number of columns */
  const UnsignedLong getNbColumns() const ;
  /** Dimension (for square matrices only */
  const UnsignedLong getDimension() const ;

  /** MatrixImplementation transpose */
  MatrixImplementation transpose () const ;

  /** Row extraction */
  const MatrixImplementation getRow(const UnsignedLong rowIndex) const ;
  const MatrixImplementation getRowSym(const UnsignedLong rowIndex) const ;
  /** Column extration */
  const MatrixImplementation getColumn(const UnsignedLong columnIndex) const ;
  const MatrixImplementation getColumnSym(const UnsignedLong columnIndex) const ;

  /** MatrixImplementation addition (must have the same dimensions) */
  MatrixImplementation operator + (const MatrixImplementation & matrix) const;

  /** MatrixImplementation substraction (must have the same dimensions) */
  MatrixImplementation operator - (const MatrixImplementation & matrix) const;

  /** MatrixImplementation multiplications (must have consistent dimensions) */
  MatrixImplementation genProd (const MatrixImplementation & matrix) const;
  MatrixImplementation symProd (const MatrixImplementation & m,
                                const char symSide) const;

  /** MatrixImplementation integer power */
  MatrixImplementation genPower(const UnsignedLong n) const;
  MatrixImplementation symPower(const UnsignedLong n) const;

  /** Multiplications with a NumericalPoint (must have consistent dimensions) */
  NumericalPoint genVectProd (const NumericalPoint & pt) const;
  NumericalPoint symVectProd (const NumericalPoint & pt) const;

  /** Multiplication with a NumericalScalar */
  MatrixImplementation operator * (const NumericalScalar s) const ;

  /** Division by a NumericalScalar*/
  MatrixImplementation operator / (const NumericalScalar s) const;

  /** Symmetrize MatrixImplementation in case it is a symmetric matrix (stored as a triangular matrix) */
  void symmetrize() const;

  /** Resolution of a linear system in case of a rectangular matrix */
  NumericalPoint solveLinearSystemRect(const NumericalPoint & b,
                                       const Bool keepIntact = true);
  MatrixImplementation solveLinearSystemRect(const MatrixImplementation & b,
                                             const Bool keepIntact = true);

  /** Resolution of a linear system in case of a square matrix */
  NumericalPoint solveLinearSystemSquare(const NumericalPoint & b,
                                         const Bool keepIntact = true);
  MatrixImplementation solveLinearSystemSquare(const MatrixImplementation & b,
                                               const Bool keepIntact = true);

  /** Resolution of a linear system in case of a symmetric matrix */
  NumericalPoint solveLinearSystemSym(const NumericalPoint & b,
                                      const Bool keepIntact = true);
  MatrixImplementation solveLinearSystemSym(const MatrixImplementation & b,
                                            const Bool keepIntact = true);

  /** Resolution of a linear system in case of a covariance matrix */
  NumericalPoint solveLinearSystemCov(const NumericalPoint & b,
                                      const Bool keepIntact = true);
  MatrixImplementation solveLinearSystemCov(const MatrixImplementation & b,
                                            const Bool keepIntact = true);

  /** Compute determinant */
  NumericalScalar computeLogAbsoluteDeterminant(NumericalScalar & sign,
                                                const Bool keepIntact = true);
  NumericalScalar computeDeterminant(const Bool keepIntact = true);
  NumericalScalar computeLogAbsoluteDeterminantSym(NumericalScalar & sign,
                                                   const Bool keepIntact = true);
  NumericalScalar computeDeterminantSym(const Bool keepIntact = true);

  /** Compute eigenvalues */
  NumericalComplexCollection computeEigenValuesSquare(const Bool keepIntact = true);
  NumericalComplexCollection computeEigenValuesSquare(ComplexMatrixImplementation & v,
                                                      const Bool keepIntact = true);
  NumericalPoint computeEigenValuesSym(const Bool keepIntact = true);
  NumericalPoint computeEigenValuesSym(MatrixImplementation & v,
                                       const Bool keepIntact = true);

  /** Compute singular values */
  NumericalPoint computeSingularValues(const Bool keepIntact = true);

  NumericalPoint computeSingularValues(MatrixImplementation & u,
                                       MatrixImplementation & vT,
                                       const Bool fullSVD = false,
                                       const Bool keepIntact = true);

  /** Check if the matrix is symmetric */
  virtual Bool isSymmetric() const;

  /** Check if the matrix is SPD */
  virtual Bool isPositiveDefinite(const Bool keepIntact = true);

  /** Check if the matrix values belong to (-1;1) */
  virtual Bool hasUnitRange() const;

  /** Set small elements to zero */
  virtual MatrixImplementation clean(const NumericalScalar threshold) const;

  virtual MatrixImplementation cleanSym(const NumericalScalar threshold) const;

  /** Build the Cholesky factorization of the matrix */
  virtual MatrixImplementation computeCholesky(const Bool keepIntact = true);

  /** Comparison operators */
  Bool operator == (const MatrixImplementation & rhs) const ;
  inline Bool operator != (const MatrixImplementation & rhs) const
  {
    return !((*this) == rhs);
  }

  /** Empty returns true if there is no element in the MatrixImplementation */
  const Bool isEmpty() const;

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

  /** MatrixImplementation Dimensions */
  UnsignedLong nbRows_;
  UnsignedLong nbColumns_;

  /** Position conversion function : the indices i & j are used to compute the actual position of the element in the collection */
  inline UnsignedLong convertPosition (const UnsignedLong i,
                                       const UnsignedLong j) const ;



}; /* class MatrixImplementation */

inline UnsignedLong MatrixImplementation::convertPosition (const UnsignedLong i,
                                                           const UnsignedLong j) const
{
  return i + nbRows_ * j ;
}

/** Constructor from range of external collection */
template <class InputIterator>
MatrixImplementation::MatrixImplementation(const UnsignedLong rowDim,
                                           const UnsignedLong colDim,
                                           const InputIterator first,
                                           const InputIterator last)
  : PersistentCollection<NumericalScalar>(rowDim * colDim, 0.0),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  this->assign(first, last);
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MATRIXIMPLEMENTATION_HXX */
