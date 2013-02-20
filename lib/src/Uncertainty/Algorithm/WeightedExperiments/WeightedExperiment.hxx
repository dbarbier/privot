//                                               -*- C++ -*-
/**
 *  @file  WeightedExperiment.hxx
 *  @brief Abstract top-level view of an weightedExperiment plane
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
 *  @author dutka
 *  @date   2008-06-26 13:50:17 +0200 (jeu, 26 jun 2008)
 *  Id      WeightedExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_WEIGHTEDEXPERIMENT_HXX
#define OPENTURNS_WEIGHTEDEXPERIMENT_HXX

#include "ExperimentImplementation.hxx"
#include "Distribution.hxx"
#include "NumericalPoint.hxx"
#include "Indices.hxx"
//#include "OrthogonalBasis.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class WeightedExperiment
 *
 * The class describes the probabilistic concept of weightedExperiment plan
 */
class WeightedExperiment
  : public ExperimentImplementation
{
  CLASSNAME;
public:

  /** Default constructor */
  explicit WeightedExperiment(const String & name = DefaultName);

  /** Parameters constructor */
  explicit WeightedExperiment(const UnsignedLong size,
                              const String & name = DefaultName);

  /** Parameters constructor */
  explicit WeightedExperiment(const Distribution & distribution,
                              const UnsignedLong size,
                              const String & name = DefaultName);

  /** Virtual constructor */
  virtual WeightedExperiment * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Distribution accessor */
  virtual void setDistribution(const Distribution & distribution);
  virtual Distribution getDistribution() const;

  /** Size accessor */
  virtual void setSize(const UnsignedLong size);
  virtual UnsignedLong getSize() const;

  /* Here is the interface that all derived class must implement */

  /** Sample generation */
  virtual NumericalSample generate();

  /** Sample generation with weights*/
  virtual NumericalSample generate(NumericalPoint & weights);

  /** Weight accessor */
  virtual NumericalPoint getWeight() const;

protected:

  /** Distribution that defines the weights of the experiment */
  Distribution distribution_;
  /** The size of the sample to be generated */
  UnsignedLong size_;
  /** The weights associated with the sample for numerical integration */
  NumericalPoint weights_;

}; /* class WeightedExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WEIGHTEDEXPERIMENT_HXX */
