
//                                               -*- C++ -*-
/**
 *  @file  MatrixImplementation.cxx
 *  @brief MatrixImplementation implements the classical mathematical MatrixImplementation
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include <cstdlib>
#include <functional>

#include "MatrixImplementation.hxx"
#include "ComplexMatrixImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Lapack.hxx"
#include "ResourceMap.hxx"
#include "SpecFunc.hxx"
#include "Os.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MatrixImplementation);


static Factory<MatrixImplementation> RegisteredFactory("MatrixImplementation");

// All the pivots with a magnitude less than this threshold are considered as zero
/* Default constructor */
MatrixImplementation::MatrixImplementation()
  : PersistentCollection<NumericalScalar>(),
    nbRows_(0),
    nbColumns_(0)
{
  // Nothing to do
}

/* Constructor with size (rowDim and colDim) */
/* The MatrixImplementation is made up of a collection of rowDim*colDim elements */
/* The MatrixImplementation is viewed as a set of column vectors read one after another */
MatrixImplementation::MatrixImplementation(const UnsignedLong rowDim,
                                           const UnsignedLong colDim)
  : PersistentCollection<NumericalScalar>(rowDim * colDim, 0.0),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  // Nothing to do
}

/* Constructor from external collection */
MatrixImplementation::MatrixImplementation(const UnsignedLong rowDim,
                                           const UnsignedLong colDim,
                                           const Collection<NumericalScalar> & elementsValues)
  : PersistentCollection<NumericalScalar>(rowDim * colDim, 0.0),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  const UnsignedLong matrixSize(std::min(rowDim * colDim, elementsValues.getSize()));
  for(UnsignedLong i = 0; i < matrixSize; ++i) operator[](i) = elementsValues[i];
}

/* Virtual constructor */
MatrixImplementation * MatrixImplementation::clone() const
{
  return new MatrixImplementation(*this);
}


/* String converter */
String MatrixImplementation::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " name=" << getName()
               << " rows=" << nbRows_
               << " columns=" << nbColumns_
               << " values=" << PersistentCollection<NumericalScalar>::__repr__();
}

String MatrixImplementation::__str__(const String & offset) const
{
  OSS oss;
  if (nbRows_ == 0 || nbColumns_ == 0) return "[]";

  if ( (nbRows_ >= ResourceMap::GetAsUnsignedLong("Matrix-size-visible-in-str-from")) ||
       (nbColumns_ >= ResourceMap::GetAsUnsignedLong("Matrix-size-visible-in-str-from")) )
    oss << nbRows_ << "x" << nbColumns_ << "\n";

  size_t lwidth(0);
  size_t rwidth(0);
  for ( UnsignedLong i = 0; i < nbRows_; ++i )
    for ( UnsignedLong j = 0; j < nbColumns_; ++j )
      {
        String st(OSS() << (*this)(i, j));
        size_t dotpos(st.find( '.' ));
        lwidth = std::max( lwidth, (dotpos != String::npos) ? dotpos             : st.size() );
        rwidth = std::max( rwidth, (dotpos != String::npos) ? st.size() - dotpos : 0         );
      }

  const char * bracket("[");
  const char * newline("");
  for ( UnsignedLong i = 0; i < nbRows_; ++i, newline = "\n", bracket = " " )
    {
      oss << newline << offset << bracket << "[ ";
      const char * sep("");
      for ( UnsignedLong j = 0; j < nbColumns_; ++j, sep = " " )
        {
          String st(OSS() << (*this)(i, j));
          size_t dotpos(st.find( '.' ));
          oss << sep << String( lwidth - ((dotpos != String::npos) ? dotpos : st.size()), ' ' )
              << st
              << String( rwidth - ((dotpos != String::npos) ? st.size() - dotpos : 0), ' ' );
        }
      oss << " ]";
    }
  oss << "]";
  return oss;
}

/* Operator () gives access to the elements of the MatrixImplementation (to modify these elements) */
/* The element of the MatrixImplementation is designated by its row number i and its column number j */
/* the first element of the MatrixImplementation is m(0,0) */
NumericalScalar & MatrixImplementation::operator() (const UnsignedLong i,
                                                    const UnsignedLong j)
{
  if (i >= nbRows_) throw OutOfBoundException(HERE) << "i (" << i << ") must be less than row dim (" << nbRows_ << ")";
  if (j >= nbColumns_) throw OutOfBoundException(HERE) << "j (" << j << ") must be less than column dim (" << nbColumns_ << ")";
  return operator[](convertPosition(i, j));
}

/* Operator () gives access to the elements of the MatrixImplementation (read only) */
/* The element of the MatrixImplementation is designated by its row number i and its column number j */
const NumericalScalar & MatrixImplementation::operator() (const UnsignedLong i,
                                                          const UnsignedLong j)  const
{
  if (i >= nbRows_) throw OutOfBoundException(HERE) << "i (" << i << ") must be less than row dim (" << nbRows_ << ")";
  if (j >= nbColumns_) throw OutOfBoundException(HERE) << "j (" << j << ") must be less than column dim (" << nbColumns_ << ")";
  return operator[](convertPosition(i, j));
}


/* Get the dimensions of the MatrixImplementation : number of rows */
const UnsignedLong MatrixImplementation::getNbRows() const
{
  return nbRows_;
}

