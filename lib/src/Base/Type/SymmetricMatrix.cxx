//                                               -*- C++ -*-
/**
 *  @file  SymmetricMatrix.cxx
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
 */
#include "SymmetricMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(SymmetricMatrix);

/* Default constructor */
SymmetricMatrix::SymmetricMatrix()
  : SquareMatrix(0),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}


/* Constructor with size (dim, which is the same for nbRows_ and nbColumns_ )*/
SymmetricMatrix::SymmetricMatrix(const UnsignedLong dim)
  : SquareMatrix(dim),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}

/* Constructor from external collection */
/* If the dimensions of the matrix and of the collection */
/* do not match, either the collection is truncated */
/* or the rest of the matrix is filled with zeros */
SymmetricMatrix::SymmetricMatrix(const UnsignedLong dim,
                                 const Collection<NumericalScalar> & elementsValues)
  : SquareMatrix(dim, elementsValues),
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}


/* Constructor with implementation */
SymmetricMatrix::SymmetricMatrix(const Implementation & i)
  : SquareMatrix(i) ,
    hasBeenSymmetrized_(false)
{
  // Nothing to do
}


/* Check if the internal representation is really symmetric */
void SymmetricMatrix::checkSymmetry() const
{
  if (!hasBeenSymmetrized_)
    {
      getImplementation()->symmetrize();
      hasBeenSymmetrized_ = true;
    }
}

/* Test if the matrix is diagonal */
Bool SymmetricMatrix::isDiagonal() const
{
  // The loops must be done in this order in order to use the quick accessor
  // of the underlying implementation without symmetrization of the matrix.
  // We know that the storage is made column-wise, using the upper triangle
  for (UnsignedLong j = 0; j < getDimension(); ++j)
    for (UnsignedLong i = j + 1; i < getDimension(); ++i)
      if ((*getImplementation())(i, j) != 0.0) return false;
  return true;
}

/* Row extraction */
const Matrix SymmetricMatrix::getRow(const UnsignedLong rowIndex) const
{
  return Implementation(getImplementation()->getRowSym(rowIndex).clone());
}

/* Column extration */
const Matrix SymmetricMatrix::getColumn(const UnsignedLong columnIndex) const
{
  return Implementation(getImplementation()->getColumnSym(columnIndex).clone());
}

/* String converter */
String SymmetricMatrix::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " dimension=" << this->getDimension()
               << " implementation=" << getImplementation()->__repr__();
}

String SymmetricMatrix::__str__(const String & offset) const
{
  checkSymmetry();
  return SquareMatrix::clean(0.0).__str__();
}

/* Operator () gives access to the elements of the matrix (to modify these elements) */
/* The element of the matrix is designated by its row number i and its column number j */
/* the first element of the matrix is m(0,0) */
NumericalScalar & SymmetricMatrix::operator() (const UnsignedLong i,
                                               const UnsignedLong j)
{
  copyOnWrite();
  hasBeenSymmetrized_ = false;

  return (i > j) ? (*getImplementation())(i, j) : (*getImplementation())(j, i) ;
}

/* Operator () gives access to the elements of the matrix (read only) */
/* The element of the matrix is designated by its row number i and its column number j */
const NumericalScalar & SymmetricMatrix::operator() (const UnsignedLong i,
                                                     const UnsignedLong j)  const
{
  return (i > j) ? (*getImplementation())(i, j) : (*getImplementation())(j, i) ;
}

/* SymmetricMatrix transpose */
SymmetricMatrix SymmetricMatrix::transpose () const
{
  return *this;
}

/* SymmetricMatrix additions (must have the same dimensions) */
Matrix SymmetricMatrix::operator + (const Matrix & m) const
{
  checkSymmetry();
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

SquareMatrix SymmetricMatrix::operator + (const SquareMatrix & m) const
{
  checkSymmetry();
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

SymmetricMatrix SymmetricMatrix::operator + (const SymmetricMatrix & m) const
{
  return Implementation((*getImplementation() + * (m.getImplementation()) ).clone());
}

/* SymmetricMatrix substractions (must have the same dimensions) */
Matrix SymmetricMatrix::operator - (const Matrix & m) const
{
  checkSymmetry();
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

SquareMatrix SymmetricMatrix::operator - (const SquareMatrix & m) const
{
  checkSymmetry();
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

SymmetricMatrix SymmetricMatrix::operator - (const SymmetricMatrix & m) const
{
  return Implementation((*getImplementation() - * (m.getImplementation()) ).clone());
}

/* SymmetricMatrix multiplications (must have consistent dimensions) */
Matrix SymmetricMatrix::operator * (const Matrix & m) const
{
  return Implementation((getImplementation()->symProd(*(m.getImplementation()), 'L') ).clone());
}

SquareMatrix SymmetricMatrix::operator * (const SquareMatrix & m) const
{
  return Implementation((getImplementation()->symProd(*(m.getImplementation()), 'L') ).clone());
}

SquareMatrix SymmetricMatrix::operator * (const SymmetricMatrix & m) const
{
  // Must check symmetry of the right-hand factor as it is seen as a square matrix
  m.checkSymmetry();
  return Implementation((getImplementation()->symProd(*(m.getImplementation()), 'L') ).clone());
}

SymmetricMatrix SymmetricMatrix::operator * (const IdentityMatrix & m) const
{
  return *this;
}


/* Multiplication with a NumericalPoint (must have consistent dimensions) */
NumericalPoint SymmetricMatrix::operator * (const NumericalPoint & pt) const
{
  return getImplementation()->symVectProd(pt) ;
}


/* Multiplication with a NumericalScalar */
SymmetricMatrix SymmetricMatrix::operator * (const NumericalScalar & s) const
{
  return Implementation((*getImplementation() * s ).clone());
}

/* Division by a NumericalScalar*/
SymmetricMatrix SymmetricMatrix::operator / (const NumericalScalar & s) const
{
  return Implementation((*getImplementation() / s ).clone());
}

/* SquareMatrix integer power */
SymmetricMatrix SymmetricMatrix::power(const UnsignedLong n) const
{
  return Implementation((getImplementation()->symPower(n)).clone());
}

/* Resolution of a linear system */
NumericalPoint SymmetricMatrix::solveLinearSystem(const NumericalPoint & b,
                                                  const Bool keepIntact)
{
  return getImplementation()->solveLinearSystemSym(b, keepIntact);
}

Matrix SymmetricMatrix::solveLinearSystem(const Matrix & b,
                                          const Bool keepIntact)
{
  return getImplementation()->solveLinearSystemSym(*b.getImplementation(), keepIntact);
}

/* Compute determinant */
NumericalScalar SymmetricMatrix::computeLogAbsoluteDeterminant(NumericalScalar & sign,
                                                               const Bool keepIntact)
{
  return getImplementation()->computeLogAbsoluteDeterminantSym(sign, keepIntact);
}

NumericalScalar SymmetricMatrix::computeDeterminant(const Bool keepIntact)
{
  return getImplementation()->computeDeterminantSym(keepIntact);
}

/* Compute eigenvalues */
NumericalPoint SymmetricMatrix::computeEigenValues(const Bool keepIntact)
{
  return getImplementation()->computeEigenValuesSym(keepIntact);
}
NumericalPoint SymmetricMatrix::computeEigenValues(SquareMatrix & v,
                                                   const Bool keepIntact)
{
  return getImplementation()->computeEigenValuesSym(*(v.getImplementation()), keepIntact);
}


END_NAMESPACE_OPENTURNS