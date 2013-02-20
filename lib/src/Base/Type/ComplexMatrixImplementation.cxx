//                                               -*- C++ -*-
/**
 *  @file  ComplexMatrixImplementation.cxx
 *  @brief ComplexMatrixImplementation implements the classical mathematicalComplexMatrixImplementation
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
#include <cstdlib>

#include "ComplexMatrixImplementation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Lapack.hxx"
#include "ResourceMap.hxx"
#include "Os.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ComplexMatrixImplementation);


static Factory<ComplexMatrixImplementation> RegisteredFactory("ComplexMatrixImplementation");

/* Default constructor */
ComplexMatrixImplementation::ComplexMatrixImplementation()
  : PersistentCollection<NumericalComplex>(),
    nbRows_(0),
    nbColumns_(0)
{
  // Nothing to do
}

/* Constructor with size (rowDim and colDim) */
/* The ComplexMatrixImplementation is made up of a collection of rowDim*colDim elements */
/* The ComplexMatrixImplementation is viewed as a set of column vectors read one after another */
ComplexMatrixImplementation::ComplexMatrixImplementation(const UnsignedLong rowDim,
                                                         const UnsignedLong colDim)
  : PersistentCollection<NumericalComplex>(rowDim * colDim, NumericalComplex(0.0, 0.0)),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  // Nothing to do
}

/* Constructor from external collection */
ComplexMatrixImplementation::ComplexMatrixImplementation(const UnsignedLong rowDim,
                                                         const UnsignedLong colDim,
                                                         const Collection<NumericalComplex> & elementsValues)
  : PersistentCollection<NumericalComplex>(rowDim * colDim, NumericalComplex(0.0, 0.0)),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  const UnsignedLong matrixSize(std::min(rowDim * colDim, elementsValues.getSize()));
  for(UnsignedLong i = 0; i < matrixSize; ++i) operator[](i) = elementsValues[i];
}

/* Constructor from external collection */
ComplexMatrixImplementation::ComplexMatrixImplementation(const UnsignedLong rowDim,
                                                         const UnsignedLong colDim,
                                                         const Collection<NumericalScalar> & elementsValues)
  : PersistentCollection<NumericalComplex>(rowDim * colDim, NumericalComplex(0.0, 0.0)),
    nbRows_(rowDim),
    nbColumns_(colDim)
{
  const UnsignedLong matrixSize(std::min(rowDim * colDim, elementsValues.getSize()));
  //  Implicit cast from NumericalScalar into NumericalComplex
  for(UnsignedLong i = 0; i < matrixSize; ++i) operator[](i) = elementsValues[i];
}


ComplexMatrixImplementation::ComplexMatrixImplementation(const MatrixImplementation & matrix)
  : PersistentCollection<NumericalComplex>(matrix.getNbRows() * matrix.getNbColumns(), 0.0),
    nbRows_(matrix.getNbRows()),
    nbColumns_(matrix.getNbColumns())
{
  // Copy from matrix
  const UnsignedLong matrixSize(nbRows_ * nbColumns_);
  for(UnsignedLong i = 0; i < matrixSize; ++i) operator[](i) = matrix[i];
}

/* Virtual constructor */
ComplexMatrixImplementation * ComplexMatrixImplementation::clone() const
{
  return new ComplexMatrixImplementation(*this);
}

/* Set small elements to zero */
ComplexMatrixImplementation ComplexMatrixImplementation::clean(const NumericalScalar threshold) const
{
  ComplexMatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      {
        const NumericalComplex value(this->operator[](convertPosition(i, j)));
        NumericalScalar realPart(std::real(value));
        NumericalScalar imagPart(std::imag(value));
        if (fabs(realPart) <= threshold) realPart = 0.0;
        if (fabs(imagPart) <= threshold) imagPart = 0.0;
        result(i, j) = NumericalComplex(realPart, imagPart);
      }
  return result;
}

/* Set small elements to zero */
ComplexMatrixImplementation ComplexMatrixImplementation::cleanHerm(const NumericalScalar threshold) const
{
  ComplexMatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j; i < nbRows_; ++i)
      {
        const NumericalComplex value(this->operator[](convertPosition(i, j)));
        NumericalScalar realPart(std::real(value));
        NumericalScalar imagPart(std::imag(value));
        if (fabs(realPart) <= threshold) realPart = 0.0;
        if (fabs(imagPart) <= threshold) imagPart = 0.0;
        result(i, j) = NumericalComplex(realPart, imagPart);
      }
  return result;
}

