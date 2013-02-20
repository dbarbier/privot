//                                               -*- C++ -*-
/**
 *  @file  ARMAFactoryImplementation.hxx
 *  @brief An interface for all implementation class of process
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
 *  Id      ARMAFactoryImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_ARMAFACTORYIMPLEMENTATION_HXX
#define OPENTURNS_ARMAFACTORYIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "ARMA.hxx"
#include "TimeSeries.hxx"
#include "ProcessSample.hxx"
#include "Pointer.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class ARMAFactoryImplementation
 *
 * An interface for all implementation class of process
 */
class ARMAFactoryImplementation
  : public PersistentObject
{
  CLASSNAME;

public:

  /** Some typedefs to ease reading */
  typedef Pointer<ARMAFactoryImplementation>     Antecedent;
  typedef Antecedent                         Implementation;

  /** Default constructor */
  ARMAFactoryImplementation(const String & name = DefaultName);

  /** Parameter constructor */
  ARMAFactoryImplementation(const UnsignedLong p,
                            const UnsignedLong q,
                            const Bool invertible = true,
                            const String & name = DefaultName);

  /** Parameter constructor */
  ARMAFactoryImplementation(const Indices & p,
                            const Indices & q,
                            const Bool invertible = true,
                            const String & name = DefaultName);

  /** Virtual constructor */
  virtual ARMAFactoryImplementation * clone() const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;

  /** P accessor */
  Indices getP() const;
  UnsignedLong getCurrentP() const;

  /** Q accessor */
  Indices getQ() const;
  UnsignedLong getCurrentQ() const;

  /** Invertibility accessor */
  Bool getInvertible() const;
  void setInvertible(const Bool invertible);

  /** Build method */
  virtual ARMA * build(const TimeSeries & timeSeries) const;
  virtual ARMA * build(const TimeSeries & timeSeries,
                       NumericalPoint & informationCriteria) const;

  /** Build method */
  virtual ARMA * build(const ProcessSample & sample) const;
  virtual ARMA * build(const ProcessSample & sample,
                       NumericalPoint & informationCriteria) const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:

  /** P accessor */
  void setP(const Indices & ARIndices);
  void setCurrentP(const UnsignedLong p) const;

  /** Q accessor */
  void setQ(const Indices & MAIndices);
  void setCurrentQ(const UnsignedLong q) const;

  /** AR size */
  Indices p_;
  mutable UnsignedLong currentP_;

  /** MA size */
  Indices q_;
  mutable UnsignedLong currentQ_;

  /** Invertibility constraint */
  Bool invertible_;

}; /* class ARMAFactoryImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_ARMAFACTORYIMPLEMENTATION_HXX */
