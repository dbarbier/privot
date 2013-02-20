//                                               -*- C++ -*-
/**
 *  @file  ComplexMatrix.cxx
 *  @brief ComplexMatrix implements the classical mathematical matrix
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

#include "ComplexMatrix.hxx"
#include "ComplexMatrixImplementation.hxx"
#include "PersistentObject.hxx"
#include "HermitianMatrix.hxx"
#include "TriangularComplexMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ComplexMatrix);

/* Default constructor */
ComplexMatrix::ComplexMatrix()
  : TypedInterfaceObject<ComplexMatrixImplementation>(new ComplexMatrixImplementation())
{
  // Nothing to do
}

/* Constructor with size (rowDim and colDim) */
/* The matrix is made up of a collection of rowDim*colDim elements */
/* The matrix is viewed as a set of column vectors read one after another */
ComplexMatrix::ComplexMatrix(const UnsignedLong rowDim,
                             const UnsignedLong colDim)
  : TypedInterfaceObject<ComplexMatrixImplementation>(new ComplexMatrixImplementation(rowDim, colDim))
{
  // Nothing to do
}

/* Constructor from external collection */
/* If the dimensions of the matrix and of the collection */
/* do not match, either the collection is truncated */
/* or the rest of the matrix is filled with zeros */
ComplexMatrix::ComplexMatrix(const UnsignedLong rowDim,
                             const UnsignedLong colDim,
                             const NumericalComplexCollection & elementsValues)
  : TypedInterfaceObject<ComplexMatrixImplementation>(new ComplexMatrixImplementation(rowDim, colDim, elementsValues))
{
  // Nothing to do
}

/* Constructor from external collection */
/* If the dimensions of the matrix and of the collection */
/* do not match, either the collection is truncated */
/* or the rest of the matrix is filled with zeros */
ComplexMatrix::ComplexMatrix(const UnsignedLong rowDim,
                             const UnsignedLong colDim,
                             const NumericalScalarCollection & elementsValues)
  : TypedInterfaceObject<ComplexMatrixImplementation>(new ComplexMatrixImplementation(rowDim, colDim, elementsValues))
{
  // Nothing to do
}

/* Constructor with implementation */
ComplexMatrix::ComplexMatrix(const Implementation & i)
  : TypedInterfaceObject<ComplexMatrixImplementation>(i)
{
  // Nothing to do
}

/* Constructor with implementation */
ComplexMatrix::ComplexMatrix(const ComplexMatrixImplementation & i)
  : TypedInterfaceObject<ComplexMatrixImplementation>(i.clone())
{
  // Nothing to do
}

/* Set small elements to zero */
ComplexMatrix ComplexMatrix::clean(const NumericalScalar threshold) const
{
  return Implementation(getImplementation()->clean(threshold).clone());
}

/* String converter */
String ComplexMatrix::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " implementation=" << getImplementation()->__repr__();
}


String ComplexMatrix::__str__(const String & offset) const
{
  OSS oss;
  const UnsignedLong rows = getNbRows();
  const UnsignedLong cols = getNbColumns();
  if ( (rows >= ResourceMap::GetAsUnsignedLong("Matrix-size-visible-in-str-from")) ||
       (cols >= ResourceMap::GetAsUnsignedLong("Matrix-size-visible-in-str-from")) )
    oss << rows << "x" << cols << "\n";

  size_t lwidth = 0;
  size_t rwidth = 0;
  for( UnsignedLong i = 0; i < rows; ++i )
    for( UnsignedLong j = 0; j < cols; ++j )
      {
        String st = OSS() << (*this)(i, j);
        size_t dotpos = st.find( '.' );
        lwidth = std::max( lwidth, (dotpos != String::npos) ? dotpos             : st.size() );
        rwidth = std::max( rwidth, (dotpos != String::npos) ? st.size() - dotpos : 0         );
      }

  const char * bracket = "[";
  const char * newline = "";
  for( UnsignedLong i = 0; i < rows; ++i, newline = "\n", bracket = " " )
    {
      oss << newline << offset << bracket << "[ ";
      const char * sep = "";
      for( UnsignedLong j = 0; j < cols; ++j, sep = " " )
        {
          String st = OSS() << (*this)(i, j);
          size_t dotpos = st.find( '.' );
          oss << sep << String( lwidth - ((dotpos != String::npos) ? dotpos : st.size()), ' ' )
              << st
              << String( rwidth - ((dotpos != String::npos) ? st.size() - dotpos : 0), ' ' );
        }
      oss << " ]";
    }
  oss << "]";
  return oss;
}


/* Get the dimensions of the matrix : number of rows */
const UnsignedLong ComplexMatrix::getNbRows() const
{
  return getImplementation()->getNbRows();
}

/* Get the dimensions of the matrix : number of columns */
const UnsignedLong ComplexMatrix::getNbColumns() const
{
  return getImplementation()->getNbColumns();
}

/* Operator () gives access to the elements of the matrix (to modify these elements) */
/* The element of the matrix is designated by its row number i and its column number j */
/* the first element of the matrix is m(0,0) */
NumericalComplex & ComplexMatrix::operator() (const UnsignedLong i,
                                              const UnsignedLong j)
{
  copyOnWrite();
  return (*getImplementation())(i, j);
}

