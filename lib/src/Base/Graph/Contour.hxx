//                                               -*- C++ -*-
/**
 *  @file  Contour.hxx
 *  @brief Contour class for piechart plots
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
 *  Id      Contour.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_CONTOUR_HXX
#define OPENTURNS_CONTOUR_HXX

#include "DrawableImplementation.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Contour
 *
 * The class describing a contour chart
 * Instance of Drawable
 */


class Contour : public DrawableImplementation
{
  CLASSNAME;

public:

  /** Default constructor */
  Contour(const UnsignedLong dimX,
          const UnsignedLong dimY,
          const NumericalSample & data,
          const String & legend = "");

  /** Constructor with parameters */
  Contour(const NumericalSample & x,
          const NumericalSample & y,
          const NumericalSample & data,
          const NumericalPoint & levels,
          const Description & labels,
          const Bool drawLabels = true,
          const String & legend = "");

  /** Constructor with parameters
      Contour(const NumericalSample & xy,
      const NumericalSample & data,
      const NumericalPoint & levels,
      const Description & labels,
      const Bool drawLabels = true,
      const String & legend = ""); */

  /** String converter */
  String __repr__() const;

  /** Accessor for first coordinate */
  NumericalSample getX() const;
  void setX(const NumericalSample & x);

  /** Accessor for second coordinate */
  NumericalSample getY() const;
  void setY(const NumericalSample & y);

  /** Accessor for levels */
  NumericalPoint getLevels() const;
  void setLevels(const NumericalPoint & levels);

  /** Accessor for labels */
  Description getLabels() const;
  void setLabels(const Description & labels);

  /** Accessor for drawLabels */
  Bool getDrawLabels() const;
  void setDrawLabels(const Bool & drawLabels);

  /** Check for data validity */
  Bool isValidData(const NumericalSample & data) const;

  /** Accessor for boundingbox */
  BoundingBox getBoundingBox() const;

  /** Draw method */
  String draw() const;

  /** Clone method */
  Contour * clone() const;

  /** Build default levels using quantiles associated with regularly spaced probability levels
   */
  void buildDefaultLevels(const UnsignedLong number = ResourceMap::GetAsUnsignedLong( "Contour-DefaultLevelsNumber" ));

  /** Build default labels by taking the level values */
  void buildDefaultLabels();

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

  /** Clean all the temporary data created by draw() method */
  virtual void clean() const;

private:

  Contour() {};
  friend class Factory<Contour>;

  /** Sample of first coordinate */
  NumericalSample x_;

  /** Sample of second coordinate */
  NumericalSample y_;

  /** Values of the level sets */
  NumericalPoint levels_;

  /** Labels of the level sets. If none is given, it defaults to the level values. */
  Description labels_;

  /** Flag for drawing the labels */
  Bool drawLabels_;

  /** Temporary file name for the exchange with R */
  mutable String xFileName_;

  /** Temporary file name for the exchange with R */
  mutable String yFileName_;

}; /* class Contour */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_CONTOUR_HXX */
