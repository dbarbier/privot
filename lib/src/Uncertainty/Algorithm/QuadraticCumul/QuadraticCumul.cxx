//                                               -*- C++ -*-
/**
 *  @file  QuadraticCumul.cxx
 *  @brief QuadraticCumul implements the first and second order quadratic cumul formulas
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
#include <iomanip>
#include "QuadraticCumul.hxx"
#include "Threadable.hxx"
#include "CompositeRandomVector.hxx"
#include "NumericalMathFunction.hxx"
#include "Pointer.hxx"
#include "NumericalSample.hxx"
#include "PersistentObjectFactory.hxx"
#include "SensitivityAnalysis.hxx"

BEGIN_NAMESPACE_OPENTURNS


typedef Pointer<RandomVectorImplementation> Implementation;

CLASSNAMEINIT(QuadraticCumul);

static Factory<QuadraticCumul> RegisteredFactory("QuadraticCumul");

/*
 * @class QuadraticCumul
 * QuadraticCumul implements the mean and covariance of a random vector
 Y=G(X) by implementing the Taylor approximation of G
*/

/* Constructor with parameters */
QuadraticCumul::QuadraticCumul(const RandomVector & limitStateVariable, const String & name)
  : PersistentObject(name),
    limitStateVariable_(limitStateVariable),
    meanInputVector_(0),
    valueAtMean_(0),
    gradientAtMean_(0, 0),
    hessianAtMean_(0, 0),
    isAlreadyComputedValue_(false),
    isAlreadyComputedGradient_(false),
    isAlreadyComputedHessian_(false),
    isAlreadyComputedMeanFirstOrder_(false),
    isAlreadyComputedMeanSecondOrder_(false),
    isAlreadyComputedCovariance_(false),
    isAlreadyComputedImportanceFactors_(false),
    inputCovariance_(0),
    meanFirstOrder_(0),
    meanSecondOrder_(0),
    covariance_(0),
    importanceFactors_(0)
{
  /* Check if the given random vector is a composite random vector, which is mandatory */
  if (!limitStateVariable.isComposite()) throw InvalidArgumentException(HERE) << "Quadratic cumul algorithm requires a composite random vector as an input";
  /** Check if the given composite random vector is based on a function with a gradient implementation */
  if (!limitStateVariable.getImplementation()->getFunction().getGradientImplementation()->isActualImplementation()) throw InvalidArgumentException(HERE) << "Quadratic cumul algorithm requires a composite random vector based on a function with an actual gradient implementation";
  /** Check if the given composite random vector is based on a function with a gradient implementation */
  if (!limitStateVariable.getImplementation()->getFunction().getHessianImplementation()->isActualImplementation()) throw InvalidArgumentException(HERE) << "Quadratic cumul algorithm requires a composite random vector based on a function with an actual hessian implementation";
}

/* Virtual constructor */
QuadraticCumul * QuadraticCumul::clone() const
{
  return new QuadraticCumul(*this);
}

/* String converter */
String QuadraticCumul::__repr__() const
{
  return OSS() << "limitStateVariable=" << limitStateVariable_
               << " meanInputVector=" << meanInputVector_
               << " hessianAtMean=" << hessianAtMean_
               << " gradientAtMean=" << gradientAtMean_
               << " valueAtMean=" << valueAtMean_
               << " meanFirstOrder=" << meanFirstOrder_
               << " meanSecondOrder=" << meanSecondOrder_
               << " covariance=" << covariance_
               << " inputCovariance=" << inputCovariance_
               << " importanceFactors=" << importanceFactors_;
}

/* limitStateVariable accessor */
RandomVector QuadraticCumul::getLimitStateVariable() const
{
  return limitStateVariable_;
}

/* meanFirstOrder accessor */
NumericalPoint QuadraticCumul::getMeanFirstOrder() const
{
  if(!isAlreadyComputedMeanFirstOrder_) computeMeanFirstOrder();
  return meanFirstOrder_;
}


/* meanSecondOrder accessor */
NumericalPoint QuadraticCumul::getMeanSecondOrder() const
{
  if(!isAlreadyComputedMeanSecondOrder_) computeMeanSecondOrder();
  return meanSecondOrder_;
}

/* covariance accessor */
CovarianceMatrix QuadraticCumul::getCovariance() const
{
  if(!isAlreadyComputedCovariance_) computeCovariance();
  return covariance_;
}

/* importance factors accessor */
NumericalPointWithDescription QuadraticCumul::getImportanceFactors() const
{
  if(!isAlreadyComputedImportanceFactors_) computeImportanceFactors();
  return importanceFactors_;
}

/* Value at mean accessor */
NumericalPoint QuadraticCumul::getValueAtMean() const
{
  return valueAtMean_;
}

/* Gradient at mean accessor */
Matrix QuadraticCumul::getGradientAtMean() const
{
  return gradientAtMean_;
}

/* Hessian at mean accessor */
SymmetricTensor QuadraticCumul::getHessianAtMean() const
{
  return hessianAtMean_;
}

