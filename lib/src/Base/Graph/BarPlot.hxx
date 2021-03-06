//                                               -*- C++ -*-
/**
 *  @file  BarPlot.hxx
 *  @brief BarPlot class for barplots
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
 *  Id      BarPlot.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_BARPLOT_HXX
#define OPENTURNS_BARPLOT_HXX

#include "OTprivate.hxx"
#include "DrawableImplementation.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class BarPlot
 *
 * The class describing a curve plot
 * Instance of Drawable
 */


class BarPlot : public DrawableImplementation
{

  CLASSNAME;

public:

  /** Default constructor */
  BarPlot(const NumericalSample & data,
          const NumericalScalar origin,
          const String & legend = "");

  /** Constructor with parameters */
  BarPlot(const NumericalSample & data,
          const NumericalScalar origin,
          const String & color,
          const String & fillStyle,
          const String & lineStyle,
          const UnsignedLong lineWidth,
          const String & legend = "");

  /** Constructor with old parameters */
  BarPlot(const NumericalSample & data,
          const NumericalScalar origin,
          const String & color,
          const String & fillStyle,
          const String & lineStyle,
          const String & legend = "");

  /** String converter */
  String __repr__() const;

  /** Accessor for origin */
  NumericalScalar getOrigin() const;
  void setOrigin(const NumericalScalar origin);

  /** Accessor for boundingbox */
  BoundingBox getBoundingBox() const;

  /** Draw method */
  String draw() const;

  /** Clone method */
  BarPlot * clone() const;

  /** Check fo data validity */
  Bool isValidData(const NumericalSample & data) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

private:

  BarPlot() {};
  friend class Factory<BarPlot>;

  /** Origin of the BarPlot */
  NumericalScalar origin_;

}; /* class BarPlot */



END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_BARPLOT_HXX */
