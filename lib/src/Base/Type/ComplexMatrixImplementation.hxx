//                                               -*- C++ -*-
/**
 *  @file  ComplexMatrixImplementation.hxx
 *  @brief ComplexMatrixImplementation implements the Matrix class with complex values
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
 *  Id      ComplexMatrixImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_COMPLEXMATRIXIMPLEMENTATION_HXX
#define OPENTURNS_COMPLEXMATRIXIMPLEMENTATION_HXX

#include "PersistentCollection.hxx"
#include "Collection.hxx"
#include "NumericalPoint.hxx"
#include "MatrixImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ComplexMatrixImplementation
 *
 * ComplexMatrixImplementation implements the classical mathematical ComplexMatrixImplementation
 */

class ComplexMatrixImplementation
  : public PersistentCollection<NumericalComplex>

{
  CLASSNAME;

#ifndef SWIG
  /** Declaration of friend operators */
  friend ComplexMatrixImplementation operator * (const NumericalComplex s,
                                                 const ComplexMatrixImplementation & matrix)
  {
    return matrix.operator * (s);
  }
#endif

public:

  typedef Collection<NumericalComplex>        NumericalComplexCollection;
  typedef Collection<NumericalScalar>         NumericalScalarCollection;

  /** Default constructor */
  ComplexMatrixImplementation();

  /** Constructor with size (rowDim and colDim) */
  ComplexMatrixImplementation(const UnsignedLong rowDim,
                              const UnsignedLong colDim);

  /** Constructor from range of external collection */
  template <class InputIterator>
  ComplexMatrixImplementation(const UnsignedLong rowDim,
                              const UnsignedLong colDim,
                              const InputIterator first,
                              const InputIterator last);

  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  ComplexMatrixImplementation(const UnsignedLong rowDim,
                              const UnsignedLong colDim,
                              const Collection<NumericalComplex> & elementsValues);


  /** Constructor from external collection */
  /** If the dimensions of the matrix and of the collection */
  /** do not correspond, either the collection is truncated */
  /** or the rest of the matrix is filled with zeros */
  ComplexMatrixImplementation(const UnsignedLong rowDim,
                              const UnsignedLong colDim,
                              const Collection<NumericalScalar> & elementsValues);

  /** Constructor from MatrixImplementation */
  ComplexMatrixImplementation(const MatrixImplementation & matrix);

  /** Virtual constructor */
  virtual ComplexMatrixImplementation * clone() const;

  /** Set small elements to zero */
  virtual ComplexMatrixImplementation clean(const NumericalScalar threshold) const;

  virtual ComplexMatrixImplementation cleanHerm(const NumericalScalar threshold) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Operator () gives access to the elements of the ComplexMatrixImplementation (to modify these elements) */
  /** The element of the ComplexMatrixImplementation is designated by its row number i and its column number j */
  NumericalComplex & operator () (const UnsignedLong i,
                                  const UnsignedLong j);

  /** Operator () gives access to the elements of the ComplexMatrixImplementation (read only) */
  /** The element of the ComplexMatrixImplementation is designated by its row number i and its column number j */
  const NumericalComplex & operator () (const UnsignedLong i,
                                        const UnsignedLong j) const;

  /** Get the dimensions of the ComplexMatrixImplementation */
  /** Number of rows */
  const UnsignedLong getNbRows() const;
  /** Number of columns */
  const UnsignedLong getNbColumns() const;
  /** Dimension (for square matrices only */
  const UnsignedLong getDimension() const;

  /** ComplexMatrixImplementation transpose */
  ComplexMatrixImplementation transpose () const;
  ComplexMatrixImplementation transposeHerm () const;

  /** ComplexMatrixImplementation conjugate */
  ComplexMatrixImplementation conjugate () const;
  ComplexMatrixImplementation conjugateHerm () const;

  /** ComplexMatrixImplementation conjugateTranspose */
  ComplexMatrixImplementation conjugateTranspose () const;

  /** "Hermitianize" ComplexMatrixImplementation in case it is an hermitian matrix (stored as a triangular matrix) */
  void hermitianize() const;

  /** Get the real part of the matrix */
  MatrixImplementation realRect() const;
  MatrixImplementation realSym() const;

  /** Get the imaginary part of the matrix */
  MatrixImplementation imagRect() const;
  MatrixImplementation imagSym() const;

  /** Operator overload */
  /** ComplexMatrixImplementation addition (must have the same dimensions) */
  ComplexMatrixImplementation operator + (const ComplexMatrixImplementation & matrix) const;
  /** ComplexMatrixImplementation addition with MatrixImplementation */
  ComplexMatrixImplementation operator + (const MatrixImplementation & matrix) const;

  /** ComplexMatrixImplementation substraction (must have the same dimensions) */
  ComplexMatrixImplementation operator - (const ComplexMatrixImplementation & matrix) const;
  /** ComplexMatrixImplementation substraction with MatrixImplementation  */
  ComplexMatrixImplementation operator - (const MatrixImplementation & matrix) const;

  /** Multiplication with a NumericalComplex */
  ComplexMatrixImplementation operator * (const NumericalComplex s) const ;

  /** Division by a NumericalComplex*/
  ComplexMatrixImplementation operator / (const NumericalComplex s) const ;

  /** ComplexMatrixImplementation multiplications (must have consistent dimensions) */
  ComplexMatrixImplementation genProd(const ComplexMatrixImplementation & matrix) const;
  ComplexMatrixImplementation symProd(const ComplexMatrixImplementation & m,
                                      const char symSide) const;
  ComplexMatrixImplementation hermProd(const ComplexMatrixImplementation & m,
                                       const char hermSide) const;
  /** Triangular matrix product : side argument L/R for the position of the triangular matrix, up/lo to tell if it  */
  ComplexMatrixImplementation triangularProd(const ComplexMatrixImplementation & m,
                                             const char side = 'L',
                                             const char uplo = 'L') const;

  /** ComplexMatrixImplementation integer power */
  ComplexMatrixImplementation genPower(const UnsignedLong n) const;
  ComplexMatrixImplementation symPower(const UnsignedLong n) const;
  ComplexMatrixImplementation hermPower(const UnsignedLong n) const;

  /** Multiplications with a NumericalComplexCollection (must have consistent dimensions) */
  NumericalComplexCollection genVectProd (const NumericalComplexCollection & pt) const;
  NumericalComplexCollection genVectProd (const NumericalScalarCollection & pt) const;
  NumericalComplexCollection genVectProd (const NumericalPoint & pt) const;

  /** Using some optimization (for Hermitian matrix) */
  NumericalComplexCollection hermVectProd (const NumericalComplexCollection & pt) const;
  NumericalComplexCollection hermVectProd (const NumericalScalarCollection & pt) const;
  NumericalComplexCollection hermVectProd (const NumericalPoint & pt) const;

  /** Using triangular matrix */
  NumericalComplexCollection triangularVectProd(const NumericalComplexCollection & pt,
                                                const char side = 'L') const;
  NumericalComplexCollection triangularVectProd(const NumericalScalarCollection & pt,
                                                const char side = 'L') const;


  NumericalComplexCollection triangularVectProd(const NumericalPoint & pt,
                                                const char side = 'L') const;

  /** Check if the matrix is self-adjoint */
  virtual Bool isHermitian() const;

  /** Check if the matrix is HPD */
  virtual Bool isHermitianPositiveDefinite(const Bool keepIntact = true);

  /** Build the Cholesky factorization of the matrix */
  virtual ComplexMatrixImplementation computeCholesky(const Bool keepIntact = true);

  /** Comparison operators */
  Bool operator == (const ComplexMatrixImplementation & rhs) const ;
  inline Bool operator != (const ComplexMatrixImplementation & rhs) const
  {
    return !((*this) == rhs);
  }

  /** Empty returns true if there is no element in the ComplexMatrixImplementation */
  const Bool isEmpty() const;

  /** Returns true if triangular lower or upper */
  const Bool isTriangular(Bool lower = true) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

  // These functions are only intended to be used by SWIG, DO NOT use them for your own purpose !
  // INTENTIONALY NOT DOCUMENTED
  const NumericalComplex * __baseaddress__ () const;
  UnsignedLong __elementsize__ () const;
  UnsignedLong __stride__ (UnsignedLong dim) const;

protected:

  /** ComplexMatrixImplementation Dimensions */
  UnsignedLong nbRows_;
  UnsignedLong nbColumns_;

  /** Position conversion function : the indices i & j are used to compute the actual position of the element in the collection */
  inline UnsignedLong convertPosition (const UnsignedLong i,
                                       const UnsignedLong j) const;

}; /* class ComplexMatrixImplementation */

inline UnsignedLong ComplexMatrixImplementation::convertPosition (const UnsignedLong i,
                                                                  const UnsignedLong j) const
{
  return i + nbRows_ * j ;
}

/** Constructor from range of external collection */
template <class InputIterator>
ComplexMatrixImplementation::ComplexMatrixImplementation(const UnsignedLong rowDim,
                                                         const UnsignedLong colDim,
                                                         const InputIterator first,
                                                         const InputIterator last)
  : PersistentCollection<NumericalComplex>(rowDim * colDim, 0.0),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  this->assign(first, last);
}

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COMPLEXMATRIXIMPLEMENTATION_HXX */
