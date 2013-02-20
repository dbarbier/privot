//                                               -*- C++ -*-
/**
 *  @file  NatafEllipticalDistributionHessian.hxx
 *  @brief Class for the Nataf transformation hessian for elliptical
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
 *  Id      NatafEllipticalDistributionHessian.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NATAFELLIPTICALDISTRIBUTIONHESSIAN_HXX
#define OPENTURNS_NATAFELLIPTICALDISTRIBUTIONHESSIAN_HXX

#include "OTprivate.hxx"
#include "ConstantNumericalMathHessianImplementation.hxx"
#include "StorageManager.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NatafEllipticalDistributionHessian
 *
 * This class offers an interface for the Nataf gradient for elliptical distributions
 */
class NatafEllipticalDistributionHessian
  : public ConstantNumericalMathHessianImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  NatafEllipticalDistributionHessian();

  /** Parameter constructor */
  NatafEllipticalDistributionHessian(const UnsignedLong dimension);


  /** String converter */
  virtual String __repr__() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

protected:


private:

}; /* NatafEllipticalDistributionHessian */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NATAFELLIPTICALDISTRIBUTIONHESSIAN_HXX */