/* String converter */
String ComplexMatrixImplementation::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " name=" << getName()
               << " rows=" << nbRows_
               << " columns=" << nbColumns_
               << " values=" << PersistentCollection<NumericalComplex>::__repr__();
}

String ComplexMatrixImplementation::__str__(const String & offset) const
{
  OSS oss;
  // Special case for empty matrix
  if (nbRows_ == 0 || nbColumns_ == 0) return oss;
  // Special case for matrix with 1 row
  if (nbRows_ == 1)
    {
      oss << offset << "(";
      for (UnsignedLong j = 0; j < nbColumns_; ++j)
        oss << (j > 0 ? " " : "") << operator()(0, j);
      oss << ")" << Os::GetEndOfLine();
      return oss;
    }
  // Array for the widths of the different components
  Collection<UnsignedLong> widthColumns(nbColumns_, 0);
  // First loop across the values to determine the optimal width for printing
  for (UnsignedLong i = 0; i < nbRows_; ++i)
    {
      for (UnsignedLong j = 0; j < nbColumns_; ++j)
        widthColumns[j] = std::max(widthColumns[j], static_cast<UnsignedLong>(String(OSS() << operator()(i, j)).length()));
    }
  // Second loop
  for (UnsignedLong i = 0; i < nbRows_; ++i)
    {
      oss << (i == 0 ? "/" : (i == nbRows_ - 1 ? "\\" : "|"));
      for (UnsignedLong j = 0; j < nbColumns_; ++j)
        oss << (j > 0 ? " " : "") << std::setw(widthColumns[j]) << operator()(i, j);
      if (i == 0) oss << "\\" << Os::GetEndOfLine();
      else if (i == nbRows_ - 1) oss << "/";
      else oss << "|" << Os::GetEndOfLine();
    }
  return oss;
}

/* Operator () gives access to the elements of theComplexMatrixImplementation (to modify these elements) */
/* The element of theComplexMatrixImplementation is designated by its row number i and its column number j */
/* the first element of theComplexMatrixImplementation is m(0,0) */
NumericalComplex & ComplexMatrixImplementation::operator () (const UnsignedLong i,
                                                             const UnsignedLong j)
{
  if ((i >= nbRows_) || (j >= nbColumns_)) throw InvalidDimensionException(HERE);

  return operator[](convertPosition(i, j));
}

/* Operator () gives access to the elements of theComplexMatrixImplementation (read only) */
/* The element of theComplexMatrixImplementation is designated by its row number i and its column number j */
const NumericalComplex & ComplexMatrixImplementation::operator () (const UnsignedLong i,
                                                                   const UnsignedLong j)  const
{
  if ((i >= nbRows_) || (j >= nbColumns_)) throw InvalidDimensionException(HERE);

  return operator[](convertPosition(i, j));
}


/* Get the dimensions of the ComplexMatrixImplementation : number of rows */
const UnsignedLong ComplexMatrixImplementation::getNbRows() const
{
  return nbRows_;
}

/* Get the dimensions of the ComplexMatrixImplementation : number of columns */
const UnsignedLong ComplexMatrixImplementation::getNbColumns() const
{
  return nbColumns_;
}

/* Get the dimensions of the ComplexMatrixImplementation : dimension (square matrix : nbRows_) */
const UnsignedLong ComplexMatrixImplementation::getDimension() const
{
  return nbRows_;
}

/* ComplexMatrixImplementation transpose */
ComplexMatrixImplementation ComplexMatrixImplementation::transpose () const
{
  ComplexMatrixImplementation trans(nbColumns_, nbRows_);
  // The source matrix is accessed columnwise in the natural order
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      trans(j, i) = operator()(i, j);
  return trans;
}

/* ComplexMatrixImplementation transpose
 * for hermitian matrices. It reduces to copy the upper triangle into
 * the lower triangle, which means to conjugate the lower triangle */
ComplexMatrixImplementation ComplexMatrixImplementation::transposeHerm () const
{
  ComplexMatrixImplementation result(nbColumns_, nbRows_);
  // The lower triangle of the source matrix is accessed columnwise in the natural order
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j; i < nbRows_; ++i)
      result(i, j) = std::conj(operator()(i, j));
  return result;
}