/* Get the dimensions of the MatrixImplementation : number of columns */
const UnsignedLong MatrixImplementation::getNbColumns() const
{
  return nbColumns_;
}

/* Get the dimensions of the MatrixImplementation : dimension (square matrix : nbRows_) */
const UnsignedLong MatrixImplementation::getDimension() const
{
  return nbRows_;
}

/* MatrixImplementation transpose */
MatrixImplementation MatrixImplementation::transpose () const
{
  MatrixImplementation trans(nbColumns_, nbRows_);
  // The source matrix is accessed columnwise in the natural order
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      trans(j, i) = operator()(i, j);
  return trans;
}

/* Row extraction */
const MatrixImplementation MatrixImplementation::getRow(const UnsignedLong rowIndex) const
{
  if (rowIndex >= nbRows_) throw InvalidArgumentException(HERE) << "Error: the row index=" << rowIndex << " must be less than the row number=" << nbRows_;
  MatrixImplementation row(1, nbColumns_);
  for (UnsignedLong i = 0; i < nbColumns_; ++i) row(0, i) = (*this)(rowIndex, i);
  return row;
}

const MatrixImplementation MatrixImplementation::getRowSym(const UnsignedLong rowIndex) const
{
  if (rowIndex >= nbRows_) throw InvalidArgumentException(HERE) << "Error: the row index=" << rowIndex << " must be less than the row number=" << nbRows_;
  MatrixImplementation row(1, nbColumns_);
  for (UnsignedLong i = 0; i < rowIndex; ++i) row(0, i) = (*this)(rowIndex, i);
  for (UnsignedLong i = rowIndex; i < nbColumns_; ++i) row(0, i) = (*this)(i, rowIndex);
  return row;
}

/* Column extration */
const MatrixImplementation MatrixImplementation::getColumn(const UnsignedLong columnIndex) const
{
  if (columnIndex >= nbColumns_) throw InvalidArgumentException(HERE) << "Error: the column index=" << columnIndex << " must be less than the column number=" << nbColumns_;
  MatrixImplementation column(nbRows_, 1);
  for (UnsignedLong i = 0; i < nbRows_; ++i) column(i, 0) = (*this)(i, columnIndex);
  return column;
}

const MatrixImplementation MatrixImplementation::getColumnSym(const UnsignedLong columnIndex) const
{
  if (columnIndex >= nbColumns_) throw InvalidArgumentException(HERE) << "Error: the column index=" << columnIndex << " must be less than the column number=" << nbColumns_;
  MatrixImplementation column(nbRows_, 1);
  for (UnsignedLong i = 0; i < columnIndex; ++i) column(i, 0) = (*this)(columnIndex, i);
  for (UnsignedLong i = columnIndex; i < nbRows_; ++i) column(i, 0) = (*this)(i, columnIndex);
  return column;
}


/* MatrixImplementation addition (must have the same dimensions) */
MatrixImplementation MatrixImplementation::operator+ (const MatrixImplementation & matrix) const
{
  if ((nbRows_ != matrix.nbRows_ ) || (nbColumns_ != matrix.nbColumns_ )) throw InvalidDimensionException(HERE);
  // Must copy as add will be overwritten by the operation
  MatrixImplementation add(matrix);
  int size(nbRows_ * nbColumns_);
  double alpha(1.0);
  int one(1);
  DAXPY_F77(&size, &alpha, const_cast<double*>(&((*this)[0])), &one, &add[0], &one);

  return add;
}

/* MatrixImplementation substraction (must have the same dimensions) */
MatrixImplementation MatrixImplementation::operator- (const MatrixImplementation & matrix) const
{
  if ((nbRows_ != matrix.nbRows_ ) || (nbColumns_ != matrix.nbColumns_ )) throw InvalidDimensionException(HERE);
  // Must copy as add will be overwritten by the operation
  MatrixImplementation sub(*this);
  int size(nbRows_ * nbColumns_);
  double alpha(-1.0);
  int one(1);
  DAXPY_F77(&size, &alpha, const_cast<double*>(&(matrix[0])), &one, &sub[0], &one);

  return sub;
}

/* MatrixImplementation multiplications (must have consistent dimensions) */
MatrixImplementation MatrixImplementation::genProd (const MatrixImplementation & matrix) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in matrix/matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;

  MatrixImplementation mult(nbRows_, matrix.nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char transa('N');
  char transb('N');
  int m(nbRows_);
  int k(nbColumns_);
  int n(matrix.nbColumns_);
  double alpha(1.0);
  double beta(0.0);
  int ltransa(1);
  int ltransb(1);
  DGEMM_F77(&transa, &transb, &m, &n, &k, &alpha, const_cast<double*>(&((*this)[0])), &m, const_cast<double*>(&(matrix[0])), &k, &beta, &mult[0], &m, &ltransa, &ltransb);

  return mult;
}

