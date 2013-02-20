//                                               -*- C++ -*-
/**
 *  @file  Mixture.cxx
 *  @brief Abstract top-level class for all Mixtures
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
#include <cmath>
#include "Mixture.hxx"
#include "Log.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

typedef UserDefined::UserDefinedPairCollection UserDefinedPairCollection;

CLASSNAMEINIT(Mixture);

static Factory<Mixture> RegisteredFactory("Mixture");

/* Default constructor */
Mixture::Mixture()
  : DistributionImplementation("Mixture")
  , distributionCollection_(1)
  , weightsDistribution_(UserDefined())
{
  // Set an empty range
  setRange(Interval(1.0, 0.0));
}

/* Parameters constructor */
Mixture::Mixture(const DistributionCollection & coll)
  : DistributionImplementation("Mixture")
  , distributionCollection_()
  , weightsDistribution_()
{
  // We could NOT set distributionCollection_ in the member area of the constructor
  // because we must check before if the collection is valid (ie, if all the
  // distributions of the collection have the same dimension). We do this by calling
  // the setDistributionCollection() method that do it for us.
  // This call set also the range.
  setDistributionCollection( coll );
}

/* Parameters constructor */
Mixture::Mixture(const DistributionCollection & coll,
                 const NumericalPoint & weights)
  : DistributionImplementation("Mixture")
  , distributionCollection_()
  , weightsDistribution_()
{
  if (coll.getSize() != weights.getSize()) throw InvalidArgumentException(HERE) << "Error: the weight size " << weights.getSize() << " must be equal to the distribution collection size " << coll.getSize();
  // We could NOT set distributionCollection_ in the member area of the constructor
  // because we must check before if the collection is valid (ie, if all the
  // distributions of the collection have the same dimension). We do this by calling
  // the setDistributionCollection() method that do it for us.
  // This call set also the range.
  setDistributionCollectionWithWeights( coll, weights );
}

/* Comparison operator */
Bool Mixture::operator ==(const Mixture & other) const
{
  if (this == &other) return true;
  return distributionCollection_ == other.distributionCollection_;
}

/* String converter */
String Mixture::__repr__() const
{
  OSS oss;
  oss << "class=" << Mixture::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " distributionCollection=" << distributionCollection_
      << " weightsDistribution=" << weightsDistribution_;
  return oss;
}

String Mixture::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(";
  String separator("");
  for (UnsignedLong i = 0; i < distributionCollection_.getSize(); ++i)
    {
      oss << separator << "(w = " << distributionCollection_[i].getWeight() << ", d = " << distributionCollection_[i].__str__();
      separator = ", ";
    }
  oss << ")";
  return oss;
}

/* Weights accessor */
NumericalPoint Mixture::getWeights() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  NumericalPoint weights(size);
  for (UnsignedLong i = 0; i < size; ++i) weights[i] = distributionCollection_[i].getWeight();
  return weights;
}

void Mixture::setWeights(const NumericalPoint & weights)
{
  const DistributionCollection coll(distributionCollection_);
  setDistributionCollectionWithWeights( coll, weights);
}


/* Compute the numerical range of the distribution given the parameters values */
void Mixture::computeRange()
{
  const UnsignedLong size(distributionCollection_.getSize());
  if (size == 0) return;
  Interval range(distributionCollection_[0].getRange());
  for (UnsignedLong i = 1; i < size; ++i) range = range.join(distributionCollection_[i].getRange());
  setRange(range);
}

/* Weights distribution accessor */
void Mixture::setWeightsDistribution(const UserDefined & weightsDistribution)
{
  weightsDistribution_ = weightsDistribution;
}

UserDefined Mixture::getWeightsDistribution() const
{
  return weightsDistribution_;
}


/* Distribution collection accessor */
void Mixture::setDistributionCollection(const DistributionCollection & coll)
{
  const UnsignedLong size(coll.getSize());
  NumericalPoint weights(size);
  for (UnsignedLong i = 0; i < size; ++i) weights[i] = coll[i].getWeight();
  setDistributionCollectionWithWeights(coll, weights);
}

