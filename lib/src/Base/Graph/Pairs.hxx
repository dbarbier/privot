//                                               -*- C++ -*-
/**
 *  @file  Pairs.hxx
 *  @brief Pairs class for piechart plots
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
 *  @author lebrun
 *  @date   2011-02-26 21:12:17 +0100 (sam. 26 févr. 2011)
 *  Id      Pairs.hxx 1813 2011-02-26 20:12:17Z lebrun
 */
#ifndef OPENTURNS_PAIRS_HXX
#define OPENTURNS_PAIRS_HXX

#include "OTprivate.hxx"
#include "Drawable.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Pairs
 *
 * The class Pairs
 * Instance of Drawable
 */


class Pairs : public DrawableImplementation
{
  CLASSNAME;

public:


  /** Default constructor */
  Pairs(const NumericalSample & data,
        const String & title = "");

  Pairs(const NumericalSample & data,
        const String & title,
        const Description labels,
        const String color,
        const String PointStyle);

  /** String converter */
  String __repr__() const;

  /** Accessor for first coordinate */
  NumericalSample getData() const;
  void setData(const NumericalSample & x);

  /** Accessor for labels */
  Description getLabels() const;
  void setLabels(const Description & labels);

  /** Accessor for title */
  String getTitle() const;
  void setTitle(const String & title);


  /** Check for data validity */
  Bool isValidData(const NumericalSample & data) const;

  /** Draw method */
  String draw() const;

  /** Clone method */
  Pairs * clone() const;

  /** Build default labels */
  void buildDefaultLabels();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

  /** Clean all the temporary data created by draw() method */
  virtual void clean() const;

private:

  Pairs() {};
  friend class Factory<Pairs>;

  /** Labels of the level sets. If none is given, it defaults to the level values. */
  Description labels_;

  /** Title of the main */
  String title_;

}; /* class Pairs */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PAIRS_HXX */