/* ComplexMatrixImplementation conjugate */
ComplexMatrixImplementation ComplexMatrixImplementation::conjugate () const
{
  ComplexMatrixImplementation result(nbRows_, nbColumns_);
  // The source matrix is accessed columnwise in the natural order
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      result(i, j) = std::conj(operator()(i, j));
  return result;
}

/* ComplexMatrixImplementation conjugate triangular */
ComplexMatrixImplementation ComplexMatrixImplementation::conjugateHerm () const
{
  return transposeHerm();
}

/* ComplexMatrixImplementation conjugateTranspose */
ComplexMatrixImplementation ComplexMatrixImplementation::conjugateTranspose () const
{
  ComplexMatrixImplementation result(nbColumns_, nbRows_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      result(j, i) = std::conj(operator()(i, j));
  return result;
}

/* Hermitianize ComplexMatrixImplementation in case it is an hermitian matrix (stored as a triangular matrix) */
void ComplexMatrixImplementation::hermitianize() const
{
  ComplexMatrixImplementation *refThis(const_cast<ComplexMatrixImplementation *>(this));
  // The lower triangle of the source matrix is accessed columnwise in the natural order
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j + 1; i < nbRows_; ++i)
      refThis->operator[](convertPosition(j, i)) = std::conj(operator[](convertPosition(i, j)));
}


/* Get the real part of the matrix */
MatrixImplementation ComplexMatrixImplementation::realRect() const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      result(i, j) = (*this)(i, j).real();
  return result;
}

MatrixImplementation ComplexMatrixImplementation::realSym() const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j; i < nbRows_; ++i)
      result(i, j) = (*this)(i, j).real();
  return result;
}

/* Get the imaginary part of the matrix */
MatrixImplementation ComplexMatrixImplementation::imagRect() const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = 0; i < nbRows_; ++i)
      result(i, j) = (*this)(i, j).imag();
  return result;
}

MatrixImplementation ComplexMatrixImplementation::imagSym() const
{
  MatrixImplementation result(nbRows_, nbColumns_);
  for (UnsignedLong j = 0; j < nbColumns_; ++j)
    for (UnsignedLong i = j; i < nbRows_; ++i)
      result(i, j) = (*this)(i, j).imag();
  return result;
}


/* Empty returns true if there is no element in theComplexMatrixImplementation */
const Bool ComplexMatrixImplementation::isEmpty() const
{
  return ((nbRows_ == 0)  || (nbColumns_ == 0) || (PersistentCollection<NumericalComplex>::isEmpty()));
}

/* Returns true if triangular lower or upper */
const Bool ComplexMatrixImplementation::isTriangular(Bool lower) const
{
  if ( nbRows_ == nbColumns_ )
    {
      for ( UnsignedLong j = 1; j < nbColumns_; ++ j )
        for ( UnsignedLong i = 0; i < j; ++ i )
          if ( std::abs( (*this)[lower ?  convertPosition(i, j) : convertPosition(j, i)] ) > 0. )
            return false;
      return true;
    }
  else
    return false;
}

/* ComplexMatrixImplementation addition (must have the same dimensions) */
ComplexMatrixImplementation ComplexMatrixImplementation::operator + (const ComplexMatrixImplementation & matrix) const
{
  if ((nbRows_ != matrix.nbRows_ ) || (nbColumns_ != matrix.nbColumns_ )) throw InvalidDimensionException(HERE);
  // Must copy as add will be overwritten by the operation
  ComplexMatrixImplementation add(matrix);
  int size(nbRows_ * nbColumns_);
  NumericalComplex alpha(1.0, 0.0);
  int one(1);
  // Lapack routine
  ZAXPY_F77(&size, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &one, &add[0], &one);

  return add;
}

/* ComplexMatrixImplementation addition (must have the same dimensions) */
ComplexMatrixImplementation ComplexMatrixImplementation::operator + (const MatrixImplementation & matrix) const
{
  if ((nbRows_ != matrix.getNbRows() ) || (nbColumns_ != matrix.getNbColumns() )) throw InvalidDimensionException(HERE);
  // Must copy as add will be overwritten by the operation
  ComplexMatrixImplementation add(matrix);
  int size(nbRows_ * nbColumns_);
  NumericalComplex alpha(1.0, 0.0);
  int one(1);
  // Lapack routine
  ZAXPY_F77(&size, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &one, &add[0], &one);

  return add;
}