void Mixture::setDistributionCollectionWithWeights(const DistributionCollection & coll,
                                                   const NumericalPoint & weights)
{
  // Not const because the collection will be simplified and its size reduced
  UnsignedLong size(coll.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Mixture based on an empty distribution collection.";
  if (weights.getSize() != size) throw InvalidArgumentException(HERE) << "Error: the number of weights=" << weights.getSize() << " is different from the number of distributions=" << size << ".";
  NumericalScalar maximumWeight(weights[0]);
  NumericalScalar weightSum(maximumWeight);
  UnsignedLong dimension(coll[0].getDimension());
  // First loop, check the atoms dimensions and the weigths values
  for(UnsignedLong i = 1; i < size; ++i)
    {
      if (dimension != coll[i].getDimension())
        // We throw an exception because the collection has distributions of different sizes
        throw InvalidArgumentException(HERE) << "Collection of distributions has distributions of different dimensions";
      NumericalScalar w(weights[i]);
      if (w < 0.0) throw InvalidArgumentException(HERE) << "Distribution " << i << " has a negative weight, w=" << w;
      if (w > maximumWeight) maximumWeight = w;
      weightSum += w;
    } /* end for */
  const NumericalScalar smallWeight(ResourceMap::GetAsNumericalScalar("Mixture-SmallWeight"));
  if (weightSum < smallWeight)
    // We throw an exception because the collection of distributions has only distributions with small weight: they cannot be renormalized
    throw InvalidArgumentException(HERE) << "Collection of distributions has atoms with too small total weight=" << weightSum << " for a threshold equals to Mixture-SmallWeight=" << smallWeight;
  // Second loop, keep only the atoms with a significant weight and update the sum
  weightSum = 0.0;
  distributionCollection_ = DistributionCollection(0);
  for(UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar w(weights[i]);
      if (w < smallWeight * maximumWeight)
        {
          LOGINFO(OSS() << "Warning! The distribution number " << i << " has a too small weight=" << w << " for a relative threshold equals to Mixture-SmallWeight=" << smallWeight << " with respect to the maximum weight=" << maximumWeight << ". It is removed from the collection.");
        }
      else
        {
          // Set the original weight into the collection as it will be reused from here in the normalization step
          Distribution atom(coll[i]);
          atom.setWeight(w);
          distributionCollection_.add(atom);
          weightSum += w;
        }
    }

  // Update the size of the collection as null-weighted distributions could have been dismissed
  size = distributionCollection_.getSize();

  // We set the member with the collection passed as argument and we renormalize it in place
  UserDefinedPairCollection pairCollection(size, UserDefinedPair(NumericalPoint(1, 0.0), 0.0));
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar normalizedWeight(distributionCollection_[i].getWeight() / weightSum);
      distributionCollection_[i].setWeight(normalizedWeight);
      pairCollection[i] = UserDefinedPair(NumericalPoint(1, NumericalScalar(i)), normalizedWeight);
    } /* end for */
  setWeightsDistribution(UserDefined(pairCollection));
  setDimension(dimension);
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  isAlreadyCreatedGeneratingFunction_ = false;
  computeRange();
}


/* Distribution collection accessor */
const Mixture::DistributionCollection & Mixture::getDistributionCollection() const
{
  return distributionCollection_;
}

/* Virtual constructor */
Mixture * Mixture::clone() const
{
  return new Mixture(*this);
}


/* Get one realization of the Mixture */
NumericalPoint Mixture::getRealization() const
{
  // Select the atom following the weightsDistribution
  const UnsignedLong index((UnsignedLong)(round(weightsDistribution_.getRealization()[0])));
  return distributionCollection_[index].getRealization();
}


/* Get the DDF of the Mixture */
NumericalPoint Mixture::computeDDF(const NumericalPoint & point) const
{
  NumericalPoint ddfValue(getDimension(), 0.0);
  if (!getRange().numericallyContains(point)) return ddfValue;
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) ddfValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computeDDF(point);
  return ddfValue;
}

/* Get the PDF of the Mixture */
NumericalScalar Mixture::computePDF(const NumericalPoint & point) const
{
  NumericalScalar pdfValue(0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  if (!getRange().numericallyContains(point)) return pdfValue;
  for(UnsignedLong i = 0; i < size; ++i) pdfValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computePDF(point);
  return pdfValue;
}

/* Get the CDF of the Mixture */
NumericalScalar Mixture::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar cdfValue(0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) cdfValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computeCDF(point);
  return cdfValue;
}

/* Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
NumericalComplex Mixture::computeCharacteristicFunction(const NumericalScalar x) const
{
  NumericalComplex cfValue(0.0);
  UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) cfValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computeCharacteristicFunction(x);
  return cfValue;
}

/* Get the PDF gradient of the distribution */
NumericalPoint Mixture::computePDFGradient(const NumericalPoint & point) const
{
  NumericalPoint pdfGradientValue;
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) pdfGradientValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computePDFGradient(point);
  return pdfGradientValue;
}

/* Get the CDF gradient of the distribution */
NumericalPoint Mixture::computeCDFGradient(const NumericalPoint & point) const
{
  NumericalPoint cdfGradientValue(getDimension(), 0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) cdfGradientValue += distributionCollection_[i].getWeight() * distributionCollection_[i].computeCDFGradient(point);
  return cdfGradientValue;
}

/* Get the i-th marginal distribution */
Mixture::Implementation Mixture::getMarginal(const UnsignedLong i) const
{
  const UnsignedLong dimension(getDimension());
  if (i >= dimension) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  DistributionCollection collection;
  const UnsignedLong size(distributionCollection_.getSize());
  for (UnsignedLong index = 0; index < size; ++index)
    {
      collection.add(distributionCollection_[index].getMarginal(i));
      collection[index].setWeight(distributionCollection_[index].getWeight());
    }
  return new Mixture(collection);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
Mixture::Implementation Mixture::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  DistributionCollection collection;
  const UnsignedLong size(distributionCollection_.getSize());
  for (UnsignedLong index = 0; index < size; ++index)
    {
      collection.add(distributionCollection_[index].getMarginal(indices));
      collection[index].setWeight(distributionCollection_[index].getWeight());
    }
  return new Mixture(collection);
}

/* Compute the mean of the Mixture */
void Mixture::computeMean() const
{
  mean_ = NumericalPoint(getDimension(), 0.0);
  const UnsignedLong size(distributionCollection_.getSize());
  for(UnsignedLong i = 0; i < size; ++i) mean_ += distributionCollection_[i].getWeight() * distributionCollection_[i].getMean();
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the Mixture */
void Mixture::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  // To insure a zero initialization
  covariance_ = CovarianceMatrix(dimension);
  for (UnsignedLong i = 0; i < dimension; ++i) covariance_(i, i) = 0.0;
  const UnsignedLong size(distributionCollection_.getSize());
  // First, compute E(X.X^t)
  for(UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar weightI(distributionCollection_[i].getWeight());
      const CovarianceMatrix covarianceI(distributionCollection_[i].getCovariance());
      const NumericalPoint meanI(distributionCollection_[i].getMean());
      for(UnsignedLong row = 0; row < dimension; ++row)
        for(UnsignedLong column = 0; column <= row; ++column)
          covariance_(row, column) += weightI * (covarianceI(row, column) + meanI[row] * meanI[column]);
    } /* end for */
  // Then, substract E(X).E(X)^t
  const NumericalPoint mean(getMean());
  for(UnsignedLong row = 0; row < dimension; ++row)
    for(UnsignedLong column = 0; column <= row; ++column)
      covariance_(row, column) -= mean[row] * mean[column];
  isAlreadyComputedCovariance_ = true;
}

