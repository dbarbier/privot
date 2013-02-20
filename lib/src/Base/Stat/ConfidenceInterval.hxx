//                                               -*- C++ -*-
/**
 *  @file  ConfidenceInterval.hxx
 *  @brief ConfidenceInterval implements the notion of confidence interval
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
 *  Id      ConfidenceInterval.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_CONFIDENCEINTERVAL_HXX
#define OPENTURNS_CONFIDENCEINTERVAL_HXX

#include "PersistentObject.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class ConfidenceInterval
 *
 * ConfidenceInterval implements the notion of confidence interval
 */

class ConfidenceInterval :
  public PersistentObject
{
  CLASSNAME;

public:


  /** Default constructor */
  ConfidenceInterval(const NumericalScalar firstPt = 0.0,
                     const NumericalScalar lastPt = 1.0);

  /** Virtual constructor */
  virtual ConfidenceInterval * clone() const;

  /** Description Accessor */
  void setDescription(const Description & description);
  Description getDescription() const;

  /** String converter */
  virtual String __repr__() const;

  /** Elements accessors */
  void setValues(const NumericalScalar firstPt,
                 const NumericalScalar lastPt);
  const NumericalScalar & getFirst() const;
  const NumericalScalar & getLast() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


  /** Comparison operator */
  Bool operator ==(const ConfidenceInterval & rhs) const;

protected:

  NumericalScalar first_ ;
  NumericalScalar last_ ;


private:

  /** The description of all components */
  Description description_;

}; /* class ConfidenceInterval */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CONFIDENCEINTERVAL_HXX */