/* ComplexMatrixImplementation substraction (must have the same dimensions) */
ComplexMatrixImplementation ComplexMatrixImplementation::operator - (const ComplexMatrixImplementation & matrix) const
{
  if ((nbRows_ != matrix.nbRows_ ) || (nbColumns_ != matrix.nbColumns_ )) throw InvalidDimensionException(HERE);
  // Must copy as add will be overwritten by the operation
  ComplexMatrixImplementation sub(*this);
  int size(nbRows_ * nbColumns_);
  NumericalComplex alpha(-1.0, 0.0);
  int one(1);

  ZAXPY_F77(&size, &alpha, const_cast<std::complex<double>*>(&(matrix)[0]), &one, &sub[0], &one);

  return sub;
}

/* ComplexMatrixImplementation substraction (must have the same dimensions) */
ComplexMatrixImplementation ComplexMatrixImplementation::operator - (const MatrixImplementation & matrix) const
{
  return operator - (ComplexMatrixImplementation(matrix));
}

/* Multiplication with a NumericalComplex */
ComplexMatrixImplementation ComplexMatrixImplementation::operator * (const NumericalComplex s) const
{
  // Check if s is null
  if (std::abs(s) == 0.0 ) return ComplexMatrixImplementation(nbRows_, nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return *this;
  ComplexMatrixImplementation scalprod(*this);

  //
  NumericalComplex alpha(s);
  int one(1);
  int n_(nbRows_ * nbColumns_);
  // Lapack routine
  ZSCAL_F77(&n_, &alpha, &scalprod[0], &one);

  return scalprod;
}

/* Division by a NumericalScalar*/
ComplexMatrixImplementation ComplexMatrixImplementation::operator / (const NumericalComplex s) const
{
  if (std::abs(s) == 0) throw InvalidArgumentException(HERE) ;

  return operator * (1.0 / s);
}

/* ComplexMatrixImplementation multiplications (must have consistent dimensions) */
ComplexMatrixImplementation ComplexMatrixImplementation::genProd(const ComplexMatrixImplementation & matrix) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in complex matrix/ complex matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;

  ComplexMatrixImplementation mult(nbRows_, matrix.nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char transa('N');
  char transb('N');
  int m(nbRows_);
  int k(nbColumns_);
  int n(matrix.nbColumns_);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int ltransa(1);
  int ltransb(1);

  //Lapack routine
  ZGEMM_F77(&transa, &transb, &m, &n, &k, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m, const_cast<std::complex<double>*>(&(matrix[0])), &k, &beta, &mult[0], &m, &ltransa, &ltransb);
  return mult;
}

ComplexMatrixImplementation ComplexMatrixImplementation::symProd (const ComplexMatrixImplementation & matrix,
                                                                  const char hermSide) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in matrix/matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;

  ComplexMatrixImplementation mult(nbRows_, matrix.nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char side(hermSide);
  char uplo('L');
  int m(nbRows_);
  int k(nbColumns_);
  int n(matrix.nbColumns_);
  NumericalComplex alpha(1.0);
  NumericalComplex beta(0.0);
  int lside(1);
  int luplo(1);

  // Lapack routine
  ZSYMM_F77(&side, &uplo, &m, &n, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m, const_cast<std::complex<double>*>(&(matrix[0])), &k, &beta, &mult[0], &m, &lside, &luplo);

  return mult;
}

ComplexMatrixImplementation ComplexMatrixImplementation::hermProd(const ComplexMatrixImplementation & matrix,
                                                                  const char hermSide) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in matrix/matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;

  ComplexMatrixImplementation mult(nbRows_, matrix.nbColumns_);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char side(hermSide);
  char uplo('L');
  int m(nbRows_);
  int k(nbColumns_);
  int n(matrix.nbColumns_);
  NumericalComplex alpha(1.0);
  NumericalComplex beta(0.0);
  int lside(1);
  int luplo(1);

  // Lapack routine
  if (hermSide == 'L') ZHEMM_F77(&side, &uplo, &m, &n, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m, const_cast<std::complex<double>*>(&(matrix[0])), &k, &beta, &mult[0], &m, &lside, &luplo);
  else ZHEMM_F77(&side, &uplo, &m, &n, &alpha, const_cast<std::complex<double>*>(&(matrix[0])), &k, const_cast<std::complex<double>*>(&((*this)[0])), &m, &beta, &mult[0], &m, &lside, &luplo);

  return mult;
}