/* Operator () gives access to the elements of the matrix (read only) */
/* The element of the matrix is designated by its row number i and its column number j */
const NumericalComplex & ComplexMatrix::operator() (const UnsignedLong i,
                                                    const UnsignedLong j)  const
{
  return (*getImplementation())(i, j);
}

/* ComplexMatrix transpose */
ComplexMatrix ComplexMatrix::transpose () const
{
  return Implementation(getImplementation()->transpose().clone());
}


/* ComplexMatrix conjugate */
ComplexMatrix ComplexMatrix::conjugate () const
{
  return Implementation(getImplementation()->conjugate().clone());
}

/* ComplexMatrix conjugate and transpose */
ComplexMatrix ComplexMatrix::conjugateTranspose () const
{
  return Implementation(getImplementation()->conjugateTranspose().clone());
}

/* Get the real part of the matrix */
Matrix ComplexMatrix::real() const
{
  return Matrix::Implementation(getImplementation()->realRect().clone());
}

/* Get the imaginary part of the matrix */
Matrix ComplexMatrix::imag() const
{
  return Matrix::Implementation(getImplementation()->imagRect().clone());
}

/* ComplexMatrix additions (must have the same dimensions) */
ComplexMatrix ComplexMatrix::operator+ (const ComplexMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* ComplexMatrix additions */
ComplexMatrix ComplexMatrix::operator+ (const Matrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* ComplexMatrix substractions (must have the same dimensions) */
ComplexMatrix ComplexMatrix::operator- (const ComplexMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* ComplexMatrix substractions */
ComplexMatrix ComplexMatrix::operator- (const Matrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* ComplexMatrix multiplications (must have consistent dimensions) */
ComplexMatrix ComplexMatrix::operator* (const ComplexMatrix & m) const
{
  return Implementation((getImplementation()->genProd(*(m.getImplementation())) ).clone());
}

/* ComplexMatrix multiplications (must have consistent dimensions) */
ComplexMatrix ComplexMatrix::operator* (const HermitianMatrix & m) const
{
  return Implementation((getImplementation()->hermProd(*(m.getImplementation()), 'R') ).conjugateTranspose().clone());
}

/* ComplexMatrix multiplications  */
ComplexMatrix ComplexMatrix::operator* (const TriangularComplexMatrix & m) const
{
  char uplo('L');
  if (!m.isTriangularLower()) uplo = 'R';
  return Implementation(m.getImplementation()->triangularProd(*(getImplementation()), 'R', uplo).clone());
}

/* ComplexMatrix multiplication - Matrix on right */
ComplexMatrix ComplexMatrix::operator* (const Matrix & m) const
{
  return Implementation((getImplementation()->genProd(*(m.getImplementation())) ).clone());
}

/* ComplexMatrix multiplication - SymmetricMatrix on right */
ComplexMatrix ComplexMatrix::operator* (const SymmetricMatrix & m) const
{
  m.checkSymmetry();
  return Implementation((getImplementation()->genProd(*(m.getImplementation())) ).clone());
}

/* ComplexMatrix multiplication - SymmetricMatrix on right */
ComplexMatrix ComplexMatrix::operator* (const IdentityMatrix & m) const
{
  return (*this);
}

/* Multiplication with a NumericalScalarCollection  */
ComplexMatrix::NumericalComplexCollection ComplexMatrix::operator* (const NumericalScalarCollection & collection) const
{
  return getImplementation()->genVectProd(collection) ;
}

/* Multiplication with a NumericalPoint  */
ComplexMatrix::NumericalComplexCollection ComplexMatrix::operator* (const NumericalPoint & point) const
{
  return getImplementation()->genVectProd(point) ;
}

/* Multiplication with a NumericalComplexCollection */
ComplexMatrix::NumericalComplexCollection ComplexMatrix::operator* (const NumericalComplexCollection & collection) const
{
  return getImplementation()->genVectProd(collection) ;
}

/* Multiplication with a NumericalComplex */
ComplexMatrix ComplexMatrix::operator* (const NumericalComplex s) const
{
  return Implementation((*getImplementation() * s ).clone());
}

/* Division by a NumericalComplex*/
ComplexMatrix ComplexMatrix::operator/ (const NumericalComplex s) const
{
  return Implementation((*getImplementation() / s ).clone());
}


/* Empty returns true if there is no element in the matrix */
const Bool ComplexMatrix::isEmpty() const
{
  return getImplementation()->isEmpty() ;
}


/* Comparison operator */
Bool ComplexMatrix::operator == (const ComplexMatrix & rhs) const
{
  const ComplexMatrix &lhs(*this);
  return (*(lhs.getImplementation()) == *(rhs.getImplementation()) );
}


const NumericalComplex* ComplexMatrix::__baseaddress__() const
{
  return getImplementation()->__baseaddress__();
}


UnsignedLong ComplexMatrix::__elementsize__() const
{
  return getImplementation()->__elementsize__();
}


UnsignedLong ComplexMatrix::__stride__(UnsignedLong dim) const
{
  return getImplementation()->__stride__(dim);
}


END_NAMESPACE_OPENTURNS
