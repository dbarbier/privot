//                                               -*- C++ -*-
/**
 * @file  PythonRandomVectorImplementation.hxx
 * @brief This class binds a Python object to an Open TURNS' RandomVector
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
 * @author schueller
 * @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */

#ifndef OPENTURNS_PYTHONRANDOMVECTORIMPLEMENTATION_HXX
#define OPENTURNS_PYTHONRANDOMVECTORIMPLEMENTATION_HXX

#include "Python.h"
#include "RandomVectorImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class PythonRandomVectorImplementation
 *
 * This class binds a Python object to an Open TURNS' RandomVector
 */
class PythonRandomVectorImplementation
  : public RandomVectorImplementation
{
  CLASSNAME;
public:


  /** Constructor from Python object*/
  PythonRandomVectorImplementation(PyObject * pyObject);

  /** Copy constructor */
  PythonRandomVectorImplementation(const PythonRandomVectorImplementation & other);

  /** Virtual constructor */
  virtual PythonRandomVectorImplementation * clone() const;

  /** Destructor */
  virtual ~PythonRandomVectorImplementation();

  /** Comparison operator */
  Bool operator ==(const PythonRandomVectorImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset) const;


  /* Here is the interface that all derived class must implement */

  /** Dimension accessor */
  virtual UnsignedLong getDimension() const;

  /** Realization accessor */
  virtual NumericalPoint getRealization() const;

  /** Numerical sample accessor */
  virtual NumericalSample getSample(const UnsignedLong size) const;

  /** Mean accessor */
  virtual NumericalPoint getMean() const;

  /** Covariance accessor */
  virtual CovarianceMatrix getCovariance() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method save() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  friend class Factory<PythonRandomVectorImplementation>;

  /** Default constructor */
  PythonRandomVectorImplementation();

  /** The underlying Python object */
  PyObject * pyObj_;

}; /* class PythonRandomVectorImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PYTHONRANDOMVECTORIMPLEMENTATION_HXX */