/** Parameters value and description accessor */
Mixture::NumericalPointWithDescriptionCollection Mixture::getParametersCollection() const
{
  const UnsignedLong dimension(getDimension());
  const UnsignedLong size(distributionCollection_.getSize());
  // Special case for dimension=1
  if (dimension == 1)
    {
      NumericalPointWithDescriptionCollection parameters(1);
      Description description;
      // Form a big NumericalPoint from the parameters of each atom and its weight
      for (UnsignedLong i = 0; i < size; ++i)
        {
          const NumericalPointWithDescription atomParameters(distributionCollection_[i].getParametersCollection()[0]);
          const Description atomDescription(atomParameters.getDescription());
          const UnsignedLong atomParameterDimension(atomParameters.getDimension());
          // Add the current atom parameters
          parameters[0].add(distributionCollection_[i].getWeight());
          description.add(OSS() << "w_" << i);
          for (UnsignedLong j = 0; j < atomParameterDimension; ++j)
            {
              parameters[0].add(atomParameters[j]);
              description.add(atomDescription[j]);
            }
        }
      parameters[0].setDescription(description);
      parameters[0].setName(getName());
      return parameters;
    }
  // General case
  NumericalPointWithDescriptionCollection parameters(dimension + 1);
  Description description;
  // First put the marginal parameters
  for (UnsignedLong marginalIndex = 0; marginalIndex < dimension; ++marginalIndex)
    {
      // Each marginal distribution must output a collection of parameters of size 1, even if it contains an empty NumericalPoint
      const NumericalPointWithDescriptionCollection marginalParameters(distributionCollection_[marginalIndex].getParametersCollection());
      NumericalPointWithDescription point(marginalParameters[0]);
      point.setName(distributionCollection_[marginalIndex].getName());
      parameters[marginalIndex] = point;
    } // marginalIndex

  // Form a big NumericalPoint from the dependence parameters of each atom
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPointWithDescription atomDependenceParameters(distributionCollection_[i].getParametersCollection()[dimension]);
      const Description atomDescription(atomDependenceParameters.getDescription());
      const UnsignedLong atomParameterDimension(atomDependenceParameters.getDimension());
      const String prefix(OSS() << "atom_" << i << "_");
      // Add the current atom dependence parameters
      for (UnsignedLong j = 0; j < atomParameterDimension; j++)
        {
          parameters[dimension].add(atomDependenceParameters[j]);
          description.add(prefix + atomDescription[j]);
        }
    }
  parameters[dimension].setDescription(description);
  parameters[dimension].setName("dependence");
  return parameters;
} // getParametersCollection

/* Check if the distribution is elliptical */
Bool Mixture::isElliptical() const
{
  // If there is only one atom
  if (distributionCollection_.getSize() == 1) return distributionCollection_[0].getImplementation()->isElliptical();
  return false;
}

/* Check if the distribution is continuous */
Bool Mixture::isContinuous() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  Bool flag(distributionCollection_[0].getImplementation()->isContinuous());
  for (UnsignedLong i = 1; i < size; ++i)
    {
      flag = flag && distributionCollection_[i].getImplementation()->isContinuous();
      if (!flag) return false;
    }
  return true;
}

/* Check if the distribution is integral */
Bool Mixture::isIntegral() const
{
  const UnsignedLong size(distributionCollection_.getSize());
  Bool flag(distributionCollection_[0].getImplementation()->isIntegral());
  for (UnsignedLong i = 1; i < size; ++i)
    {
      flag = flag && distributionCollection_[i].getImplementation()->isIntegral();
      if (!flag) return false;
    }
  return true;
}

/* Tell if the distribution has elliptical copula */
Bool Mixture::hasEllipticalCopula() const
{
  // In 1D, all the distributions have an elliptical copula
  if (getDimension() == 1) return true;
  // If there is only one atom, the mixture has the same properties as this atom
  if (distributionCollection_.getSize() == 1) return distributionCollection_[0].getImplementation()->hasEllipticalCopula();
  // General case
  return false;
}

/* Tell if the distribution has independent copula */
Bool Mixture::hasIndependentCopula() const
{
  // In 1D, all the distributions have an independent copula
  if (getDimension() == 1) return true;
  // If there is only one atom, the mixture has the same properties as this atom
  if (distributionCollection_.getSize() == 1) return distributionCollection_[0].getImplementation()->hasIndependentCopula();
  // General case
  return false;
}

/* Method save() stores the object through the StorageManager */
void Mixture::save(Advocate & adv) const
{
  DistributionImplementation::save(adv);
  adv.saveAttribute( "distributionCollection_", distributionCollection_ );
  adv.saveAttribute( "weightsDistribution_", weightsDistribution_ );
}

/* Method load() reloads the object from the StorageManager */
void Mixture::load(Advocate & adv)
{
  DistributionImplementation::load(adv);
  adv.loadAttribute( "distributionCollection_", distributionCollection_ );
  adv.loadAttribute( "weightsDistribution_", weightsDistribution_ );
  // To compute the range
  setDistributionCollection(distributionCollection_);
}

END_NAMESPACE_OPENTURNS
