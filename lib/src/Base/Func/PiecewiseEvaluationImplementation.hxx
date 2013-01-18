//                                               -*- C++ -*-
/**
 *  @file  PiecewiseEvaluationImplementation.hxx
 *  @brief The evaluation part of a linear piecewise scalar function
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_PIECEWISEEVALUATIONIMPLEMENTATION_HXX
#define OPENTURNS_PIECEWISEEVALUATIONIMPLEMENTATION_HXX

#include "NumericalMathEvaluationImplementation.hxx"
#include "Collection.hxx"
#include "PersistentCollection.hxx"
#include "NumericalMathFunction.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class PiecewiseEvaluationImplementation
 *
 * The evaluation part of a linear piecewise scalar function
 *
 */

class PiecewiseEvaluationImplementation
  : public NumericalMathEvaluationImplementation
{
  CLASSNAME;
public:

  // friend class Factory<PiecewiseEvaluationImplementation>;

  /** Default constructor */
  PiecewiseEvaluationImplementation();

  /** Parameter constructor */
  PiecewiseEvaluationImplementation(const NumericalPoint & locations,
				    const NumericalPoint & values);

  /** Virtual constructor */
  virtual PiecewiseEvaluationImplementation * clone() const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Evaluation operator */
  using NumericalMathEvaluationImplementation::operator ();
  NumericalPoint operator () (const NumericalPoint & inP) const;

  /** Locations accessor */
  NumericalPoint getLocations() const;
  void setLocations(const NumericalPoint & locations);

  /** Values accessor */
  NumericalPoint getValues() const;
  void setValues(const NumericalPoint & values);

  /** Simultaneous locations and values acessor */
  void setLocationsAndValues(const NumericalPoint & locations,
			     const NumericalPoint & values);

  /** Input dimension accessor */
  UnsignedLong getInputDimension() const;

  /** Output dimension accessor */
  UnsignedLong getOutputDimension() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);


protected:

private:
  // The locations
  NumericalPoint locations_;

  // The values
  NumericalPoint values_;

} ; /* class PiecewiseEvaluationImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PIECEWISEEVALUATIONIMPLEMENTATION_HXX */