MatrixImplementation MatrixImplementation::symProd (const MatrixImplementation & matrix,
                                                    const char symSide) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in matrix/matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;
  MatrixImplementation mult(nbRows_, matrix.nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char side(symSide);
  char uplo('L');
  int m(nbRows_);
  int k(nbColumns_);
  int n(matrix.nbColumns_);
  double alpha(1.0);
  double beta(0.0);
  int lside(1);
  int luplo(1);
  if (side == 'L') DSYMM_F77(&side, &uplo, &m, &n, &alpha, const_cast<double*>(&((*this)[0])), &m, const_cast<double*>(&(matrix[0])), &k, &beta, &mult[0], &m, &lside, &luplo);
  else DSYMM_F77(&side, &uplo, &m, &n, &alpha, const_cast<double*>(&(matrix[0])), &k, const_cast<double*>(&((*this)[0])), &m, &beta, &mult[0], &m, &lside, &luplo);
  return mult;
}

/* Multiplications with a NumericalPoint (must have consistent dimensions) */
NumericalPoint MatrixImplementation::genVectProd (const NumericalPoint & pt) const
{
  if (nbColumns_ != pt.getDimension() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product: columns=" << nbColumns_ << " / vector dimension=" << pt.getDimension() << ".";

  NumericalPoint prod(nbRows_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return prod;
  char trans('N');
  int m_(nbRows_);
  int n_(nbColumns_);
  int one(1);
  double alpha(1.0);
  double beta(0.0);
  int ltrans(1);

  DGEMV_F77(&trans, &m_, &n_, &alpha, const_cast<double*>(&((*this)[0])), &m_, const_cast<double*>(&(pt[0])), &one, &beta, &prod[0], &one, &ltrans);

  return prod;
}

NumericalPoint MatrixImplementation::symVectProd (const NumericalPoint & pt) const
{
  if (nbColumns_ != pt.getDimension() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalPoint prod(nbRows_);
  // In this case, nbRows_ == nbColumns_
  if (nbRows_ == 0) return prod;
  char uplo('L');
  int n(nbRows_);
  int one(1);
  double alpha(1.0);
  double beta(0.0);
  int luplo(1);
  DSYMV_F77(&uplo, &n, &alpha, const_cast<double*>(&((*this)[0])), &n, const_cast<double*>(&(pt[0])), &one, &beta, &prod[0], &one, &luplo);

  return prod;
}

/* Multiplication with a NumericalScalar */
MatrixImplementation MatrixImplementation::operator* (const NumericalScalar s) const
{
  if (s == 0.0) return MatrixImplementation(nbRows_, nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return *this;
  MatrixImplementation scalprod(*this);
  double alpha(s);
  int one(1);
  int n_(nbRows_ * nbColumns_);
  DSCAL_F77(&n_, &alpha, &scalprod[0], &one);

  return scalprod;
}

/* Division by a NumericalScalar*/
MatrixImplementation MatrixImplementation::operator/ (const NumericalScalar s) const
{
  if (s == 0.0) throw InvalidArgumentException(HERE);

  return operator * (1.0 / s);
}

/* Integer power, general matrix */
MatrixImplementation MatrixImplementation::genPower(const UnsignedLong n) const
{
  Bool first(true);
  UnsignedLong exponent(n);
  MatrixImplementation y;
  MatrixImplementation z(*this);
  while (exponent > 0)
    {
      // t is the right bit of exponent
      const UnsignedLong t(exponent % 2);
      // remove last bit from exponent
      exponent /= 2;
      // if right bit is 1
      if (t != 0)
        {
          // if it is the rightmost bit equals to 1
          if (first)
            {
              first = false;
              y = z;
            }
          else y = y.genProd(z);
          // if no more bit to consider
          if (exponent == 0) return y;
        }
      // Square the contribution
      z = z.genProd(z);
    }
  return y;
}

/* Integer power, symmetric matrix */
MatrixImplementation MatrixImplementation::symPower(const UnsignedLong n) const
{
  Bool first(true);
  UnsignedLong exponent(n);
  MatrixImplementation y;
  MatrixImplementation z(*this);
  while (exponent > 0)
    {
      // t is the right bit of exponent
      const UnsignedLong t(exponent % 2);
      // remove last bit from exponent
      exponent /= 2;
      // if right bit is 1
      if (t != 0)
        {
          // if it is the rightmost bit equals to 1
          if (first)
            {
              first = false;
              y = z;
            }
          else y = y.symProd(z, 'L');
          // if no more bit to consider
          if (exponent == 0) return y;
        }
      // Square the contribution
      z = z.symProd(z, 'L');
    }
  return y;
}

/* Empty returns true if there is no element in the MatrixImplementation */
const Bool MatrixImplementation::isEmpty() const
{
  return ((nbRows_ == 0)  || (nbColumns_ == 0) || (PersistentCollection<NumericalScalar>::isEmpty()));
}


/* Comparison operator */
Bool MatrixImplementation::operator == (const MatrixImplementation & rhs) const
{
  const MatrixImplementation &lhs(*this);
  Bool equality(true);

  if (&lhs != &rhs)   // Not the same object
    {
      const Collection<NumericalScalar> & refLhs(static_cast<const Collection<NumericalScalar> >(lhs));
      const Collection<NumericalScalar> & refRhs(static_cast<const Collection<NumericalScalar> >(rhs));
      equality = ( lhs.nbRows_ == rhs.nbRows_ && lhs.nbColumns_ == rhs.nbColumns_ && refLhs == refRhs);
    }

  return equality;
}


Bool MatrixImplementation::isSymmetric() const
{
  if ( nbRows_ == nbColumns_ )
    {
      for ( UnsignedLong i = 1; i < nbRows_; ++ i )
        for ( UnsignedLong j = 0; j < i; ++ j )
          if ( this->operator[](convertPosition(i, j)) != operator[](convertPosition(j, i)) )
            return false;
      return true;
    }
  else
    return false;
}


/* Copy the lower triangle into the upper one, used by symmetric matrices */
void MatrixImplementation::symmetrize() const
{
  MatrixImplementation *refThis(const_cast<MatrixImplementation *>(this));
  // Loop over the upper triangle
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < j; ++i)
      refThis->operator[](convertPosition(i, j)) = operator[](convertPosition(j, i));
}

/* Check if the matrix values belong to (-1;1) */
Bool MatrixImplementation::hasUnitRange() const
{
  bool unitRange = true;
  for (UnsignedLong i = 0; i < nbRows_; ++i)
    for (UnsignedLong j = 0; j < nbColumns_; ++j)
      {
        if (fabs(this->operator[](convertPosition(i, j))) > 1.0)
          {
            unitRange = false;
            break;
          }
      }
  return unitRange;
}

/* Set small elements to zero */
MatrixImplementation MatrixImplementation::clean(const NumericalScalar threshold) const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      {
        NumericalScalar value((*this)(i, j));
        if (fabs(value) <= threshold) value = 0.0;
        result(i, j) = value;
      }
  return result;
}

/* Set small elements to zero */
MatrixImplementation MatrixImplementation::cleanSym(const NumericalScalar threshold) const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j; i < nbRows_; ++i)
      {
        NumericalScalar value((*this)(i, j));
        if (fabs(value) <= threshold) value = 0.0;
        result(i, j) = value;
      }
  return result;
}

