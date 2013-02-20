//                                               -*- C++ -*-
/**
 *  @file  NonEllipticalDistribution.hxx
 *  @brief Abstract top-level class for non elliptical distributions
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
 *  Id      NonEllipticalDistribution.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_NONELLIPTICALDISTRIBUTION_HXX
#define OPENTURNS_NONELLIPTICALDISTRIBUTION_HXX

#include "ContinuousDistribution.hxx"

BEGIN_NAMESPACE_OPENTURNS




/**
 * @class NonEllipticalDistribution
 *
 * All traditionnal distribution inherits from this class.
 * Classes derived from NonEllipticalDistribution are known by themselves,
 * without being reconstructed or built in any way.
 */
class NonEllipticalDistribution
  : public ContinuousDistribution
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit NonEllipticalDistribution(const String & name = DefaultName);

  /** Virtual constructor */
  virtual NonEllipticalDistribution * clone() const;

  /** String converter */
  String __repr__() const;


protected:


private:


}; /* class NonEllipticalDistribution */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NONELLIPTICALDISTRIBUTION_HXX */
