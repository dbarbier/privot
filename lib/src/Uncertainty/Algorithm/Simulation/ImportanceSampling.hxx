//                                               -*- C++ -*-
/**
 *  @file  ImportanceSampling.hxx
 *  @brief ImportanceSampling is an implementation of the importance sampling Montex Carlo simulation method
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
 *  Id      ImportanceSampling.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_IMPORTANCESAMPLING_HXX
#define OPENTURNS_IMPORTANCESAMPLING_HXX

#include "Simulation.hxx"
#include "Distribution.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class ImportanceSampling
 */

class ImportanceSampling :
  public Simulation
{

  CLASSNAME;
public:


  /** Constructor with parameters */
  ImportanceSampling(const Event & event,
                     const Distribution & importanceDistribution);


  /** Virtual constructor */
  virtual ImportanceSampling * clone() const;

  /** Importance distribution accessor */
  Distribution getImportanceDistribution() const;

  /** String converter */
  String __repr__() const;

private:

  /** Compute the block sample */
  NumericalSample computeBlockSample();

  /** Instrumental importance distribution */
  Distribution importanceDistribution_;

} ; /* class ImportanceSampling */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_IMPORTANCESAMPLING_HXX */