/* Resolution of a linear system : rectangular matrix
 * MX = b, M is an mxn matrix, b is an mxq matrix and
 * X is an nxq matrix */
MatrixImplementation MatrixImplementation::solveLinearSystemRect (const MatrixImplementation & b,
                                                                  const Bool keepIntact)
{
  if (nbRows_ != b.nbRows_) throw InvalidDimensionException(HERE);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (b.nbColumns_ == 0)) throw InvalidDimensionException(HERE);
  int m(nbRows_);
  int n(nbColumns_);
  // B is an extended copy of b, it must be large enought to store the solution, see LAPACK documentation
  int p(std::max(m, n));
  int q(b.nbColumns_);
  MatrixImplementation B(p, q);
  for(UnsignedLong j = 0; j < static_cast<UnsignedLong>(q); ++j)
    for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(m); ++i)
      B(i, j) = b(i, j);
  int nrhs(q);
  int lwork(-1);
  double lwork_d;
  int info;
  std::vector<int> jpiv(n);
  double rcond(ResourceMap::GetAsNumericalScalar("MatrixImplementation-DefaultSmallPivot"));
  int rank;
  // We must copy the matrix as it will be overwritten by the operation
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DGELSY_F77(&m, &n, &nrhs, &A[0], &m, &B[0], &p, &jpiv[0], &rcond, &rank, &lwork_d, &lwork, &info);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGELSY_F77(&m, &n, &nrhs, &A[0], &m, &B[0], &p, &jpiv[0], &rcond, &rank, &work[0], &lwork, &info);
    }
  else
    {
      DGELSY_F77(&m, &n, &nrhs, &(*this)[0], &m, &B[0], &p, &jpiv[0], &rcond, &rank, &lwork_d, &lwork, &info);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGELSY_F77(&m, &n, &nrhs, &(*this)[0], &m, &B[0], &p, &jpiv[0], &rcond, &rank, &work[0], &lwork, &info);
    }
  MatrixImplementation result(n, q);
  for(UnsignedLong j = 0; j < static_cast<UnsignedLong>(q); ++j)
    for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i)
      result(i, j) = B(i, j);
  return result;
}

/* Resolution of a linear system : rectangular matrix
 * Mx = b, M is an mxn matrix, b is an m-dimensional
 * vector and x is an n-dimensional vector */
NumericalPoint MatrixImplementation::solveLinearSystemRect (const NumericalPoint & b,
                                                            const Bool keepIntact)
{
  const UnsignedLong m(b.getDimension());
  if (nbRows_ != m) throw InvalidDimensionException(HERE);
  if (nbRows_ == 0) throw InvalidDimensionException(HERE);
  // Solve the matrix linear system
  // A MatrixImplementation is also a collection of NumericalScalar, so it is automatically converted into a NumericalPoint
  return solveLinearSystemRect(MatrixImplementation(m, 1, b), keepIntact);
}

/* Resolution of a linear system : square matrix */
MatrixImplementation MatrixImplementation::solveLinearSystemSquare (const MatrixImplementation & b,
                                                                    const Bool keepIntact)
{
  if (nbColumns_ != b.nbRows_ ) throw InvalidDimensionException(HERE);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (b.nbColumns_ == 0)) throw InvalidDimensionException(HERE);

  // We must copy the right-hand side because it will be overwritten by the operation
  MatrixImplementation B(b);
  int m(nbRows_);
  int nrhs(B.nbColumns_);
  int info;
  std::vector<int> ipiv(m);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DGESV_F77(&m, &nrhs, &A[0], &m, &ipiv[0], &B[0], &m, &info);
    }
  else DGESV_F77(&m, &nrhs, &(*this)[0], &m, &ipiv[0], &B[0], &m, &info);
  return B;
}