ComplexMatrixImplementation ComplexMatrixImplementation::triangularProd(const ComplexMatrixImplementation & matrix,
                                                                        const char triangularSide,
                                                                        const char upperLower) const
{
  if (nbColumns_ != matrix.nbRows_) throw InvalidDimensionException(HERE) << "Invalid dimensions in matrix/matrix product left="
                                                                          << nbRows_ << "x" << nbColumns_
                                                                          << " right=" << matrix.nbRows_ << "x"
                                                                          << matrix.nbColumns_;

  ComplexMatrixImplementation mult(matrix);
  if ((nbRows_ == 0) || (nbColumns_ == 0) || (matrix.nbColumns_ == 0)) return mult;
  char side(triangularSide);
  int lside(1);

  char uplo(upperLower);
  int luplo(1);

  char trans('N');
  int ltrans(1);

  char diag('N');
  int ldiag(1);

  int m(nbRows_);
  int n(matrix.nbColumns_);

  NumericalComplex alpha(1.0);

  // Lapack routine
  ZTRMM_F77(&side, &uplo, &trans, &diag, &m, &n, &alpha , const_cast<std::complex<double>*>(&((*this)[0])),  &m, const_cast<std::complex<double>*>(&(mult[0])), &m, &lside , &luplo, &ltrans,  &ldiag);
  return mult;
}

/* Integer power, general matrix */
ComplexMatrixImplementation ComplexMatrixImplementation::genPower(const UnsignedLong n) const
{
  Bool first(true);
  UnsignedLong exponent(n);
  ComplexMatrixImplementation y;
  ComplexMatrixImplementation z(*this);
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
ComplexMatrixImplementation ComplexMatrixImplementation::symPower(const UnsignedLong n) const
{
  Bool first(true);
  UnsignedLong exponent(n);
  ComplexMatrixImplementation y;
  ComplexMatrixImplementation z(*this);
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

/* Integer power, hermitian matrix */
ComplexMatrixImplementation ComplexMatrixImplementation::hermPower(const UnsignedLong n) const
{
  Bool first(true);
  UnsignedLong exponent(n);
  ComplexMatrixImplementation y;
  ComplexMatrixImplementation z(*this);
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
          else y = y.hermProd(z, 'L');
          // if no more bit to consider
          if (exponent == 0) return y;
        }
      // Square the contribution
      z = z.hermProd(z, 'L');
    }
  return y;
}

