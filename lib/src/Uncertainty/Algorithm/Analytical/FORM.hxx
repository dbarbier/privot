//                                               -*- C++ -*-
/**
 *  @file  FORM.hxx
 *  @brief FORM implements the First Order Reliability Method
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
 *  Id      FORM.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_FORM_HXX
#define OPENTURNS_FORM_HXX

#include "Analytical.hxx"
#include "FORMResult.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class FORM
 * FORM implements the First Order Reliability Method
 * and compute the results of such kind of analyses
 */

class FORM
  : public Analytical
{

  CLASSNAME;
public:


  /** Default constructor */
  FORM();

  /** Constructor with parameters */
  explicit FORM(const NearestPointAlgorithm & nearestPointAlgorithm,
                const Event & event,
                const NumericalPoint & physicalStartingPoint);


  /** Virtual constructor */
  virtual FORM * clone() const;

  /** Result accessor */
  FORMResult getResult() const;

  /** Result accessor */
  void setResult(const FORMResult & formResult);

  /** String converter */
  String __repr__() const;

  /** Function that computes the design point by re-using the Analytical::run() and creates a FORM::Result */
  void run();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  FORMResult formResult_;
} ; /* class FORM */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FORM_HXX */



