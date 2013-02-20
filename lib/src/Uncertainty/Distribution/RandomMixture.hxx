//                                               -*- C++ -*-
/**
 *  @file  RandomMixture.hxx
 *  @brief The class that implements randomMixtures
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
 *  Id      RandomMixture.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_RANDOMMIXTURE_HXX
#define OPENTURNS_RANDOMMIXTURE_HXX

#include "Distribution.hxx"
#include "DistributionFactory.hxx"
#include "DistributionImplementation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "SpecFunc.hxx"
#include "Normal.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class RandomMixture
 *
 * The class describes the probabilistic concept of RandomMixture.
 */
class RandomMixture
  : public DistributionImplementation
{
  CLASSNAME;
public:

  typedef Collection<Distribution>               DistributionCollection;
  typedef PersistentCollection<Distribution>     DistributionPersistentCollection;
  typedef PersistentCollection<NumericalComplex> NumericalComplexPersistentCollection;
  typedef Collection<DistributionFactory>        DistributionFactoryCollection;


  /** Parameter constructor */
  explicit RandomMixture(const DistributionCollection & coll,
                         const NumericalScalar constant = 0.0);

  /** Parameter constructor */
  explicit RandomMixture(const DistributionCollection & coll,
                         const NumericalPoint & weights,
                         const NumericalScalar constant = 0.0);


  /** Comparison operator */
  Bool operator ==(const RandomMixture & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /** Distribution collection accessor */
  void setDistributionCollection(const DistributionCollection & coll);
  const DistributionCollection & getDistributionCollection() const;

  /** Constant accessor */
  void setConstant(const NumericalScalar constant);
  NumericalScalar getConstant() const;



  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual RandomMixture * clone() const;

  /** Get one realization of the RandomMixture */
  NumericalPoint getRealization() const;

  /** Get the DDF of the RandomMixture */
  using DistributionImplementation::computeDDF;
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the RandomMixture */
  using DistributionImplementation::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Compute the PDF over a regular grid */
  NumericalSample computePDF(const NumericalScalar xMin,
                             const NumericalScalar xMax,
                             const UnsignedLong pointNumber,
                             const NumericalScalar precision = ResourceMap::GetAsNumericalScalar( "RandomMixture-GraphPDFEpsilon" )) const;

protected:

private:

  /** Quantile computation for dimension=1 */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar( "DistributionImplementation-DefaultQuantileEpsilon" )) const;

  /** Compute the characteristic function of 1D distributions by difference to a reference Normal distribution with the same mean and the same standard deviation in a regular pattern with cache */
  NumericalComplex computeDeltaCharacteristicFunction(const UnsignedLong index) const;

public:
  /** Get the CDF of the RandomMixture */
  using DistributionImplementation::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;
  using DistributionImplementation::computeComplementaryCDF;
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Compute the CDF over a regular grid */
  NumericalSample computeCDF(const NumericalScalar xMin,
                             const NumericalScalar xMax,
                             const UnsignedLong pointNumber,
                             const NumericalScalar precision = ResourceMap::GetAsNumericalScalar( "RandomMixture-GraphCDFEpsilon" )) const;

  /** Get the probability content of an interval */
  NumericalScalar computeProbability(const Interval & interval) const;

  /** Compute the quantile over a regular grid */
  using DistributionImplementation::computeQuantile;
  NumericalSample computeQuantile(const NumericalScalar qMin,
                                  const NumericalScalar qMax,
                                  const UnsignedLong pointNumber,
                                  const NumericalScalar precision = ResourceMap::GetAsNumericalScalar( "RandomMixture-GraphCDFEpsilon" ),
                                  const Bool tail = false) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  using DistributionImplementation::computeCharacteristicFunction;
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;
  using DistributionImplementation::computeLogCharacteristicFunction;
  NumericalComplex computeLogCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDF gradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;

  /** Weights distribution accessor */
  void setWeights(const NumericalPoint & weights);
  NumericalPoint getWeights() const;

  /** Get a positon indicator for a 1D distribution */
  NumericalScalar getPositionIndicator() const;

  /** Get a dispersion indicator for a 1D distribution */
  NumericalScalar getDispersionIndicator() const;

  /** BlockMin accessor */
  void setBlockMin(const UnsignedLong blockMin);
  UnsignedLong getBlockMin() const;

  /** BlockMax accessor */
  void setBlockMax(const UnsignedLong blockMax);
  UnsignedLong getBlockMax() const;

  /** MaxSize accessor */
  void setMaxSize(const UnsignedLong maxSize);
  UnsignedLong getMaxSize() const;

  /** Alpha accessor */
  void setAlpha(const NumericalScalar alpha);
  NumericalScalar getAlpha() const;

  /** Beta accessor */
  void setBeta(const NumericalScalar beta);
  NumericalScalar getBeta() const;

  /** Reference bandwidth accessor */
  void setReferenceBandwidth(const NumericalScalar bandwidth);
  NumericalScalar getReferenceBandwidth() const;

  /** PDF epsilon accessor. For other distributions, it is a read-only attribute. */
  void setPDFPrecision(const NumericalScalar pdfPrecision);

  /** CDF epsilon accessor. For other distributions, it is a read-only attribute. */
  void setCDFPrecision(const NumericalScalar cdfPrecision);

  /** Project a RandomMixture distribution over a collection of DistributionFactory by using sampling and Kolmogorov distance. */
  DistributionCollection project(const DistributionFactoryCollection & factoryCollection,
                                 NumericalPoint & kolmogorovNorm,
                                 const UnsignedLong size = ResourceMap::GetAsUnsignedLong( "RandomMixture-ProjectionDefaultSize" )) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  class KolmogorovProjection
  {
  public:
    /** Constructor from a distribution and a data set */
    KolmogorovProjection(const NumericalSample & data,
                         const DistributionFactory & factory):
      data_(data),
      factory_(factory) {};

    /** Compute the Kolmogorov distance based on the given data, for a given parameter set */
    NumericalPoint computeNorm(const NumericalPoint & parameters) const
    {
      NumericalScalar norm(0.0);
      try
        {
          const Distribution candidate(factory_.build(NumericalPointCollection(1, parameters)));
          for (UnsignedLong i = 0; i < data_.getSize(); ++i)
            norm += pow(candidate.computeCDF(data_[i][0]) - data_[i][1], 2);
          return NumericalPoint(1, norm);
        }
      catch(...)
        {
          return NumericalPoint(1, SpecFunc::MaxNumericalScalar);
        }
    }

    /** factory accessor */
    void setDistributionFactory(const DistributionFactory & factory)
    {
      factory_ = factory;
    }
  private:
    NumericalSample data_;
    DistributionFactory factory_;
  };

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** Compute the safety coefficient for the Poisson algorithm. See documentation for its meaning. */
  void computeBeta();

  /** Compute the left-hand sum in Poisson's summation formula for the equivalent normal */
  NumericalScalar computeEquivalentNormalPDFSum(const NumericalScalar x) const;
  NumericalScalar computeEquivalentNormalCDFSum(const NumericalScalar s,
                                                const NumericalScalar t) const;

  /** Default constructor for save/load mechanism */
  RandomMixture() {};
  friend class Factory<RandomMixture>;

public:
  /** Get the mean of a randomMixture */
  void computeMean() const;

  /** Get the covariance of a randomMixture */
  void computeCovariance() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Compute the position indicator */
  void computePositionIndicator() const;

  /** Compute the dispersion indicator */
  void computeDispersionIndicator() const;

private:
  /** Adjust a given bandwidth with respect to a reference bandwidth,
      in order to be an integral divisor or multiple of the reference bandwidth */
  NumericalScalar adjustBandwidth(const NumericalScalar bandwidth) const;

  /** Compute the reference bandwidth. It is defined as the maximum bandwidth
      that allow a precise computation of the PDF over the range
      [positionIndicator_ +/- beta * dispersionIndicator_] */
  void computeReferenceBandwidth();

  /** Compute the equivalent normal distribution, i.e. with the same mean and
      the same standard deviation */
  void computeEquivalentNormal();

  /** The collection of distribution of the randomMixture */
  DistributionPersistentCollection distributionCollection_;

  /** The constant term of the mixture */
  NumericalScalar constant_;

  /** Position indicator */
  mutable NumericalScalar positionIndicator_;
  mutable Bool isAlreadyComputedPositionIndicator_;

  /** Dispersion indicator */
  mutable NumericalScalar dispersionIndicator_;
  mutable Bool isAlreadyComputedDispersionIndicator_;

  /** Minimum number of blocks to consider for PDF and CDF computation */
  UnsignedLong blockMin_;

  /** Maximum number of blocks to consider for PDF and CDF computation */
  UnsignedLong blockMax_;

  /** Reference bandwidth */
  NumericalScalar referenceBandwidth_;

  /** Maximum size of the cache for the CharacteristicFunction values */
  UnsignedLong maxSize_;

  /** Index of the top of the cache */
  mutable UnsignedLong storedSize_;

  /** Cache for the characteristic function values */
  mutable NumericalComplexPersistentCollection characteristicValuesCache_;

  /** A priori range of PDF and CDF argument expressed in dispersionIndicator units */
  NumericalScalar alpha_;

  /** Distance from the boundary of the a priori range at which the PDF is negligible */
  NumericalScalar beta_;

  /** Requested precision for PDF computation */
  mutable NumericalScalar pdfPrecision_;

  /** Requested precision for CDF computation */
  mutable NumericalScalar cdfPrecision_;

  /** Normal distribution with the same mean and standard deviation than the RandomMixture */
  Normal equivalentNormal_;

}; /* class RandomMixture */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_RANDOMMIXTURE_HXX */