/* ImportanceFactors graph */
Graph QuadraticCumul::drawImportanceFactors() const
{
  // To ensure that the importance factors are up to date
  getImportanceFactors();
  OSS oss;
  oss << "Importance Factors from Quadratic Cumul - " << limitStateVariable_.getDescription()[0];
  return SensitivityAnalysis::DrawImportanceFactors(importanceFactors_, String(oss));
}

/* the function that computes the first order evaluation of the mean vector and the matrix covariance */
void QuadraticCumul::computeMeanFirstOrder() const
{
  if (!isAlreadyComputedValue_)
    {
      meanInputVector_ = limitStateVariable_.getImplementation()->getAntecedent()->getMean();
      valueAtMean_ = limitStateVariable_.getImplementation()->getFunction().operator()(meanInputVector_);
      isAlreadyComputedValue_ = true;
    }
  /* we compute here the meanFirstOrder_ */
  meanFirstOrder_ = valueAtMean_;
  isAlreadyComputedMeanFirstOrder_ = true;
} // QuadraticCumul::computeMeanFirstOrder()


/* the function that computes the first order evaluation of the mean vector and the matrix covariance */
void QuadraticCumul::computeCovariance() const
{
  inputCovariance_ = limitStateVariable_.getImplementation()->getAntecedent()->getCovariance();
  if (!isAlreadyComputedGradient_)
    {
      meanInputVector_ = limitStateVariable_.getImplementation()->getAntecedent()->getMean();
      gradientAtMean_ = limitStateVariable_.getImplementation()->getFunction().gradient(meanInputVector_);
      isAlreadyComputedGradient_ = true;
    }

  /* we compute here the covariance_
     hessianAtMean_ is symmetrical
     The following straightforward implementation does'nt work because OT has no way to discover
     that the resulting matrice is square and symmetric only by checking the type of the arguments
     setCovariance(getGradientAtMean().transpose() * getInputCovariance() * getGradientAtMean());
     We use an intermediate rectangular matrix, then we make an element-wise copy of this matrix into
     the output covariance matrix */

  /* a matrix, in fact a square symmetric matrix but we don't know it from its C++ definition */
  /* outputCovariance is a pxp matrix */
  const UnsignedLong inputDimension(gradientAtMean_.getNbRows());
  const UnsignedLong outputDimension(gradientAtMean_.getNbColumns());
  covariance_ = CovarianceMatrix(outputDimension);
  /* we unroll the matrix multiplications transpose(gradient).covariance.gradient */
  for (UnsignedLong i = 0; i < outputDimension; ++i)
    for (UnsignedLong j = 0; j < outputDimension; ++j)
      {
        covariance_(i, j) = 0.0;
        for (UnsignedLong k = 0; k < inputDimension; ++k)
          for (UnsignedLong l = 0; l < inputDimension; ++l)
            covariance_(i, j) += gradientAtMean_(l, i) * inputCovariance_(l, k) * gradientAtMean_(k, j);
      }
  /* We check that outputCovariance is really symmetric positive definite */
  if (!covariance_.isPositiveDefinite()) LOGWARN(OSS() << "Warning! The covariance matrix computed by quadratic cumul is not positive definite outputCovariance=" << covariance_);
  isAlreadyComputedCovariance_ = true;
} // QuadraticCumul::computeCovariance()

/* the function that computes the importance factors only in the scalar case */
void QuadraticCumul::computeImportanceFactors() const
{
  /* importance factors are evaluated only if the outputRandomVector is scalar */
  /* this case is tested with the  dimension of limitStateVariable_ : it must be equal to 1 in the right case */
  if (limitStateVariable_.getDimension() != 1) throw InvalidDimensionException(HERE) << "The importance factors are computed only if the output random vector is scalar";
  computeCovariance();
  /* we compute here the importance factors */
  /* in this scalar case, gradientAtMean is a NumericalPoint */
  const UnsignedLong dimension(gradientAtMean_.getNbRows());

  /* in this scalar case, importance factors is a NumericalPoint, defined as importanceFactors = gradient .* inputCovariance * gradient / outputCovariance, where .* means an element-wise multiplication between vectors */
  importanceFactors_ = NumericalPointWithDescription(dimension, 0.0);
  // This line looks strange, but it is here to ensure that the covariance has actually been computed
  getCovariance();
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      for (UnsignedLong j = 0; j < dimension; ++j) importanceFactors_[i] += inputCovariance_(i, j) * gradientAtMean_(j, 0);
      importanceFactors_[i] *= gradientAtMean_(i, 0) / covariance_(0, 0);
    }
  importanceFactors_.setDescription(limitStateVariable_.getImplementation()->getAntecedent()->getDescription());
  isAlreadyComputedImportanceFactors_ = true;
} // QuadraticCumul::computeImportanceFactors()

