//                                               -*- C++ -*-
/**
 *  @file  NearestPointChecker.hxx
 *  @brief NearestPointChecker checks wether the nearest point found is really the nearestpoint
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
 *  Id      NearestPointChecker.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NEARESTPOINTCHECKER_HXX
#define OPENTURNS_NEARESTPOINTCHECKER_HXX

#include "OTprivate.hxx"
#include "Threadable.hxx"
#include "Pointer.hxx"
#include "NumericalMathFunction.hxx"
#include "NumericalSample.hxx"
#include "ComparisonOperator.hxx"
#include "PersistentObject.hxx"
#include "NearestPointCheckerResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class NearestPointChecker
 *  checks wether the nearest point found is really the nearestpoint
 *
 */

class NearestPointChecker
  : public PersistentObject,
    public Threadable
{
  CLASSNAME;
public:


  /** Constructor with parameters */
  explicit NearestPointChecker(const NumericalMathFunction & levelFunction,
                               const ComparisonOperator & comparisonOperator,
                               const NumericalScalar threshold,
                               const NumericalSample & sample);


  /** Virtual constructor */
  virtual NearestPointChecker * clone() const;

  /** levelFunction  accessor */
  void setLevelFunction(const NumericalMathFunction & levelFunction);

  /** levelFunction accessor */
  NumericalMathFunction getLevelFunction() const ;

  /** Result accessor */
  void setResult(const NearestPointCheckerResult & result );

  /** Result accessor */
  NearestPointCheckerResult getResult() const;

  /** Comparison operator accessor */
  void setComparisonOperator(const ComparisonOperator & comparisonOperator );

  /** Comparison operator accessor */
  ComparisonOperator getComparisonOperator() const ;

  /** threshold accessor */
  void setThreshold(const NumericalScalar threshold);

  /** threshold accessor */
  NumericalScalar getThreshold() const ;

  /** sample accessor */
  void setSample(const NumericalSample & sample);

  /** sample accessor */
  const NumericalSample & getSample() const ;

  /** Performs the actual test */
  void  run();

  /** String converter */
  String __repr__() const;

private:

  NumericalMathFunction levelFunction_;
  NearestPointCheckerResult result_;
  ComparisonOperator comparisonOperator_;
  NumericalScalar threshold_;
  NumericalSample sample_;
} ; /* class NearestPointChecker */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NEARESTPOINTCHECKER_HXX */
