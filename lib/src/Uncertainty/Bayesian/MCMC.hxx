//                                               -*- C++ -*-
/**
 *  @file  MCMC.hxx
 *  @brief MCMC updates the chain
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */
#ifndef OPENTURNS_MCMC_HXX
#define OPENTURNS_MCMC_HXX

#include "OTprivate.hxx"
#include "SamplerImplementation.hxx"
#include "ResourceMap.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class MCMC
 *
 * @brief The class implements the concept of comparison operator defined in Sampler.
 *
 * This class is abstract so it can not be instanciated. It must be derived.
 * @see Sampler
 */
class MCMC
  : public SamplerImplementation
{
  CLASSNAME;
public:
  typedef Collection<NumericalPoint> NumericalPointCollection;
  typedef PersistentCollection<NumericalPoint> NumericalPointPersistentCollection;
  typedef Collection<NumericalPointWithDescription> NumericalPointWithDescriptionCollection;


  /** Default constructor */
  MCMC();

  /** Constructor with parameters*/
  MCMC(const Distribution & prior,
       const Distribution & conditional,
       const NumericalSample & observations,
       const NumericalPoint & initialState);

  /** Constructor with parameters*/
  MCMC(const Distribution & prior,
       const Distribution & conditional,
       const NumericalMathFunction & model,
       const NumericalSample & observations,
       const NumericalPoint & initialState);

  /** String converter */
  virtual String __repr__() const;

  /** Compute the likelihood w.r.t. observartions */
  NumericalScalar computeLogLikelihood(const NumericalPoint & currentState) const;

  /** Prior accessor */
  void setPrior(const Distribution & prior);
  Distribution getPrior() const;

  /** Conditional accessor */
  Distribution getConditional() const;

  /** Model accessor */
  NumericalMathFunction getModel() const;

  /** Obervations accessor */
  void setObservations(const NumericalSample & observations);
  NumericalSample getObservations() const;

  /// Burning accessor
  void setBurnIn(UnsignedLong burnIn);
  UnsignedLong getBurnIn() const;

  /// Thinning accessor
  void setThinning(UnsignedLong thinning);
  UnsignedLong getThinning() const;

  /** Virtual constructor */
  virtual MCMC * clone() const;

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:
  NumericalPoint initialState_;
  mutable NumericalPoint currentState_;

private:
  Distribution prior_;
  Distribution conditional_;

  NumericalMathFunction model_;
  NumericalSample observations_;

  UnsignedLong burnIn_; // number of first samples discarded to reach stationary regime
  UnsignedLong thinning_; // number of samples skipped at each generation


}; /* class MCMC */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_MCMC_HXX */
