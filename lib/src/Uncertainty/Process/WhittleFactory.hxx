//                                               -*- C++ -*-
/**
 *  @file  WhittleFactory.hxx
 *  @brief The class enables to get the coefficients of an ARMA process using its spectral density function
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
 *  Id      WhittleFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_WHITTLEFACTORY_HXX
#define OPENTURNS_WHITTLEFACTORY_HXX

#include "ARMAFactoryImplementation.hxx"
#include "SpectralModelFactory.hxx"
#include "WhittleFactoryState.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class WhittleFactory
 *
 * The class implements the Whittle likelihood for estimating ARMA coefficients
 */
class WhittleFactory
  : public ARMAFactoryImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  WhittleFactory(const String & name = DefaultName);

  /** Default constructor */
  WhittleFactory(const UnsignedLong p,
                 const UnsignedLong q,
                 const Bool invertible = true,
                 const String & name = DefaultName);

  /** Default constructor */
  WhittleFactory(const Indices & p,
                 const Indices & q,
                 const Bool invertible = true,
                 const String & name = DefaultName);

  /** Virtual constructor */
  WhittleFactory * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** SpectralModelFactory acccessors - Make easy change of periodogram estimate */
  SpectralModelFactory getSpectralModelFactory() const;
  void setSpectralModelFactory(const SpectralModelFactory & factory);

  /** Build method ==> estimating the coefficients */
  ARMA * build(const TimeSeries & timeSeries,
               NumericalPoint & informationCriteria) const;
  ARMA * build(const TimeSeries & timeSeries) const;
  ARMA * build(const ProcessSample & sample,
               NumericalPoint & informationCriteria) const;
  ARMA * build(const ProcessSample & sample) const;

  /** Verbosity accessor */
  Bool getVerbose() const;
  void setVerbose(const Bool verbose);

  /** Enable or disable the estimation history */
  void enableHistory() const;
  void disableHistory() const;
  Bool isHistoryEnabled() const;
  void resetHistory() const;
  Collection< WhittleFactoryState > getHistory() const;

  /** Starting points accessor */
  void setStartingPoints(const Collection< NumericalPoint > & startingPoints);
  Collection< NumericalPoint > getStartingPoints() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private :

  /** Likelihood function ==> Compute the reduced form of the likelihood */
  NumericalScalar computeLogLikelihood(const NumericalPoint & theta) const;

  /** Log likelihood maximization, shared among the different build() methods */
  ARMA * maximizeLogLikelihood(NumericalPoint & InformationCriterion) const;

  /** Compute the spectral density at the relevant normalized frequencies */
  void computeSpectralDensity(const SpectralModel & spectralModel) const;

  /** Estimate the periodogram */
  void buildSpectralDensity(const TimeSeries & timeSeries) const;
  void buildSpectralDensity(const ProcessSample & sample) const;

  /** Wrapper function passed to cobyla */
  static int ComputeObjectiveAndConstraint( int n,
                                            int m,
                                            double * x,
                                            double * f,
                                            double * con,
                                            void * state);

  /** Initialize the starting points using the ResourceMap */
  void initializeStartingPoints();

  /** Spectral model factory ==> enables user to set anotherparametrized method*/
  SpectralModelFactory spectralFactory_;

  /** Frequency grid to be used in internal */
  mutable NumericalPoint normalizedFrequencies_;

  /** Time grid associated with the given data */
  mutable RegularGrid timeGrid_;

  /** Number of frequecies - Used to pass data to be used in computeLogLikeliHood */
  mutable UnsignedLong m_;

  /** only used to pass data to be used in computeLogLikeliHood */
  mutable NumericalPoint spectralDensity_;

  /** only used to pass data to be used in computeLogLikeliHood */
  mutable NumericalScalar sigma2_;

  /** Verbosity control */
  Bool verbose_;

  /** History flag */
  mutable Bool isHistoryEnabled_;

  /** History */
  mutable PersistentCollection< WhittleFactoryState > history_;

  /** Starting points for the estimation process */
  PersistentCollection< NumericalPoint > startingPoints_;
}; /* class WhittleFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WHITTLEFACTORY_HXX */
