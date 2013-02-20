//                                               -*- C++ -*-
/**
 *  @file  EnumerateFunctionImplementation.hxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
#ifndef OPENTURNS_ENUMERATEFUNCTIONIMPLEMENTATION_HXX
#define OPENTURNS_ENUMERATEFUNCTIONIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Indices.hxx"


BEGIN_NAMESPACE_OPENTURNS



/**
 * @class EnumerateFunctionImplementation
 *
 * The bijective function to select polynomials in the orthogonal basis
 */

class EnumerateFunctionImplementation
  : public PersistentObject
{
  CLASSNAME;
public:


  /** Parameter constructor */
  EnumerateFunctionImplementation(const UnsignedLong dimension = 1);

  /** Virtual constrcutor */
  virtual EnumerateFunctionImplementation * clone() const = 0;

  /** String converter */
  virtual String __repr__() const;

  /** The bijective association between an integer and a set of indices */
  virtual Indices operator() (const UnsignedLong index) const = 0;

  /** The inverse of the association */
  virtual UnsignedLong inverse(const Indices & indices) const;

  /** The cardinal of the given strata */
  virtual UnsignedLong getStrataCardinal(const UnsignedLong strateIndex) const = 0;

  /** The cardinal of the cumulated strata above or equal to the given strate */
  virtual UnsignedLong getStrataCumulatedCardinal(const UnsignedLong strateIndex) const = 0;

  /** Dimension accessor */
  void setDimension(const UnsignedLong dimension);
  UnsignedLong getDimension() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

private:

  // The enumerate function maps N into N^dimension
  UnsignedLong dimension_;

} ; /* class EnumerateFunctionImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ENUMERATEFUNCTIONIMPLEMENTATION_HXX */