/* Resolution of a linear system : square matrix */
NumericalPoint MatrixImplementation::solveLinearSystemSquare (const NumericalPoint & b,
                                                              const Bool keepIntact)
{
  const UnsignedLong m(b.getDimension());
  if (nbRows_ != m) throw InvalidDimensionException(HERE);
  if (nbRows_ == 0) throw InvalidDimensionException(HERE);
  // A MatrixImplementation is also a collection of NumericalScalar, so it is automatically converted into a NumericalPoint
  return solveLinearSystemRect(MatrixImplementation(m, 1, b), keepIntact);
}

/* Resolution of a linear system : symmetric matrix */
MatrixImplementation MatrixImplementation::solveLinearSystemSym (const MatrixImplementation & b,
                                                                 const Bool keepIntact)
{
  if (nbColumns_ != b.nbRows_ ) throw InvalidDimensionException(HERE);
  if ((nbColumns_ == 0) || (b.nbColumns_ == 0)) throw InvalidDimensionException(HERE);

  char uplo('L');
  // We must copy the right-hand side as it will be overwritten by the operation
  MatrixImplementation B(b);
  int n(nbRows_);
  int nrhs(B.nbColumns_);
  int lwork(-1);
  double lwork_d;
  int info;
  std::vector<int> ipiv(n);
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DSYSV_F77(&uplo, &n, &nrhs, &A[0], &n, &ipiv[0], &B[0], &n, &lwork_d, &lwork, &info, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYSV_F77(&uplo, &n, &nrhs, &A[0], &n, &ipiv[0], &B[0], &n, &work[0], &lwork, &info, &luplo);
    }
  else
    {
      DSYSV_F77(&uplo, &n, &nrhs, &(*this)[0], &n, &ipiv[0], &B[0], &n, &lwork_d, &lwork, &info, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYSV_F77(&uplo, &n, &nrhs, &(*this)[0], &n, &ipiv[0], &B[0], &n, &work[0], &lwork, &info, &luplo);
    }
  return B;
}

/* Resolution of a linear system : symmetric matrix */
NumericalPoint MatrixImplementation::solveLinearSystemSym (const NumericalPoint & b,
                                                           const Bool keepIntact)
{
  const UnsignedLong dimension(b.getDimension());
  if (nbRows_ != dimension) throw InvalidDimensionException(HERE);
  if (nbRows_ == 0) throw InvalidDimensionException(HERE);
  MatrixImplementation B(dimension, 1, b);
  // A MatrixImplementation is also a collection of NumericalScalar, so it is automatically converted into a NumericalPoint
  return solveLinearSystemSym(B, keepIntact);
}

/* Resolution of a linear system : covariance matrix */
MatrixImplementation MatrixImplementation::solveLinearSystemCov (const MatrixImplementation & b,
                                                                 const Bool keepIntact)
{
  if (nbColumns_ != b.nbRows_ ) throw InvalidDimensionException(HERE);

  char uplo('L');
  // We must copy the right-hand side as it will be overwritten by the operation
  MatrixImplementation B(b);
  int n(nbRows_);
  int nrhs(B.nbColumns_);
  int info;
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DPOSV_F77(&uplo, &n, &nrhs, &A[0], &n, &B[0], &n, &info, &luplo);
    }
  else
    {
      DPOSV_F77(&uplo, &n, &nrhs, &(*this)[0], &n, &B[0], &n, &info, &luplo);
    }
  return B;
}

/* Resolution of a linear system : symmetric matrix */
NumericalPoint MatrixImplementation::solveLinearSystemCov (const NumericalPoint & b,
                                                           const Bool keepIntact)
{
  const UnsignedLong dimension(b.getDimension());
  MatrixImplementation B(dimension, 1, b);
  // A MatrixImplementation is also a collection of NumericalScalar, so it is automatically converted into a NumericalPoint
  return solveLinearSystemCov(B, keepIntact);
}

