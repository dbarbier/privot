//                                               -*- C++ -*-
/**
 *  @file  TriangularComplexMatrix.cxx
 *  @brief TriangularComplexMatrix implements the classical mathematical triangluar matrix
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
#include "TriangularComplexMatrix.hxx"
#include "Exception.hxx"
#include "Matrix.hxx"
#include "SquareMatrix.hxx"
#include "SymmetricMatrix.hxx"
#include "IdentityMatrix.hxx"
#include "HermitianMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TriangularComplexMatrix);

/* Default constructor */
TriangularComplexMatrix::TriangularComplexMatrix()
  : SquareComplexMatrix(0),
    isTriangularLower_(true)
{
  // Nothing to do
}


/* Constructor with size (dim, which is the same for nbRows_ and nbColumns_ )*/
TriangularComplexMatrix::TriangularComplexMatrix(const UnsignedLong dimension,
                                                 Bool isLower)
  : SquareComplexMatrix(dimension),
    isTriangularLower_(isLower)
{

}


/* Constructor with implementation */
TriangularComplexMatrix::TriangularComplexMatrix(const Implementation & i,
                                                 Bool isLower)

  : SquareComplexMatrix(i),
    isTriangularLower_(isLower)
{
  // Nothing to do
}


/* String converter */
String TriangularComplexMatrix::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " dimension=" << this->getDimension()
               << " implementation=" << getImplementation()->__repr__();
}

String TriangularComplexMatrix::__str__(const String & offset) const
{
  return SquareComplexMatrix::__str__();
}

/* Get the dimension of the matrix */
const UnsignedLong TriangularComplexMatrix::getDimension() const
{
  return getImplementation()->getDimension();
}

/* TriangularComplexMatrix transpose */
TriangularComplexMatrix TriangularComplexMatrix::transpose () const
{
  // Quick return for empty or scalar TriangularComplexMatrix
  if (getDimension() <= 1) return (*this);
  return TriangularComplexMatrix(Implementation(getImplementation()->transpose().clone()), !isTriangularLower_);
}

/* TriangularComplexMatrix conjugate */
TriangularComplexMatrix TriangularComplexMatrix::conjugate () const
{
  // Quick return for empty or scalar TriangularComplexMatrix
  if (getDimension() <= 1) return (*this);
  return TriangularComplexMatrix(Implementation(getImplementation()->conjugate().clone()), isTriangularLower_);
}

/* TriangularComplexMatrix conjugate */
TriangularComplexMatrix TriangularComplexMatrix::conjugateTranspose () const
{
  // Quick return for empty or scalar TriangularComplexMatrix
  if (getDimension() <= 1) return (*this);
  return TriangularComplexMatrix(Implementation(getImplementation()->conjugateTranspose().clone()), !isTriangularLower_);
}

/* Check if the matrix is lower or upper */
Bool TriangularComplexMatrix::isTriangularLower() const
{
  return isTriangularLower_;
}


/* Operator () gives access to the elements of the matrix (to modify these elements) */
/* The element of the matrix is designated by its row number i and its column number j */
/* the first element of the matrix is m(0,0) */
NumericalComplex & TriangularComplexMatrix::operator() (const UnsignedLong i,
                                                        const UnsignedLong j)
{
  if (isTriangularLower() && (i < j))
    throw InvalidArgumentException(HERE) << "Error; The triangular matrix is lower. "
                                         << "The indices are not valid" ;

  if (!isTriangularLower() && (i > j))
    throw InvalidArgumentException(HERE) << "Error; The triangular matrix is upper. "
                                         << "The indices are not valid" ;

  return  (*getImplementation())(i, j) ;
}

/* Operator () gives access to the elements of the matrix (read only) */
/* The element of the matrix is designated by its row number i and its column number j */
const NumericalComplex & TriangularComplexMatrix::operator() (const UnsignedLong i,
                                                              const UnsignedLong j)  const
{
  return (*getImplementation())(i, j) ;
}


/* TriangularComplexMatrix additions */
SquareComplexMatrix TriangularComplexMatrix::operator+ (const TriangularComplexMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* ComplexMatrix additions */
SquareComplexMatrix TriangularComplexMatrix::operator+ (const SquareComplexMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* ComplexMatrix substractions */
SquareComplexMatrix TriangularComplexMatrix::operator- (const SquareComplexMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* TriangularComplexMatrix substractions */
SquareComplexMatrix TriangularComplexMatrix::operator- (const TriangularComplexMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* ComplexMatrix multiplications */
SquareComplexMatrix TriangularComplexMatrix::operator * (const SquareComplexMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L',  uplo ) ).clone());
}

/* ComplexMatrix multiplications */
ComplexMatrix TriangularComplexMatrix::operator * (const ComplexMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L',  uplo ) ).clone());
}

/* HermitianMatrix multiplications */
SquareComplexMatrix TriangularComplexMatrix::operator * (const HermitianMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/* TriangularComplexMatrix multiplications */
SquareComplexMatrix TriangularComplexMatrix::operator * (const TriangularComplexMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/* Real SquareMatrix multiplications */
SquareComplexMatrix  TriangularComplexMatrix::operator * (const SquareMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/* Matrix multiplications */
ComplexMatrix  TriangularComplexMatrix::operator * (const Matrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/* SymmetricMatrix multiplications */
SquareComplexMatrix TriangularComplexMatrix::operator * (const SymmetricMatrix & m) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return Implementation((getImplementation()->triangularProd(*(m.getImplementation()), 'L', uplo ) ).clone());
}

/*  IdentityMatrix multiplications */
TriangularComplexMatrix TriangularComplexMatrix::operator * (const IdentityMatrix & m) const
{
  return (*this);
}

/* Multiplication with a NumericalComplexCollection (must have consistent dimensions) */
TriangularComplexMatrix::NumericalComplexCollection TriangularComplexMatrix::operator * (const NumericalComplexCollection & pt) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return getImplementation()->triangularVectProd(pt, uplo) ;
}

/* Multiplication with a NumericalScalarCollection (must have consistent dimensions) */
TriangularComplexMatrix::NumericalComplexCollection TriangularComplexMatrix::operator * (const NumericalScalarCollection & pt) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return getImplementation()->triangularVectProd(pt, uplo) ;
}

/* Multiplication with a NumericalPoint (must have consistent dimensions) */
TriangularComplexMatrix::NumericalComplexCollection TriangularComplexMatrix::operator * (const NumericalPoint & pt) const
{
  char uplo('L');
  if (!isTriangularLower()) uplo = 'R';
  return getImplementation()->triangularVectProd(pt, uplo) ;
}

/* Multiplication with a NumericalComplex */
TriangularComplexMatrix TriangularComplexMatrix::operator* (const NumericalComplex s) const
{
  return TriangularComplexMatrix(Implementation((*getImplementation() * s ).clone()), isTriangularLower_);
}

/* Division by a NumericalComplex*/
TriangularComplexMatrix TriangularComplexMatrix::operator / (const NumericalComplex s) const
{
  return TriangularComplexMatrix(Implementation((*getImplementation() / s ).clone()), isTriangularLower_);
}

END_NAMESPACE_OPENTURNS
