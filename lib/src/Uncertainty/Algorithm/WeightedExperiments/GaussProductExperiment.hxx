//                                               -*- C++ -*-
/**
 *  @file  GaussProductExperiment.hxx
 *  @brief Abstract top-level view of an monteCarloExperiment plane
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
 *  Id      GaussProductExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_GAUSSPRODUCTEXPERIMENT_HXX
#define OPENTURNS_GAUSSPRODUCTEXPERIMENT_HXX

#include "WeightedExperiment.hxx"
#include "OrthogonalUniVariatePolynomialFamily.hxx"
#include "Collection.hxx"
#include "Indices.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class GaussProductExperiment
 *
 * The class describes the probabilistic concept of monteCarloExperiment plan
 */
class GaussProductExperiment
  : public WeightedExperiment
{
  CLASSNAME;
public:

  typedef Collection<OrthogonalUniVariatePolynomialFamily> OrthogonalUniVariatePolynomialFamilyCollection;

  /** Default constructor */
  explicit GaussProductExperiment(const String & name = DefaultName);

  /** Parameters constructor */
  explicit GaussProductExperiment(const Indices & marginalDegrees,
                                  const String & name = DefaultName);

  /** Parameters constructor */
  explicit GaussProductExperiment(const Distribution & distribution,
                                  const String & name = DefaultName);

  /** Parameters constructor */
  explicit GaussProductExperiment(const Distribution & distribution,
                                  const Indices & marginalDegrees,
                                  const String & name = DefaultName);

  /** Virtual constructor */
  virtual GaussProductExperiment * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Sample generation */
  using WeightedExperiment::generate;
  NumericalSample generate();

  /** Marginal degrees accessor */
  void setMarginalDegrees(const Indices & marginalDegrees);
  Indices getMarginalDegrees() const;

  /** Distribution accessor */
  void setDistribution(const Distribution & distribution);

protected:

private:
  // Compute the tensor product nodes and weights
  void computeNodesAndWeights();

  // Marginal orthogonal univariate polynomial family collection
  OrthogonalUniVariatePolynomialFamilyCollection collection_;

  // Marginal degrees
  Indices marginalDegrees_;

  // Integration nodes
  NumericalSample nodes_;

  // Flag to manage the computation of nodes and weights
  mutable Bool isAlreadyComputedNodesAndWeights_;

}; /* class GaussProductExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_GAUSSPRODUCTEXPERIMENT_HXX */
