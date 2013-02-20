//                                               -*- C++ -*-
/**
 *  @file  LeastSquaresMetaModelSelection.cxx
 *  @brief Basis selection algorithm
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

#include <limits>
#include "Log.hxx"
#include "Indices.hxx"
#include "PersistentObjectFactory.hxx"
#include "PenalizedLeastSquaresAlgorithm.hxx"
#include "LeastSquaresMetaModelSelection.hxx"

BEGIN_NAMESPACE_OPENTURNS




CLASSNAMEINIT(LeastSquaresMetaModelSelection);

static Factory<LeastSquaresMetaModelSelection> RegisteredFactory("LeastSquaresMetaModelSelection");

/* Default constructor */
LeastSquaresMetaModelSelection::LeastSquaresMetaModelSelection()
  : ApproximationAlgorithmImplementation()
{
  // Nothing to do
}

/* Default constructor */
LeastSquaresMetaModelSelection::LeastSquaresMetaModelSelection(const NumericalSample & x,
                                                               const NumericalSample & y,
                                                               const Basis & psi,
                                                               const BasisSequenceFactory & basisSequenceFactory,
                                                               const FittingAlgorithm & fittingAlgorithm)
  : ApproximationAlgorithmImplementation( x, y, psi ),
    basisSequenceFactory_(basisSequenceFactory),
    fittingAlgorithm_(fittingAlgorithm)

{
  // Nothing to do
}

/* Default constructor */
LeastSquaresMetaModelSelection::LeastSquaresMetaModelSelection(const NumericalSample & x,
                                                               const NumericalSample & y,
                                                               const NumericalPoint & weight,
                                                               const Basis & psi,
                                                               const BasisSequenceFactory & basisSequenceFactory,
                                                               const FittingAlgorithm & fittingAlgorithm)
  : ApproximationAlgorithmImplementation( x, y, weight, psi ),
    basisSequenceFactory_(basisSequenceFactory),
    fittingAlgorithm_(fittingAlgorithm)
{
  // Nothing to do
}


/* Virtual constructor */
LeastSquaresMetaModelSelection * LeastSquaresMetaModelSelection::clone() const
{
  return new LeastSquaresMetaModelSelection( *this );
}

/* BasisSequenceFactory accessor */
void LeastSquaresMetaModelSelection::setBasisSequenceFactory(const BasisSequenceFactory & basisSequenceFactory)
{
  basisSequenceFactory_ = basisSequenceFactory;
}

BasisSequenceFactory LeastSquaresMetaModelSelection::getBasisSequenceFactory() const
{
  return basisSequenceFactory_;
}

/* FittingAlgorithm accessor */
void LeastSquaresMetaModelSelection::setFittingAlgorithm(const FittingAlgorithm & fittingAlgorithm)
{
  fittingAlgorithm_ = fittingAlgorithm;
}

FittingAlgorithm LeastSquaresMetaModelSelection::getFittingAlgorithm() const
{
  return fittingAlgorithm_;
}


/* String converter */
String LeastSquaresMetaModelSelection::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " basisSequenceFactory=" << basisSequenceFactory_
               << " fittingAlgorithm=" << fittingAlgorithm_;
}


/* Perform the selection */
void LeastSquaresMetaModelSelection::run()
{
  // compute weighted right hand
  const UnsignedLong sampleSize( y_.getSize() );
  NumericalSample weightedY( sampleSize, 1 );
  for ( UnsignedLong i = 0; i < sampleSize; ++ i ) weightedY[i][0] = y_[i][0] * sqrt( weight_[i] );

  // generate all the sub-basis
  const BasisSequence basisSequence( basisSequenceFactory_.build( x_, weightedY, psi_ ) );
  const UnsignedLong sequenceSize( basisSequence.getSize() );

  // for each sub-basis ...
  NumericalScalar minimumError( std::numeric_limits< NumericalScalar >::max() );

  UnsignedLong optimalBasisIndex(0);
  for ( UnsignedLong i = 0; i < sequenceSize; ++ i )
    {
      // retrieve the i-th basis of the sequence
      const Basis basis( basisSequence.getBasis( i ) );
      const NumericalScalar error( fittingAlgorithm_.run( x_, weightedY, basis ) );

      if ( getVerbose() ) LOGINFO( OSS() << "subbasis=" << i << ", size=" << basis.getSize() << ", error=" << error << ", qSquare=" << 1.0 - error );

      if ( error < minimumError )
        {
          optimalBasisIndex = i;
          minimumError = error;
        }
    }

  // recompute the coefficients of the selected sparse metamodel by least-square regression
  PenalizedLeastSquaresAlgorithm penalizedLeastSquaresAlgorithm( x_, y_, weight_, basisSequence.getBasis( optimalBasisIndex ) );
  penalizedLeastSquaresAlgorithm.run();
  const NumericalPoint optimalBasisCoefficients(penalizedLeastSquaresAlgorithm.getCoefficients());
  const NumericalScalar optimalResidual(penalizedLeastSquaresAlgorithm.getResidual());
  const NumericalScalar optimalRelativeError(penalizedLeastSquaresAlgorithm.getRelativeError());


  // compute the coefficients in the master basis from the ones in the optimal sub-basis
  NumericalPoint optimalCoefficients( psi_.getSize() );
  const Indices optimalBasisIndices( basisSequence[optimalBasisIndex] );
  UnsignedLong optimalBasisSize( basisSequence[optimalBasisIndex].getSize() );
  for ( UnsignedLong i = 0; i < optimalBasisSize; ++ i ) optimalCoefficients[ optimalBasisIndices[i] ] = optimalBasisCoefficients[i];

  setCoefficients( optimalCoefficients );
  setResidual( optimalResidual );
  setRelativeError( minimumError );

  if ( getVerbose() )
    {
      LOGINFO( OSS() << "optimalBasisIndex=" << optimalBasisIndex );
      LOGINFO( OSS() << "optimalError=" << minimumError );
      LOGINFO( OSS() << "optimalResidual=" << optimalResidual );
      LOGINFO( OSS() << "optimalRelativeError=" << optimalRelativeError );
    }
}


/* Method save() stores the object through the StorageManager */
void LeastSquaresMetaModelSelection::save(Advocate & adv) const
{
  ApproximationAlgorithmImplementation::save(adv);
  adv.saveAttribute( "basisSequenceFactory_", basisSequenceFactory_ );
  adv.saveAttribute( "fittingAlgorithm_", fittingAlgorithm_ );
}


/* Method load() reloads the object from the StorageManager */
void LeastSquaresMetaModelSelection::load(Advocate & adv)
{
  ApproximationAlgorithmImplementation::load(adv);
  adv.loadAttribute( "basisSequenceFactory_", basisSequenceFactory_ );
  adv.loadAttribute( "fittingAlgorithm_", fittingAlgorithm_ );
}


END_NAMESPACE_OPENTURNS
