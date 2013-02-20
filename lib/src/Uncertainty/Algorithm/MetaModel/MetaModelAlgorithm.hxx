//                                               -*- C++ -*-
/**
 *  @file  MetaModelAlgorithm.hxx
 *  @brief Implementation for metamodel algorithms
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
 *  @date   2008-06-05 12:11:56 +0200 (Thu, 05 Jun 2008)
 *  Id      MetaModelAlgorithm.hxx 846 2008-06-05 10:11:56Z schueller
 */
#ifndef OPENTURNS_METAMODELALGORITHM_HXX
#define OPENTURNS_METAMODELALGORITHM_HXX

#include "NumericalPoint.hxx"
#include "NumericalSample.hxx"
#include "NumericalMathFunction.hxx"
#include "Distribution.hxx"
#include "MetaModelResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class MetaModelAlgorithm
 *
 */

class MetaModelAlgorithm
  : public PersistentObject
{
  CLASSNAME;
public:


  /** Default constructor */
  MetaModelAlgorithm(const String & name = DefaultName);

  /** Constructor with parameters */
  MetaModelAlgorithm(const Distribution & distribution,
                     const NumericalMathFunction & model,
                     const String & name = DefaultName);

  /** Virtual constructor */
  MetaModelAlgorithm * clone() const;

  /** String converter */
  virtual String __repr__() const;

  /** Distribution accessor */
  void setDistribution(const Distribution & distribution);
  Distribution getDistribution() const;

  /** Response surface computation */
  virtual void run();

  /** Sample accessors */
  virtual NumericalSample getInputSample() const;
  virtual NumericalSample getOutputSample() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** The input vector distribution */
  Distribution distribution_;

  /** The model */
  NumericalMathFunction model_;


private:


}; /* class MetaModelAlgorithm */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_METAMODELALGORITHM_HXX */
