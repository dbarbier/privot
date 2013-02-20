//                                               -*- C++ -*-
/**
 *  @file  DirectionalSampling.hxx
 *  @brief DirectionalSampling is an implementation of the directional sampling Montex Carlo simulation method
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
 *  Id      DirectionalSampling.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DIRECTIONALSAMPLING_HXX
#define OPENTURNS_DIRECTIONALSAMPLING_HXX

#include "Simulation.hxx"
#include "NumericalPoint.hxx"
#include "Distribution.hxx"
#include "StandardEvent.hxx"
#include "RootStrategy.hxx"
#include "SamplingStrategy.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class DirectionalSampling
 */

class DirectionalSampling :
  public Simulation
{
  CLASSNAME;
public:

  typedef Collection<NumericalScalar> NumericalScalarCollection;
  typedef Distribution::Implementation            Implementation;

  /** Constructor with parameters */
  DirectionalSampling(const Event & event);

  /** Constructor with parameters */
  DirectionalSampling(const Event & event,
                      const RootStrategy & rootStrategy,
                      const SamplingStrategy & samplingStrategy);


  /** Virtual constructor */
  virtual DirectionalSampling * clone() const;

  /** Root strategy accessor */
  void setRootStrategy(const RootStrategy & rootStrategy);
  RootStrategy getRootStrategy() const;

  /** Sampling strategy */
  void setSamplingStrategy(const SamplingStrategy & samplingStrategy);
  SamplingStrategy getSamplingStrategy() const;

  /** String converter */
  String __repr__() const;

private:

  /** Compute the block sample and the points that realized the event */
  NumericalSample computeBlockSample();

  /** Compute the contribution of a direction to the probability given the roots of the performance function along the direction */
  NumericalScalar computeContribution(const NumericalScalarCollection & roots);

  /** Compute the mean point of a direction given the roots of the performance function along the direction */
  NumericalScalar computeMeanContribution(const NumericalScalarCollection & roots);

  /** Compute the contribution of a set of direction to the probability given the contributions of each direction in the set */
  NumericalScalar computeTotalContribution(const NumericalSample & directionSample);

  StandardEvent standardEvent_;
  NumericalMathFunction standardFunction_;
  Implementation inputDistribution_;
  RootStrategy rootStrategy_;
  SamplingStrategy samplingStrategy_;

} ; /* class DirectionalSampling */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DIRECTIONALSAMPLING_HXX */
