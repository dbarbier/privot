//                                               -*- C++ -*-
/**
 *  @file  PenalizedLeastSquaresAlgorithm.cxx
 *  @brief This Penalized Least Squares Algorithm as a functor class
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "PenalizedLeastSquaresAlgorithm.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Matrix.hxx"
#include "IdentityMatrix.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(PenalizedLeastSquaresAlgorithm);

static Factory<PenalizedLeastSquaresAlgorithm> RegisteredFactory("PenalizedLeastSquaresAlgorithm");


/* Default constructor */
PenalizedLeastSquaresAlgorithm::PenalizedLeastSquaresAlgorithm()
  : ApproximationAlgorithmImplementation()
{
  // Nothing to do
}

/* Parameters constructor */
PenalizedLeastSquaresAlgorithm::PenalizedLeastSquaresAlgorithm(const NumericalSample & x,
                                                               const NumericalSample & y,
                                                               const NumericalMathFunctionCollection & psi,
                                                               const NumericalScalar penalizationFactor)
  : ApproximationAlgorithmImplementation( x, y, psi ),
    penalizationFactor_(penalizationFactor),
    penalizationMatrix_(0)
{
  // If the penalization factor is strictly positive, use the identity matrix as a penalization term
  if (penalizationFactor > 0.0)
    {
      const UnsignedLong basisSize(psi.getSize());
      penalizationMatrix_ = IdentityMatrix(basisSize);
    }
}


/* Parameters constructor */
PenalizedLeastSquaresAlgorithm::PenalizedLeastSquaresAlgorithm(const NumericalSample & x,
                                                               const NumericalSample & y,
                                                               const NumericalPoint & weight,
                                                               const NumericalMathFunctionCollection & psi,
                                                               const NumericalScalar penalizationFactor)
  : ApproximationAlgorithmImplementation( x, y, weight, psi ),
    penalizationFactor_(penalizationFactor),
    penalizationMatrix_(0)
{
  // If the penalization factor is strictly positive, use the identity matrix as a penalization term
  if (penalizationFactor > 0.0)
    {
      const UnsignedLong basisSize(psi.getSize());
      penalizationMatrix_ = IdentityMatrix(basisSize);
    }
}

/* Parameters constructor */
PenalizedLeastSquaresAlgorithm::PenalizedLeastSquaresAlgorithm(const NumericalSample & x,
                                                               const NumericalSample & y,
                                                               const NumericalPoint & weight,
                                                               const NumericalMathFunctionCollection & psi,
                                                               const NumericalScalar penalizationFactor,
                                                               const CovarianceMatrix & penalizationMatrix)
  : ApproximationAlgorithmImplementation( x, y, weight, psi ),
    penalizationFactor_(penalizationFactor),
    penalizationMatrix_(penalizationMatrix)
{
  const UnsignedLong basisSize(psi.getSize());
  // Check if the penalization matrix has the proper dimension
  if (penalizationMatrix_.getDimension() != basisSize) throw InvalidArgumentException(HERE) << "Error: the given penalization matrix has an improper dimension.";
  if (!const_cast<CovarianceMatrix*>(&penalizationMatrix)->isPositiveDefinite()) throw NotSymmetricDefinitePositiveException(HERE) << "Error: the given penalization matrix is not strictly definite positive.";
}

/* Perform the optimization */
void PenalizedLeastSquaresAlgorithm::run()
{
  const UnsignedLong basisDimension(psi_.getSize());
  const UnsignedLong sampleSize(x_.getSize());
  Matrix basisMatrix;
  NumericalPoint rightHandSide;
  // If there is a penalization term, augment the dimension of the matrix and the right hand side
  if (penalizationFactor_ > 0.0)
    {
      basisMatrix = Matrix(sampleSize + basisDimension, basisDimension);
      rightHandSide = NumericalPoint(sampleSize + basisDimension, 0.0);
    }
  else
    {
      basisMatrix = Matrix(sampleSize, basisDimension);
      rightHandSide = NumericalPoint(sampleSize, 0.0);
    }
  NumericalPoint weightSquareRoot(sampleSize);
  // Build the right-hand side
  for (UnsignedLong rowIndex = 0; rowIndex < sampleSize; ++rowIndex)
    {
      weightSquareRoot[rowIndex] = sqrt(weight_[rowIndex]);
      rightHandSide[rowIndex] = weightSquareRoot[rowIndex] * y_[rowIndex][0];
    }
  // Build the matrix column-wise
  for (UnsignedLong functionIndex = 0; functionIndex < basisDimension; ++functionIndex)
    {
      NumericalSample functionSample(psi_[functionIndex](x_));
      // Fill-in the matrix column
      for (UnsignedLong rowIndex = 0; rowIndex < sampleSize; ++rowIndex)
        {
          basisMatrix(rowIndex, functionIndex) = functionSample[rowIndex][0] * weightSquareRoot[rowIndex];
        }
    }
  // If there is a penalization term, add the penalization matrix to the basis matrix
  if (penalizationFactor_ > 0.0)
    {
      const Matrix transposedSquareRootPenalizationMatrix(sqrt(penalizationFactor_) * penalizationMatrix_.computeCholesky());
      for (UnsignedLong i = 0; i < basisDimension; ++i)
        {
          // The cholesky factor has to be transposed, thus we fill only the upper triangular part of the trailing block
          for (UnsignedLong j = i; j < basisDimension; ++j)
            basisMatrix(sampleSize + i, j) = transposedSquareRootPenalizationMatrix(j, i);
        }
    }
  // Solve the linear system (least squares solution)
  setCoefficients( basisMatrix.solveLinearSystem( rightHandSide ) );

  const NumericalScalar quadraticResidual( (basisMatrix * getCoefficients() - rightHandSide).norm2() );

  // The residual is the mean L2 norm of the fitting
  setResidual( sqrt( quadraticResidual ) / sampleSize );

  const NumericalScalar empiricalError( quadraticResidual / sampleSize );

  // The relative error
  setRelativeError( empiricalError / y_.computeVariancePerComponent()[0] );
}


/* Virtual constructor */
PenalizedLeastSquaresAlgorithm * PenalizedLeastSquaresAlgorithm::clone() const
{
  return new PenalizedLeastSquaresAlgorithm(*this);
}


/* String converter */
String PenalizedLeastSquaresAlgorithm::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << ApproximationAlgorithmImplementation::__repr__()
               << " penalization factor=" << penalizationFactor_
               << " penalization matrix=" << penalizationMatrix_;
}


/* Method save() stores the object through the StorageManager */
void PenalizedLeastSquaresAlgorithm::save(Advocate & adv) const
{
  ApproximationAlgorithmImplementation::save(adv);
  adv.saveAttribute( "penalizationFactor_", penalizationFactor_ );
  adv.saveAttribute( "penalizationMatrix_", penalizationMatrix_ );
}


/* Method load() reloads the object from the StorageManager */
void PenalizedLeastSquaresAlgorithm::load(Advocate & adv)
{
  ApproximationAlgorithmImplementation::load(adv);
  adv.loadAttribute( "penalizationFactor_", penalizationFactor_ );
  adv.loadAttribute( "penalizationMatrix_", penalizationMatrix_ );
}



END_NAMESPACE_OPENTURNS
