//                                               -*- C++ -*-
/**
 *  @file  LAR.cxx
 *  @brief Least Angle Regression Algorithm
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

#include "PersistentObjectFactory.hxx"
#include "BasisSequenceFactoryImplementation.hxx"
#include "LAR.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(LAR);

typedef Collection<NumericalScalar>    NumericalScalarCollection;

static Factory<LAR> RegisteredFactory("LAR");

/* Default constructor */
LAR::LAR(const Bool verbose)
  : BasisSequenceFactoryImplementation(verbose)
{
  // Nothing to do
}

/* Virtual constructor */
LAR * LAR::clone() const
{
  return new LAR( *this );
}

/* Method to create new BasisSequence objects */
BasisSequence LAR::build(const NumericalSample & x,
                         const NumericalSample & y,
                         const Basis & psi) const
{
  const UnsignedLong sampleSize( x.getSize() );
  const UnsignedLong basisSize( psi.getSize() );

  if ( y.getDimension() != 1 ) throw InvalidArgumentException( HERE ) << "Output sample should be unidimensional (dim=" << y.getDimension() << ").";
  if ( y.getSize() != sampleSize ) throw InvalidArgumentException( HERE ) << "Samples should be equally sized (in=" << sampleSize << " out=" << y.getSize() << ").";
  if ( x.getDimension() != psi.getDimension() ) throw InvalidArgumentException( HERE ) << "Sample dimension (" << x.getDimension() << ") does not match basis dimension (" << psi.getDimension() << ").";

  BasisSequence result( psi );

  // get y as as point
  NumericalPoint mY( sampleSize );
  for ( UnsignedLong j = 0; j < sampleSize; ++ j ) mY[j] = y[j][0];

  // precompute the Gram matrix
  Matrix mPsiX( sampleSize, basisSize );
  for ( UnsignedLong i = 0; i < sampleSize; ++ i )
    for ( UnsignedLong j = 0; j < basisSize; ++ j )
      mPsiX(i, j) = psi[j](x[i])[0];

  // regression coefficients
  NumericalPoint coefficients( basisSize );

  // current least-square state
  NumericalPoint mu( sampleSize );

  // list of indices of the active set
  Indices predictors;


  // main loop
  NumericalScalar oldCoefficientsL1Norm( 0.0 );
  NumericalScalar coefficientsL1Norm( 0.0 );
  NumericalScalar relativeConvergence( 1.0);
  Matrix squareRootGramMatrix;

  const UnsignedLong maximumNumberOfIterations(std::min( basisSize, sampleSize - 1 ));
  UnsignedLong iterations(0);

  do
    {
      // find the predictor most correlated with the current residual
      const NumericalPoint c( mPsiX.transpose() * ( mY - mu ));
      UnsignedLong candidatePredictor(0);
      NumericalScalar cMax(-1.0);
      for ( UnsignedLong j = 0; j < basisSize; ++ j )
        if ( ! predictors.__contains__(j) )
          {
            NumericalScalar cAbs(fabs( c[j] ));
            if ( cAbs > cMax )
              {
                cMax = cAbs;
                candidatePredictor = j;
              }
          } // if

      if ( getVerbose() ) LOGINFO( OSS() << "predictor=" << candidatePredictor << " residual=" << cMax );

      // add the predictor index
      predictors.add( candidatePredictor );

      // store the sign of the correlation
      NumericalPoint s( predictors.getSize() );
      for ( UnsignedLong j = 0; j < predictors.getSize(); ++ j ) s[j] = (c[predictors[j]] < 0.0 ? -1.0 : 1.0);

      // store correlations of the inactive set
      NumericalPoint cI;
      for ( UnsignedLong j = 0; j < basisSize; ++ j )
        if ( ! predictors.__contains__(j) )
          cI.add( c[j] );

      // matrix of elements of the inactive set
      Matrix mPsiAc ( sampleSize, basisSize - predictors.getSize() );
      UnsignedLong acIndex(0);
      for ( UnsignedLong j = 0; j < basisSize; ++ j )
        if ( ! predictors.__contains__(j) )
          {
            for ( UnsignedLong i = 0; i < sampleSize; ++ i )
              {
                mPsiAc(i, acIndex) = mPsiX(i, j);
              }
            ++ acIndex;
          } // if

      if ( getVerbose() ) LOGINFO( OSS() << "matrix of elements of the inactive set built.");

      Matrix mPsiAk( sampleSize, predictors.getSize() - 1 );
      for ( UnsignedLong i = 0; i < sampleSize; ++ i )
        for ( UnsignedLong j = 0; j < predictors.getSize() - 1; ++ j )
          mPsiAk(i, j) = mPsiX(i, predictors[j]);

      if ( getVerbose() ) LOGINFO( OSS() << "matrix of elements of the active set built.");

      // update the cholesky decomposition of the Gram matrix
      Matrix xk( sampleSize, 1 );
      NumericalScalar diagk( 0.0 );
      for ( UnsignedLong i = 0; i < sampleSize; ++ i )
        {
          xk(i, 0) = mPsiX(i, candidatePredictor);
          diagk += pow( xk(i, 0), 2.0 );
        }
      if ( squareRootGramMatrix.getNbRows() > 0 )
        {
          // solve upper triangular system R*rk=xk'*A to get the extra column
          NumericalPoint colk( ( (xk.transpose() * mPsiAk).transpose()) * NumericalPoint( 1, 1.0 ) ) ;
          NumericalPoint rk( predictors.getSize() - 1 );
          for (UnsignedLong i = 0; i < predictors.getSize() - 1; ++ i)
            {
              NumericalScalar sum( colk[i] );
              for ( SignedInteger j = i - 1; j >= 0; -- j )
                sum -= squareRootGramMatrix( j, i ) * rk[j];
              rk[i] = sum / squareRootGramMatrix( i, i );
            }

          // the extra diagonal term
          const NumericalScalar rkk( sqrt( diagk - dot( rk, rk ) ) );

          // reconstitute the whole decomposition matrix
          SquareMatrix newSquareRootGramMatrix( predictors.getSize() );
          for ( UnsignedLong i = 0; i < predictors.getSize() - 1; ++ i )
            for ( UnsignedLong j = 0; j < predictors.getSize() - 1; ++ j )
              newSquareRootGramMatrix(i, j) = squareRootGramMatrix( i, j );
          for ( UnsignedLong i = 0; i < predictors.getSize() - 1; ++ i )
            newSquareRootGramMatrix(i, predictors.getSize() - 1 ) = rk[i];
          newSquareRootGramMatrix( predictors.getSize() - 1, predictors.getSize() - 1 ) = rkk;
          squareRootGramMatrix = newSquareRootGramMatrix;
        } // if ( squareRootGramMatrix.getNbRows() > 0 )
      else squareRootGramMatrix = SquareMatrix( 1, NumericalScalarCollection( 1, sqrt( diagk ) ) );

      if ( getVerbose() ) LOGINFO( OSS() << "Cholesky factor updated.");

      // compute ga1 = AA'^-1*s using the cholesky decomposition
      NumericalPoint ga1( predictors.getSize() );
      for ( UnsignedLong i = 0; i < predictors.getSize(); ++ i )
        {
          NumericalScalar sum( s[i] );
          for ( SignedInteger j = i - 1; j >= 0; -- j ) sum -= squareRootGramMatrix( j, i ) * ga1[j];
          ga1[i] = sum / squareRootGramMatrix( i, i );
        }
      for ( SignedInteger i = predictors.getSize() - 1; i >= 0; -- i )
        {
          NumericalScalar sum( ga1[i] );
          for ( UnsignedLong j = i + 1; j < predictors.getSize(); ++ j )
            sum -= squareRootGramMatrix( i, j ) * ga1[j];
          ga1[i] = sum / squareRootGramMatrix( i, i );
        }

      if ( getVerbose() ) LOGINFO( OSS() << "Solved normal equation.");

      // normalization coefficient
      NumericalScalar cNorm( 1.0 / sqrt( dot( s, ga1 ) ) );

      // update the Matrix of elements of the active set
      mPsiAk = Matrix ( sampleSize, predictors.getSize() );
      for ( UnsignedLong i = 0; i < sampleSize; ++ i )
        for ( UnsignedLong j = 0; j < predictors.getSize(); ++ j )
          mPsiAk( i, j ) = mPsiX( i, predictors[j] );

      // descent direction
      const NumericalPoint descentDirectionAk( cNorm * ga1 );
      const NumericalPoint u( mPsiAk * descentDirectionAk );
      const NumericalPoint d( mPsiAc.transpose() * u );

      // compute step
      NumericalScalar step( cMax / cNorm );
      for ( UnsignedLong j = 0; j < basisSize - predictors.getSize(); ++ j )
        {
          NumericalScalar lhs( (cMax - cI[j]) / (cNorm - d[j]) );
          NumericalScalar rhs( (cMax + cI[j]) / (cNorm + d[j]) );
          if (lhs > 0.0)
            step = std::min(step, lhs);
          if (rhs > 0.0)
            step = std::min(step, rhs);
        }

      // update mu
      mu += step * u;

      // update coefficients
      oldCoefficientsL1Norm = coefficientsL1Norm;
      coefficientsL1Norm = 0.0;
      for ( UnsignedLong j = 0; j < predictors.getSize(); ++ j )
        {
          coefficients[predictors[j]] += step * descentDirectionAk[j];
          coefficientsL1Norm += fabs( coefficients[predictors[j]] );
        }

      relativeConvergence = fabs( coefficientsL1Norm - oldCoefficientsL1Norm ) / fabs( coefficientsL1Norm );

      result.add(predictors);

      if ( getVerbose() ) LOGINFO( OSS() << "End of iteration " << iterations << " over " << maximumNumberOfIterations - 1 << " iteration(s)");

      ++ iterations;

    }
  while ( ( iterations < maximumNumberOfIterations ) && ( relativeConvergence > maximumRelativeConvergence_ ) );

  return result;
}

/* String converter */
String LAR::__repr__() const
{
  return OSS() << "class=" << getClassName();
}

String LAR::__str__(const String & offset) const
{
  return OSS(false) << offset << __repr__();
}

/* Method save() stores the object through the StorageManager */
void LAR::save(Advocate & adv) const
{
  BasisSequenceFactoryImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void LAR::load(Advocate & adv)
{
  BasisSequenceFactoryImplementation::load(adv);
}

END_NAMESPACE_OPENTURNS
