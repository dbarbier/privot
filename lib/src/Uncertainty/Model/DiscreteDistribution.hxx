//                                               -*- C++ -*-
/**
 *  @file  DiscreteDistribution.hxx
 *  @brief Abstract top-level class for Discrete distributions
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
 *  Id      DiscreteDistribution.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_DISCRETEDISTRIBUTION_HXX
#define OPENTURNS_DISCRETEDISTRIBUTION_HXX

#include "OTprivate.hxx"
#include "UsualDistribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class DiscreteDistribution
 *
 * A subclass for Discrete usual distributions.
 */
class DiscreteDistribution
  : public UsualDistribution
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit DiscreteDistribution(const String & name = DefaultName);


  /** Virtual constructor */
  virtual DiscreteDistribution * clone() const;

  /** Comparison operator */
  Bool operator ==(const DiscreteDistribution & other) const;

  /** String converter */
  String __repr__() const;

  /* Methods inherited from upper classes */

  /** Tell if the distribution is continuous */
  Bool isContinuous() const;

  /** Tell if the distribution is discrete */
  Bool isDiscrete() const;

  /** Tell if the distribution is integer valued */
  Bool isIntegral() const;

protected:

private:

}; /* class DiscreteDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DISCRETEDISTRIBUTION_HXX */
