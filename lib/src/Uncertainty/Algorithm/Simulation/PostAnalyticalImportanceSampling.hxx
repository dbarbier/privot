//                                               -*- C++ -*-
/**
 *  @file  PostAnalyticalImportanceSampling.hxx
 *  @brief PostAnalyticalImportanceSampling is an implementation of the importance sampling Montex Carlo simulation method
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
 *  @date   2007-05-02 14:59:25 +0200 (mer, 02 mai 2007)
 *  Id      PostAnalyticalImportanceSampling.hxx 424 2007-05-02 12:59:25Z dutka
 */
#ifndef OPENTURNS_POSTANALYTICALIMPORTANCESAMPLING_HXX
#define OPENTURNS_POSTANALYTICALIMPORTANCESAMPLING_HXX

#include "PostAnalyticalSimulation.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class PostAnalyticalImportanceSampling
 */

class PostAnalyticalImportanceSampling :
  public PostAnalyticalSimulation
{

  CLASSNAME;
public:


  /** Constructor with parameters */
  PostAnalyticalImportanceSampling(const AnalyticalResult & AnalyticalResult);

  /** Virtual constructor */
  virtual PostAnalyticalImportanceSampling * clone() const;

  /** String converter */
  String __repr__() const;

private:

  /** Compute the block sample */
  NumericalSample computeBlockSample();

} ; /* class PostAnalyticalImportanceSampling */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_POSTANALYTICALIMPORTANCESAMPLING_HXX */