/* Compute determinant */
NumericalScalar MatrixImplementation::computeLogAbsoluteDeterminant (NumericalScalar & sign,
                                                                     const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  NumericalScalar logAbsoluteDeterminant(0.0);
  sign = 1.0;
  if (n <= 2)
    {
      NumericalScalar value(0.0);
      if (n == 1) value = (*this)(0, 0);
      else value = (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
      if (value == 0.0)
        {
          sign = 0.0;
          logAbsoluteDeterminant = -SpecFunc::MaxNumericalScalar;
        }
      else
        {
          sign = (value > 0.0 ? 1.0 : -1.0);
          logAbsoluteDeterminant = log(fabs(value));
        }
    } // n <= 2
  else
    {
      std::vector<int> ipiv (n);
      int info;

      // LU Factorization with LAPACK
      if (keepIntact)
        {
          MatrixImplementation A(*this);
          DGETRF_F77(&n, &n, &A[0], &n, &ipiv[0], &info);
          // Determinant computation
          for (UnsignedLong i = 0; i < ipiv.size(); ++i)
            {
              const NumericalScalar pivot(A[i * (ipiv.size() + 1)]);
              if (fabs(pivot) == 0.0)
                {
                  logAbsoluteDeterminant = -SpecFunc::MaxNumericalScalar;
                  sign = 0.0;
                }
              if (logAbsoluteDeterminant > -SpecFunc::MaxNumericalScalar) logAbsoluteDeterminant += log(fabs(pivot));
              if (pivot < 0.0) sign = -sign;
              if (ipiv[i] != int(i + 1)) sign = -sign;
            }
        } // keepIntact true
      else
        {
          DGETRF_F77(&n, &n, &(*this)[0], &n, &ipiv[0], &info);
          // Determinant computation
          for (UnsignedLong i = 0; i < ipiv.size(); ++i)
            {
              const NumericalScalar pivot((*this)[i * (ipiv.size() + 1)]);
              if (fabs(pivot) == 0.0)
                {
                  logAbsoluteDeterminant = -SpecFunc::MaxNumericalScalar;
                  sign = 0.0;
                }
              if (logAbsoluteDeterminant > -SpecFunc::MaxNumericalScalar) logAbsoluteDeterminant += log(fabs(pivot));
              if (pivot < 0.0) sign = -sign;
              if (ipiv[i] != int(i + 1)) sign = -sign;
            }
        } // keepIntact false
    } // n > 2
  return logAbsoluteDeterminant;
}

/* Compute determinant */
NumericalScalar MatrixImplementation::computeDeterminant (const Bool keepIntact)
{
  if (nbRows_ == 1) return (*this)(0, 0);
  if (nbRows_ == 2) return (*this)(0, 0) * (*this)(1, 1) - (*this)(0, 1) * (*this)(1, 0);
  NumericalScalar sign(0.0);
  const NumericalScalar logAbsoluteDeterminant(computeLogAbsoluteDeterminant(sign, keepIntact));
  return sign * exp(logAbsoluteDeterminant);
}

/* Compute determinant for a symmetric matrix */
NumericalScalar MatrixImplementation::computeLogAbsoluteDeterminantSym (NumericalScalar & sign,
                                                                        const Bool keepIntact)
{
  symmetrize();
  return computeLogAbsoluteDeterminant(sign, keepIntact);
  /* The implementation based on dsytrf does not uses the 2x2 diagonal blocks correctly
     int n(nbRows_);
     if (n == 0) throw InvalidDimensionException(HERE);
     std::vector<int> ipiv (n);
     char uplo('L');
     int info;
     NumericalScalar determinant(1.0);
     int lwork(-1);
     double lwork_d;
     int luplo(1);

     // LU Factorization with LAPACK
     if (keepIntact)
     {
     MatrixImplementation A(*this);
     DSYTRF_F77(&uplo, &n, &A[0], &n, &ipiv[0],&lwork_d, &lwork, &info, &luplo);
     lwork = static_cast<int>(lwork_d);
     NumericalPoint work(lwork);
     DSYTRF_F77(&uplo, &n, &A[0], &n, &ipiv[0],&work[0], &lwork, &info, &luplo);
     // Determinant computation
     for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i)
     {
     determinant *= A[i * (ipiv.size() + 1)];
     if (ipiv[i] != int(i + 1)) determinant = -determinant;
     }
     }
     else
     {
     DSYTRF_F77(&uplo, &n, &(*this)[0], &n, &ipiv[0],&lwork_d, &lwork, &info, &luplo);
     lwork = static_cast<int>(lwork_d);
     NumericalPoint work(lwork);
     DSYTRF_F77(&uplo, &n, &(*this)[0], &n, &ipiv[0],&work[0], &lwork, &info, &luplo);
     // Determinant computation
     for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i)
     {
     determinant *= (*this)[i * (ipiv.size() + 1)];
     if (ipiv[i] != int(i + 1)) determinant = -determinant;
     }
     }

     return determinant; */
}

/* Compute determinant for a symmetric matrix */
NumericalScalar MatrixImplementation::computeDeterminantSym (const Bool keepIntact)
{
  if (nbRows_ == 1) return (*this)(0, 0);
  if (nbRows_ == 2) return (*this)(0, 0) * (*this)(1, 1) - (*this)(1, 0) * (*this)(1, 0);
  NumericalScalar sign(0.0);
  const NumericalScalar logAbsoluteDeterminant(computeLogAbsoluteDeterminant(sign, keepIntact));
  return sign * exp(logAbsoluteDeterminant);
}


/* Compute the eigenvalues of a square matrix */
MatrixImplementation::NumericalComplexCollection MatrixImplementation::computeEigenValuesSquare (const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  char jobvl('N');
  char jobvr('N');
  NumericalPoint wr(n, 0.0);
  NumericalPoint wi(n, 0.0);
  double vl;
  double vr;
  int ldvl(1);
  int ldvr(1);
  int lwork(-1);
  double lwork_d;
  int info;
  int ljobvl(1);
  int ljobvr(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DGEEV_F77(&jobvl, &jobvr, &n, &A[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr, &ldvr, &lwork_d, &lwork, &info, &ljobvl, &ljobvr);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGEEV_F77(&jobvl, &jobvr, &n, &A[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr, &ldvr, &work[0], &lwork, &info, &ljobvl, &ljobvr);
    }
  else
    {
      DGEEV_F77(&jobvl, &jobvr, &n, &(*this)[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr, &ldvr, &lwork_d, &lwork, &info, &ljobvl, &ljobvr);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGEEV_F77(&jobvl, &jobvr, &n, &(*this)[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr, &ldvr, &work[0], &lwork, &info, &ljobvl, &ljobvr);
    }
  NumericalComplexCollection eigenValues(n);
  for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i) eigenValues[i] = NumericalComplex(wr[i], wi[i]);
  return eigenValues;
}

MatrixImplementation::NumericalComplexCollection MatrixImplementation::computeEigenValuesSquare (ComplexMatrixImplementation & v,
                                                                                                 const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  char jobvl('N');
  char jobvr('V');
  NumericalPoint wr(n, 0.0);
  NumericalPoint wi(n, 0.0);
  double vl;
  MatrixImplementation vr(n, n);
  int ldvl(1);
  int ldvr(n);
  int lwork(-1);
  double lwork_d;
  int info;
  int ljobvl(1);
  int ljobvr(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DGEEV_F77(&jobvl, &jobvr, &n, &A[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr[0], &ldvr, &lwork_d, &lwork, &info, &ljobvl, &ljobvr);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGEEV_F77(&jobvl, &jobvr, &n, &A[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr[0], &ldvr, &work[0], &lwork, &info, &ljobvl, &ljobvr);
    }
  else
    {
      DGEEV_F77(&jobvl, &jobvr, &n, &(*this)[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr[0], &ldvr, &lwork_d, &lwork, &info, &ljobvl, &ljobvr);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DGEEV_F77(&jobvl, &jobvr, &n, &(*this)[0], &n, &wr[0], &wi[0], &vl, &ldvl, &vr[0], &ldvr, &work[0], &lwork, &info, &ljobvl, &ljobvr);
    }
  // Cast the eigenvalues into OpenTURNS data structures
  NumericalComplexCollection eigenValues(n);
  for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i)
    {
      eigenValues[i] = NumericalComplex(wr[i], wi[i]);
    }
  // Cast the eigenvectors into OpenTURNS data structures
  v = ComplexMatrixImplementation(n, n);
  UnsignedLong j(0);
  while (j < static_cast<UnsignedLong>(n))
    {
      // Real eigenvalue
      if (wi[j] == 0.0)
        {
          for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i) v(i, j) = vr(i, j);
          ++j;
        }
      // Complex conjugate pair of eigenvalues
      else
        {
          for (UnsignedLong i = 0; i < static_cast<UnsignedLong>(n); ++i)
            {
              v(i, j)     = NumericalComplex(vr(i, j),  vr(i, j + 1));
              v(i, j + 1) = NumericalComplex(vr(i, j), -vr(i, j + 1));
            }
          j += 2;
        }
    } // Conversion of eigenvectors
  return eigenValues;
}

/* Compute the eigenvalues of a symmetric matrix */
NumericalPoint MatrixImplementation::computeEigenValuesSym (const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  char jobz('N');
  char uplo('L');
  NumericalPoint w(n, 0.0);
  int lwork(-1);
  double lwork_d;
  int info;
  int ljobz(1);
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DSYEV_F77(&jobz, &uplo, &n, &A[0], &n, &w[0], &lwork_d, &lwork, &info, &ljobz, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYEV_F77(&jobz, &uplo, &n, &A[0], &n, &w[0], &work[0], &lwork, &info, &ljobz, &luplo);
    }
  else
    {
      DSYEV_F77(&jobz, &uplo, &n, &(*this)[0], &n, &w[0], &lwork_d, &lwork, &info, &ljobz, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYEV_F77(&jobz, &uplo, &n, &(*this)[0], &n, &w[0], &work[0], &lwork, &info, &ljobz, &luplo);
    }
  return w;
}

NumericalPoint MatrixImplementation::computeEigenValuesSym (MatrixImplementation & v,
                                                            const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  char jobz('V');
  char uplo('L');
  NumericalPoint w(n, 0.0);
  int lwork(-1);
  double lwork_d;
  int info;
  int ljobz(1);
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DSYEV_F77(&jobz, &uplo, &n, &A[0], &n, &w[0], &lwork_d, &lwork, &info, &ljobz, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYEV_F77(&jobz, &uplo, &n, &A[0], &n, &w[0], &work[0], &lwork, &info, &ljobz, &luplo);
      v = A;
    }
  else
    {
      DSYEV_F77(&jobz, &uplo, &n, &(*this)[0], &n, &w[0], &lwork_d, &lwork, &info, &ljobz, &luplo);
      lwork = static_cast<int>(lwork_d);
      NumericalPoint work(lwork);
      DSYEV_F77(&jobz, &uplo, &n, &(*this)[0], &n, &w[0], &work[0], &lwork, &info, &ljobz, &luplo);
      v = *this;
    }
  return w;
}

/* Compute the singular values of a matrix */
NumericalPoint MatrixImplementation::computeSingularValues(const Bool keepIntact)
{
  int m(nbRows_);
  int n(nbColumns_);
  if ((m == 0) || (n == 0)) throw InvalidDimensionException(HERE);
  char jobz('N');
  NumericalPoint S(std::min(m, n), 0.0);
  NumericalPoint work(1, 0.0);
  MatrixImplementation u(1, 1);
  int ldu(1);
  int ldvt(1);
  MatrixImplementation vT(1, 1);
  int lwork(-1);
  std::vector<int> iwork(8 * std::min(m, n));
  int info(0);
  int ljobz(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      // First call to compute the optimal work size
      DGESDD_F77(&jobz, &m, &n, &A[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
      lwork = static_cast<int>(work[0]);
      work = NumericalPoint(lwork, 0.0);
      // Second call to compute the SVD
      DGESDD_F77(&jobz, &m, &n, &A[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
    }
  else
    {
      // First call to compute the optimal work size
      DGESDD_F77(&jobz, &m, &n, &(*this)[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
      lwork = static_cast<int>(work[0]);
      work = NumericalPoint(lwork, 0.0);
      // Second call to compute the SVD
      DGESDD_F77(&jobz, &m, &n, &(*this)[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
    }
  return S;
}

/* Compute the singular values and singular decomposition of a matrix */
NumericalPoint MatrixImplementation::computeSingularValues(MatrixImplementation & u,
                                                           MatrixImplementation & vT,
                                                           const Bool fullSVD,
                                                           const Bool keepIntact)
{
  int m(nbRows_);
  int n(nbColumns_);
  if ((m == 0) || (n == 0)) throw InvalidDimensionException(HERE);
  char jobz( fullSVD ? 'A' : 'S');
  MatrixImplementation A(*this);
  int ldu(m);
  u = MatrixImplementation(m, ( fullSVD ? m : std::min(m, n)));
  int ldvt(n);
  vT = MatrixImplementation(( fullSVD ? n : std::min(m, n)), n);
  NumericalPoint S(std::min(m, n), 0.0);
  NumericalPoint work(1, 0.0);
  int lwork(-1);
  std::vector<int> iwork(8 * std::min(m, n));
  int info(0);
  int ljobz(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      // First call to compute the optimal work size
      DGESDD_F77(&jobz, &m, &n, &A[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
      lwork = static_cast<int>(work[0]);
      work = NumericalPoint(lwork, 0.0);
      // Second call to compute the SVD
      DGESDD_F77(&jobz, &m, &n, &A[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
    }
  else
    {
      // First call to compute the optimal work size
      DGESDD_F77(&jobz, &m, &n, &(*this)[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
      lwork = static_cast<int>(work[0]);
      work = NumericalPoint(lwork, 0.0);
      // Second call to compute the SVD
      DGESDD_F77(&jobz, &m, &n, &(*this)[0], &m, &S[0], &u[0], &ldu, &vT[0], &ldvt, &work[0], &lwork, &iwork[0], &info, &ljobz);
    }
  return S;
}


/* Check if the matrix is SPD */
Bool MatrixImplementation::isPositiveDefinite(const Bool keepIntact)
{
  int info;
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  char uplo('L');
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DPOTRF_F77(&uplo, &n, &A[0], &n, &info, &luplo);
    }
  else DPOTRF_F77(&uplo, &n, &(*this)[0], &n, &info, &luplo);
  return (info == 0) ;
}

/* Build the Cholesky factorization of the matrix */
MatrixImplementation MatrixImplementation::computeCholesky(const Bool keepIntact)
{
  int n(nbRows_);
  if (n == 0) throw InvalidDimensionException(HERE);
  int info;
  char uplo('L');
  int luplo(1);
  if (keepIntact)
    {
      MatrixImplementation A(*this);
      DPOTRF_F77(&uplo, &n, &A[0], &n, &info, &luplo);
      for (UnsignedLong j = 0; j < (UnsignedLong)(n); ++j)
        for (UnsignedLong i = 0; i < j; ++i)
          A(i, j) = 0.0;
      // Check return code from Lapack
      if(info != 0)
        throw InvalidArgumentException(HERE) << " Error - Matrix is not positive definite" ;
      A.setName(DefaultName);
      return A;
    }
  else
    {
      DPOTRF_F77(&uplo, &n, &(*this)[0], &n, &info, &luplo);
      for (UnsignedLong j = 0; j < (UnsignedLong)(n); ++j)
        for (UnsignedLong i = 0; i < (UnsignedLong)(j); ++i)
          (*this)(i, j) = 0.0;
      // Check if return code is ok
      if(info != 0)
        throw InvalidArgumentException(HERE) << " Error - Matrix is not positive definite" ;
      setName(DefaultName);
      return (*this);
    }
}

/* Method save() stores the object through the StorageManager */
void MatrixImplementation::save(Advocate & adv) const
{
  PersistentCollection<NumericalScalar>::save(adv);
  adv.saveAttribute( "nbRows_",    nbRows_);
  adv.saveAttribute( "nbColumns_", nbColumns_);
}

/* Method load() reloads the object from the StorageManager */
void MatrixImplementation::load(Advocate & adv)
{
  PersistentCollection<NumericalScalar>::load(adv);

  adv.loadAttribute( "nbRows_",    nbRows_);
  adv.loadAttribute( "nbColumns_", nbColumns_);
}

const NumericalScalar* MatrixImplementation::__baseaddress__() const
{
  return &(*this)[0];
}

UnsignedLong MatrixImplementation::__elementsize__() const
{
  return sizeof(NumericalScalar);
}

UnsignedLong MatrixImplementation::__stride__(UnsignedLong dim) const
{
  UnsignedLong stride = __elementsize__();
  if (dim > 0)
    stride *= nbRows_;
  return stride;
}


END_NAMESPACE_OPENTURNS

