//                                               -*- C++ -*-
/**
 *  @file  CorrectedLeaveOneOut.cxx
 *  @brief Corrected implicit leave-one-out cross validation
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "NumericalMathFunction.hxx"
#include "PersistentObjectFactory.hxx"
#include "CorrectedLeaveOneOut.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(CorrectedLeaveOneOut);

static Factory<CorrectedLeaveOneOut> RegisteredFactory("CorrectedLeaveOneOut");

/* Default constructor */
CorrectedLeaveOneOut::CorrectedLeaveOneOut()
  : FittingAlgorithmImplementation()
{
  // Nothing to do
}

/* Virtual constructor */
CorrectedLeaveOneOut * CorrectedLeaveOneOut::clone() const
{
  return new CorrectedLeaveOneOut( *this );
}

/* String converter */
String CorrectedLeaveOneOut::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}

/* Perform cross-validation */
NumericalScalar CorrectedLeaveOneOut::run(const NumericalSample & x,
                                          const NumericalSample & y,
                                          const Basis & basis) const
{
  const UnsignedLong sampleSize( x.getSize() );

  if ( y.getDimension() != 1 ) throw InvalidArgumentException( HERE ) << "Output sample should be unidimensional (dim=" << y.getDimension() << ").";
  if ( y.getSize() != sampleSize ) throw InvalidArgumentException( HERE ) << "Samples should be equally sized (in=" << sampleSize << " out=" << y.getSize() << ").";
  const NumericalScalar variance( y.computeVariancePerComponent()[0] );
  if ( variance <= 0.0 ) throw InvalidArgumentException( HERE ) << "Null output sample variance.";

  const UnsignedLong basisSize( basis.getSize() );

  // Build the design of experiments
  Matrix psiAk(sampleSize, basisSize);
  for (UnsignedLong i = 0; i < sampleSize; ++ i )
    for (UnsignedLong j = 0; j < basisSize; ++ j )
      psiAk( i, j ) = basis[j]( x[i] )[0];

  // Compute the reduced SVD (first 'false' flag) trashing the psiAk matrix (second 'false' flag) as it is no more needed
  Matrix u;
  Matrix vT;
  const NumericalPoint svd( psiAk.computeSingularValues(u, vT, false, false) );
  // Solve the least squares problem argmin ||psiAk * coefficients - b||^2 using this decomposition
  NumericalPoint b( sampleSize );
  for (UnsignedLong i = 0; i < sampleSize; ++i) b[i] = y[i][0];
  // First step
  const NumericalPoint c(u.transpose() * b);
  // Second step
  NumericalPoint d( basisSize );
  for (UnsignedLong i = 0; i < basisSize; ++i) d[i] = c[i] / svd[i];
  // Third step
  const NumericalPoint coefficients(vT.transpose() * d);

  // Compute the  empirical error
  NumericalPoint h( sampleSize );
  for (UnsignedLong i = 0; i < sampleSize; ++ i )
    for (UnsignedLong j = 0; j < basisSize; ++ j )
      h[i] += u(i, j) * u(i, j);

  const NumericalMathFunction metamodel(basis, coefficients);
  const NumericalSample yHat(metamodel(x));
  NumericalScalar empiricalError(0.0);
  for ( UnsignedLong j = 0; j < sampleSize; ++ j )
    empiricalError += pow( ( y[j][0] - yHat[j][0] ) / ( 1.0 - h[j] ), 2.0 ) / sampleSize;

  // compute correcting factor

  NumericalScalar traceInverse( 0.0 );
  for (UnsignedLong k = 0; k < svd.getDimension(); ++ k)
    traceInverse += 1.0 / pow(svd[k], 2.0);

  const NumericalScalar correctingFactor( ( static_cast<NumericalScalar> (sampleSize) / static_cast<NumericalScalar>(sampleSize - basisSize) ) * ( 1.0 + traceInverse ) );

  return correctingFactor * empiricalError / variance;
}

/* Method save() stores the object through the StorageManager */
void CorrectedLeaveOneOut::save(Advocate & adv) const
{
  FittingAlgorithmImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void CorrectedLeaveOneOut::load(Advocate & adv)
{
  FittingAlgorithmImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