/* the function that computes the second order evaluation of the mean vector */
void QuadraticCumul::computeMeanSecondOrder() const
{
  inputCovariance_ = limitStateVariable_.getImplementation()->getAntecedent()->getCovariance();
  if (!isAlreadyComputedValue_)
    {
      meanInputVector_ = limitStateVariable_.getImplementation()->getAntecedent()->getMean();
      valueAtMean_ = limitStateVariable_.getImplementation()->getFunction().operator()(meanInputVector_);
      isAlreadyComputedValue_ = true;
    }
  if (!isAlreadyComputedHessian_)
    {
      meanInputVector_ = limitStateVariable_.getImplementation()->getAntecedent()->getMean();
      hessianAtMean_ = limitStateVariable_.getImplementation()->getFunction().hessian(meanInputVector_);
      isAlreadyComputedHessian_ = true;
    }
  /* we compute here the meanSecondOrder_ */
  /* tensoriel writting : setCovariance(getGradientAtMean() + 0.5*getHessianAtMean().dotdot(getInputCovariance())); */
  /* developped formula */

  const UnsignedLong rowDimension(hessianAtMean_.getNbRows());
  /* i */
  const UnsignedLong sheetDimension(hessianAtMean_.getNbSheets());
  /* k */
  meanSecondOrder_ = valueAtMean_;
  /* loop on k */
  for (UnsignedLong k = 0; k < sheetDimension; ++k)
    {
      NumericalScalar kSecondOrderContribution(0.0);
      /* loop on i */
      for (UnsignedLong i = 0; i < rowDimension; ++i)
        {
          kSecondOrderContribution += 0.5 * inputCovariance_(i, i) * hessianAtMean_(i, i, k);
          /* loop on j */
          for (UnsignedLong j = 0; j < i; ++j) kSecondOrderContribution += inputCovariance_(i, j) * hessianAtMean_(i, j, k);
        }/* end loop on i */
      meanSecondOrder_[k] += kSecondOrderContribution;
    } /* end loop on k */
  isAlreadyComputedMeanSecondOrder_ = true;
} // QuadraticCumul::computeMeanSecondOrder()

/* Method save() stores the object through the StorageManager */
void QuadraticCumul::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "limitStateVariable_", limitStateVariable_ );
  adv.saveAttribute( "meanInputVector_", meanInputVector_ );
  adv.saveAttribute( "valueAtMean_", valueAtMean_ );
  adv.saveAttribute( "gradientAtMean_", gradientAtMean_ );
  adv.saveAttribute( "hessianAtMean_", hessianAtMean_ );
  adv.saveAttribute( "isAlreadyComputedValue_", isAlreadyComputedValue_ );
  adv.saveAttribute( "isAlreadyComputedGradient_", isAlreadyComputedGradient_ );
  adv.saveAttribute( "isAlreadyComputedHessian_", isAlreadyComputedHessian_ );
  adv.saveAttribute( "isAlreadyComputedMeanFirstOrder_", isAlreadyComputedMeanFirstOrder_ );
  adv.saveAttribute( "isAlreadyComputedMeanSecondOrder_", isAlreadyComputedMeanSecondOrder_ );
  adv.saveAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.saveAttribute( "isAlreadyComputedImportanceFactors_", isAlreadyComputedImportanceFactors_ );
  adv.saveAttribute( "inputCovariance_", inputCovariance_ );
  adv.saveAttribute( "meanFirstOrder_", meanFirstOrder_ );
  adv.saveAttribute( "meanSecondOrder_", meanSecondOrder_ );
  adv.saveAttribute( "covariance_", covariance_ );
  adv.saveAttribute( "importanceFactors_", importanceFactors_ );
}

/* Method load() reloads the object from the StorageManager */
void QuadraticCumul::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "limitStateVariable_", limitStateVariable_ );
  adv.loadAttribute( "meanInputVector_", meanInputVector_ );
  adv.loadAttribute( "valueAtMean_", valueAtMean_ );
  adv.loadAttribute( "gradientAtMean_", gradientAtMean_ );
  adv.loadAttribute( "hessianAtMean_", hessianAtMean_ );
  adv.loadAttribute( "isAlreadyComputedValue_", isAlreadyComputedValue_ );
  adv.loadAttribute( "isAlreadyComputedGradient_", isAlreadyComputedGradient_ );
  adv.loadAttribute( "isAlreadyComputedHessian_", isAlreadyComputedHessian_ );
  adv.loadAttribute( "isAlreadyComputedMeanFirstOrder_", isAlreadyComputedMeanFirstOrder_ );
  adv.loadAttribute( "isAlreadyComputedMeanSecondOrder_", isAlreadyComputedMeanSecondOrder_ );
  adv.loadAttribute( "isAlreadyComputedCovariance_", isAlreadyComputedCovariance_ );
  adv.loadAttribute( "isAlreadyComputedImportanceFactors_", isAlreadyComputedImportanceFactors_ );
  adv.loadAttribute( "inputCovariance_", inputCovariance_ );
  adv.loadAttribute( "meanFirstOrder_", meanFirstOrder_ );
  adv.loadAttribute( "meanSecondOrder_", meanSecondOrder_ );
  adv.loadAttribute( "covariance_", covariance_ );
  adv.loadAttribute( "importanceFactors_", importanceFactors_ );
}

END_NAMESPACE_OPENTURNS
