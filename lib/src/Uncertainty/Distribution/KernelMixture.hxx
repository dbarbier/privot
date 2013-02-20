//                                               -*- C++ -*-
/**
 *  @file  KernelMixture.hxx
 *  @brief The class that implements kernelMixtures
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
 *  Id      KernelMixture.hxx 2608 2012-07-16 13:59:45Z schueller
 */
#ifndef OPENTURNS_KERNELMIXTURE_HXX
#define OPENTURNS_KERNELMIXTURE_HXX

#include "Distribution.hxx"
#include "NonEllipticalDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class KernelMixture
 *
 * The class describes the probabilistic concept of KernelMixture.
 */
class KernelMixture
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  /** Default constructor */
  explicit KernelMixture(const Distribution & kernel,
                         const NumericalPoint & bandwidth,
                         const NumericalSample & sample);


  /** Comparison operator */
  Bool operator ==(const KernelMixture & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;


  /** Kernel accessor */
  void setKernel(const Distribution & kernel);
  Distribution getKernel() const;

  /** Bandwidth accessor */
  void setBandwidth(const NumericalPoint & bandwidth);
  NumericalPoint getBandwidth() const;

  /** Sample accessor */
  void setInternalSample(const NumericalSample & sample);
  NumericalSample getInternalSample() const;


  /* Here is the interface that all derived class must implement */

  /** Virtual constructor */
  virtual KernelMixture * clone() const;

  /** Get one realization of the KernelMixture */
  virtual NumericalPoint getRealization() const;

  /** Get the DDF of the KernelMixture */
  using NonEllipticalDistribution::computeDDF;
  virtual NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the KernelMixture */
  using NonEllipticalDistribution::computePDF;
  virtual NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the KernelMixture */
  using NonEllipticalDistribution::computeCDF;
  virtual NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the characteristic function of the distribution, i.e. phi(u) = E(exp(I*u*X)) */
  NumericalComplex computeCharacteristicFunction(const NumericalScalar x) const;

  /** Get the PDF gradient of the distribution */
  virtual NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDF gradient of the distribution */
  virtual NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the i-th marginal distribution */
  Implementation getMarginal(const UnsignedLong i) const;

  /** Get the distribution of the marginal distribution corresponding to indices dimensions */
  Implementation getMarginal(const Indices & indices) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Parameters value and description accessor */
  virtual NumericalPointWithDescriptionCollection getParametersCollection() const;

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is constinuous */
  Bool isContinuous() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  /** Compute the numerical range of the distribution given the parameters values */
  void computeRange();

  /** For save/load mechanism */
  KernelMixture() {};
  friend class Factory<KernelMixture>;

  /** Get the mean of a kernelMixture */
  void computeMean() const;

  /** Get the covariance of a kernelMixture */
  void computeCovariance() const;

  /** Compute the normalization factor */
  void computeNormalizationFactor();

  /** The kernel of the kernelMixture */
  Distribution kernel_;

  /** The bandwidth of the KernelMixture */
  NumericalPoint bandwidth_;

  /** The componentwise bandwidth inverse of the KernelMixture */
  NumericalPoint bandwidthInverse_;

  /** The (n\product_{k=1}^{dim}h_k)^{-1} normalization factor */
  NumericalScalar normalizationFactor_;

  /** The sample of the kernelMixture */
  NumericalSample sample_;

}; /* class KernelMixture */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KERNELMIXTURE_HXX */
