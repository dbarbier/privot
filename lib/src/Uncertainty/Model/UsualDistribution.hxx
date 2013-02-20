//                                               -*- C++ -*-
/**
 *  @file  UsualDistribution.hxx
 *  @brief Abstract top-level class for all standard distributions
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
 *  Id      UsualDistribution.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_USUALDISTRIBUTION_HXX
#define OPENTURNS_USUALDISTRIBUTION_HXX

#include "OTprivate.hxx"
#include "DistributionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class UsualDistribution
 *
 * All traditionnal distribution inherits from this class.
 * Classes derived from UsualDistribution are known by themselves,
 * without being reconstructed or built in any way.
 */
class UsualDistribution
  : public DistributionImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit UsualDistribution(const String & name = DefaultName);


  /** Virtual constructor */
  virtual UsualDistribution * clone() const;

  /** String converter */
  String __repr__() const;



  /* Here is the interface that all derived class must implement */

  /** Tell if the distribution is elliptical */
  virtual Bool isElliptical() const;

  /** Tell if the distribution is continuous */
  virtual Bool isContinuous() const;



protected:


private:

}; /* class UsualDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_USUALDISTRIBUTION_HXX */