/* Multiplications with a NumericalComplexCollection */
ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::genVectProd(const NumericalComplexCollection & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return prod;
  char trans('N');
  int m_(nbRows_);
  int n_(nbColumns_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int ltrans(1);

  ZGEMV_F77(&trans, &m_, &n_, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m_, const_cast<std::complex<double>*>(&(pt[0])), &one, &beta, &prod[0], &one, &ltrans);

  return prod;
}

/* Multiplications with a NumericalScalarCollection */
ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::genVectProd(const NumericalScalarCollection & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return prod;
  NumericalComplexCollection copyPoint(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) copyPoint[i] = pt[i];
  char trans('N');
  int m_(nbRows_);
  int n_(nbColumns_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int ltrans(1);

  ZGEMV_F77(&trans, &m_, &n_, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m_, const_cast<std::complex<double>*>(&(copyPoint[0])), &one, &beta, &prod[0], &one, &ltrans);

  return prod;
}

/* Multiplications with a NumericalPoint */
ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::genVectProd(const NumericalPoint & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  if ((nbRows_ == 0) || (nbColumns_ == 0)) return prod;
  NumericalComplexCollection copyPoint(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) copyPoint[i] = pt[i];
  char trans('N');
  int m_(nbRows_);
  int n_(nbColumns_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int ltrans(1);

  ZGEMV_F77(&trans, &m_, &n_, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &m_, const_cast<std::complex<double>*>(&(copyPoint[0])), &one, &beta, &prod[0], &one, &ltrans);

  return prod;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::hermVectProd(const NumericalComplexCollection & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  // In this case, nbRows_ == nbColumns_
  if (nbRows_ == 0) return prod;
  char uplo('L');
  int n(nbRows_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int luplo(1);
  ZHEMV_F77(&uplo, &n, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &n, const_cast<std::complex<double>*>(&(pt[0])), &one, &beta, &prod[0], &one, &luplo);

  return prod;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::hermVectProd(const NumericalScalarCollection & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  // In this case, nbRows_ == nbColumns_
  if (nbRows_ == 0) return prod;
  NumericalComplexCollection copyPoint(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) copyPoint[i] = pt[i];
  char uplo('L');
  int n(nbRows_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int luplo(1);
  ZHEMV_F77(&uplo, &n, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &n, const_cast<std::complex<double>*>(&(copyPoint[0])), &one, &beta, &prod[0], &one, &luplo);

  return prod;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::hermVectProd(const NumericalPoint & pt) const
{
  if (nbColumns_ != pt.getSize() ) throw InvalidDimensionException(HERE) << "Invalid dimension in matrix/vector product";

  NumericalComplexCollection prod(nbRows_);
  // In this case, nbRows_ == nbColumns_
  if (nbRows_ == 0) return prod;
  NumericalComplexCollection copyPoint(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) copyPoint[i] = pt[i];
  char uplo('L');
  int n(nbRows_);
  int one(1);
  NumericalComplex alpha(1.0, 0.0);
  NumericalComplex beta(0.0, 0.0);
  int luplo(1);
  ZHEMV_F77(&uplo, &n, &alpha, const_cast<std::complex<double>*>(&((*this)[0])), &n, const_cast<std::complex<double>*>(&(copyPoint[0])), &one, &beta, &prod[0], &one, &luplo);

  return prod;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::triangularVectProd(const NumericalComplexCollection & pt,
                                                                                                        const char side) const
{
  // Product matrix vector using Blas
  // side tells if the triangular matrix is "Lower" or "Upper"
  char uplo(side);
  int luplo(1);

  // trans tells if the matrix is transposed or not
  char trans('N');
  int ltrans(1);

  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);

  // the dimension of the matrix
  int n(nbRows_);

  // leading dimension of M
  int lda(nbRows_);
  int one(1);

  NumericalComplexCollection x(pt);
  ZTRMV_F77(&uplo, &trans, &diag, &n, const_cast<std::complex<double>*>(&((*this)[0])), &lda, const_cast<std::complex<double>*>(&(x[0])), &one, &luplo, &ltrans, &ldiag);
  return x;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::triangularVectProd(const NumericalScalarCollection & pt,
                                                                                                        const char side) const
{
  char uplo(side);
  int luplo(1);

  // trans tells if the matrix is transposed or not
  char trans('N');
  int ltrans(1);

  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);

  // the dimension of the matrix
  int n(nbRows_);

  // leading dimension of M
  int lda(nbRows_);
  int one(1);

  NumericalComplexCollection x(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) x[i] = pt[i];

  ZTRMV_F77(&uplo, &trans, &diag, &n, const_cast<std::complex<double>*>(&((*this)[0])), &lda, const_cast<std::complex<double>*>(&(x[0])), &one, &luplo, &ltrans, &ldiag);
  return x;
}

ComplexMatrixImplementation::NumericalComplexCollection ComplexMatrixImplementation::triangularVectProd(const NumericalPoint & pt,
                                                                                                        const char side) const
{
  char uplo(side);
  int luplo(1);

  // trans tells if the matrix is transposed or not
  char trans('N');
  int ltrans(1);

  // diag tells if M is unit diagonal or not
  char diag('N');
  int ldiag(1);

  // the dimension of the matrix
  int n(nbRows_);

  // leading dimension of M
  int lda(nbRows_);
  int one(1);

  NumericalComplexCollection x(nbRows_);
  for (UnsignedLong i = 0; i < pt.getSize(); ++i) x[i] = pt[i];

  ZTRMV_F77(&uplo, &trans, &diag, &n, const_cast<std::complex<double>*>(&((*this)[0])), &lda, const_cast<std::complex<double>*>(&(x[0])), &one, &luplo, &ltrans, &ldiag);
  return x;
}


Bool ComplexMatrixImplementation::isHermitian() const
{
  if ( nbRows_ == nbColumns_ )
    {
      for ( UnsignedLong i = 1; i < nbRows_; ++ i )
        for ( UnsignedLong j = 0; j < i; ++ j )
          if ( operator[](convertPosition(i, j)) != std::conj(operator[](convertPosition(j, i))) )
            return false;
      return true;
    }
  else
    return false;
}


/* Check if the matrix is HPD */
Bool ComplexMatrixImplementation::isHermitianPositiveDefinite(const Bool keepIntact)
{
  // Exception for null dimension
  if (getDimension() == 0) throw InvalidDimensionException(HERE);
  int info;
  int n(nbRows_);
  char uplo('L');
  int luplo(1);
  if (keepIntact)
    {
      ComplexMatrixImplementation A(*this);
      ZPOTRF_F77(&uplo, &n, &A[0], &n, &info, &luplo);
      if (info != 0) throw InternalException(HERE) << "Lapack ZPOTRF: error code=" << info;
    }
  else
    {
      ZPOTRF_F77(&uplo, &n, &(*this)[0], &n, &info, &luplo);
      if (info != 0) throw InternalException(HERE) << "Lapack ZPOTRF: error code=" << info;
    }
  return (info == 0) ;
}

/* Build the Cholesky factorization of the hermitian matrix */
ComplexMatrixImplementation ComplexMatrixImplementation::computeCholesky(const Bool keepIntact)
{
  // Exception for null dimension
  if (getDimension() == 0) throw InvalidDimensionException(HERE);
  int info;
  int n(nbRows_);
  char uplo('L');
  int luplo(1);
  if (keepIntact)
    {
      ComplexMatrixImplementation A(*this);
      ZPOTRF_F77(&uplo, &n, &A[0], &n, &info, &luplo);
      if (info != 0) throw InternalException(HERE) << "Lapack ZPOTRF: error code=" << info;
      for (UnsignedLong j = 0; j < (UnsignedLong)(n); ++j)
        for (UnsignedLong i = 0; i < (UnsignedLong)(j); ++i)
          A(i, j) = NumericalComplex(0.0, 0.0);
      // Check return code from Lapack
      if(info != 0)
        throw InvalidArgumentException(HERE) << " Error - Matrix is not positive definite" ;
      return A;
    }
  else
    {
      ZPOTRF_F77(&uplo, &n, &(*this)[0], &n, &info, &luplo);
      if (info != 0) throw InternalException(HERE) << "Lapack ZPOTRF: error code=" << info;
      for (UnsignedLong j = 0; j < (UnsignedLong)(n); ++j)
        for (UnsignedLong i = 0; i < (UnsignedLong)(j); ++i)
          (*this)(i, j) = NumericalComplex(0.0, 0.0);
      // Check return code from Lapack
      if(info != 0)
        throw InvalidArgumentException(HERE) << " Error - Matrix is not positive definite" ;
      return (*this);
    }
}

/* Comparison operator */
Bool ComplexMatrixImplementation::operator == (const ComplexMatrixImplementation & rhs) const
{
  const ComplexMatrixImplementation &lhs(*this);
  Bool equality(true);

  if (&lhs != &rhs)   // Not the same object
    {
      const Collection<NumericalComplex> & refLhs(static_cast<const Collection<NumericalComplex> >(lhs));
      const Collection<NumericalComplex> & refRhs(static_cast<const Collection<NumericalComplex> >(rhs));
      equality = ( lhs.nbRows_ == rhs.nbRows_ && lhs.nbColumns_ == rhs.nbColumns_ && refLhs == refRhs);
    }

  return equality;
}



/* Method save() stores the object through the StorageManager */
void ComplexMatrixImplementation::save(Advocate & adv) const
{
  PersistentCollection<NumericalComplex>::save(adv);
  adv.saveAttribute( "nbRows_",    nbRows_);
  adv.saveAttribute( "nbColumns_", nbColumns_);
}

/* Method load() reloads the object from the StorageManager */
void ComplexMatrixImplementation::load(Advocate & adv)
{
  PersistentCollection<NumericalComplex>::load(adv);

  adv.loadAttribute( "nbRows_",    nbRows_);
  adv.loadAttribute( "nbColumns_", nbColumns_);
}


const NumericalComplex* ComplexMatrixImplementation::__baseaddress__() const
{
  return &(*this)[0];
}


UnsignedLong ComplexMatrixImplementation::__elementsize__() const
{
  return sizeof(NumericalComplex);
}


UnsignedLong ComplexMatrixImplementation::__stride__(UnsignedLong dim) const
{
  UnsignedLong stride = __elementsize__();
  if (dim > 0)
    stride *= nbRows_;
  return stride;
}


END_NAMESPACE_OPENTURNS
