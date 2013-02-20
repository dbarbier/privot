//                                               -*- C++ -*-
/**
 *  @file  IdentityMatrix.hxx
 *  @brief The class IdentityMatrix implements identity matrices
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
 *  Id      IdentityMatrix.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_IDENTITYMATRIX_HXX
#define OPENTURNS_IDENTITYMATRIX_HXX

#include "OTprivate.hxx"
#include "CorrelationMatrix.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class IdentityMatrix
 */

class IdentityMatrix
  : public CorrelationMatrix
{
  CLASSNAME;

public:


  /** Default constructor */
  IdentityMatrix();

  /** Constructor with size */
  IdentityMatrix(const UnsignedLong dim);


  /** String converter */
  virtual String __repr__() const;

  /** IdentityMatrix transpose */
  IdentityMatrix transpose () const ;

#ifndef SWIG
  /** Operator () gives access to the elements of the matrix (read only) */
  /** The element of the matrix is designated by its row number i and its column number j */
  const NumericalScalar & operator () (const UnsignedLong i,
                                       const UnsignedLong j) const;
#endif

  /** Multiplications */
  Matrix operator * (const Matrix & m) const;
  SquareMatrix operator * (const SquareMatrix & m) const;
  SymmetricMatrix operator * (const SymmetricMatrix & m) const;
  CovarianceMatrix operator * (const CovarianceMatrix & m) const;
  CorrelationMatrix operator * (const CorrelationMatrix & m) const;
  using CorrelationMatrix::operator *;

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

  /** Check if the matrix is SPD */
  virtual Bool isPositiveDefinite(const Bool keepIntact = true);

  /** Build the Cholesky factorization of the matrix */
  SquareMatrix computeCholesky(const Bool keepIntact = true);

  /** Compute singular values */
  NumericalPoint computeSingularValues(const Bool keepIntact = true);

  NumericalPoint computeSingularValues(Matrix & u,
                                       Matrix & vT,
                                       const Bool fullSVD = false,
                                       const Bool keepIntact = true);

protected:


  /** Constructor with implementation */
  IdentityMatrix(const Implementation & i);

private:

  /** Operator () gives access to the elements of the matrix (to modify these elements) */
  /** The element of the matrix is designated by its row number i and its column number j */
  NumericalScalar & operator () (const UnsignedLong i,
                                 const UnsignedLong j);
}; /* class IdentityMatrix */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_COVARIANCEMATRIX_HXX */
