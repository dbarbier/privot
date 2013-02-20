//                                               -*- C++ -*-
/**
 *  @file  DrawableImplementation.hxx
 *  @brief Abstract top-level class for all drawables
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
 *  Id      DrawableImplementation.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_DRAWABLEIMPLEMENTATION_HXX
#define OPENTURNS_DRAWABLEIMPLEMENTATION_HXX

#include <map>
#include "Description.hxx"
#include "Collection.hxx"
#include "PersistentObject.hxx"
#include "NumericalSample.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class DrawableImplementation
 *
 * The class describes the a drawable graphic
 * Instances of this class can actually be plotted
 */

class DrawableImplementation :
  public PersistentObject
{

  CLASSNAME;

public:

  typedef NumericalPoint BoundingBox;

  /** Default Constructor with legend label */
  DrawableImplementation(const NumericalSample & data,
                         const String & legend = "");

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset = "") const;

  /** Comparison operator */
  Bool operator == (const DrawableImplementation & other) const;

  /** Code for point style accessor */
  virtual UnsignedLong getPointCode(const String & key) const;

  /** Check validity of data */
  virtual Bool isValidData(const NumericalSample & data) const;
  virtual Bool isValidData(const NumericalPoint & data) const;

  /** Check validity of color */
  static Bool IsValidColor(const String & key);

  /** Check validity of line style */
  static Bool IsValidLineStyle(const String & key);

  /** Check validity of point style */
  static Bool IsValidPointStyle(const String & key);

  /** Check validity of fill style */
  static Bool IsValidFillStyle(const String & key);

  /** Check validity of pattern parameter */
  static Bool IsValidPattern(const String & pattern);

  /** Legend name accessor */
  virtual String getLegendName() const;
  virtual void setLegendName(const String & legendName);

  /** Data accessor */
  virtual NumericalSample getData() const;

  /** Line style accessor */
  virtual String getLineStyle() const;
  virtual void setLineStyle(const String & lineStyle);

  /** Color accessor */
  virtual String getColor() const;
  virtual void setColor(const String & color);

  /** Point style accessor */
  virtual String getPointStyle() const;
  virtual void setPointStyle(const String & pointStyle);

  /** Filling style (for surfaces) accessor */
  virtual String getFillStyle() const;
  virtual void setFillStyle(const String & fillStyle);

  /** Line width accessor */
  virtual UnsignedLong getLineWidth() const;
  virtual void setLineWidth(const UnsignedLong lineWidth);

  /** Accessor for pattern */
  virtual String getPattern() const;
  virtual void setPattern(const String style);

  /** Show points accessors */
  virtual void setShowPoints(const Bool showPoints);
  virtual Bool getShowPoints() const;

  /** Accessor for center */
  virtual NumericalPoint getCenter() const;
  virtual void setCenter(const NumericalPoint & center);

  /** Accessor for radius */
  virtual NumericalScalar getRadius() const;
  virtual void setRadius(const NumericalScalar radius);

  /** Accessor for labels */
  virtual Description getLabels() const;
  virtual void setLabels(const Description & labels);

  /** Accessor for color palette */
  virtual Description getPalette() const;
  virtual void setPalette(const Description & palette);

  /** Accessor for origin */
  virtual NumericalScalar getOrigin() const;
  virtual void setOrigin(const NumericalScalar origin);

  /** Accessor for first coordinate */
  virtual NumericalSample getX() const;
  virtual void setX(const NumericalSample & x);

  /** Accessor for second coordinate */
  virtual NumericalSample getY() const;
  virtual void setY(const NumericalSample & y);

  /** Accessor for levels */
  virtual NumericalPoint getLevels() const;
  virtual void setLevels(const NumericalPoint & levels);

  /** Accessor for drawLabels */
  virtual Bool getDrawLabels() const;
  virtual void setDrawLabels(const Bool & drawLabels);

  /** Bounding box accessor */
  virtual BoundingBox getBoundingBox() const;

  /** R command generating method, for plotting through R */
  virtual String draw() const;

  /** Clean all the temporary data created by draw() method */
  virtual void clean() const;

  /** Clone method */
  virtual DrawableImplementation * clone() const;

  /** Give the colors name */
  static Description GetValidColors();

  /** Give the line style names */
  static Description GetValidLineStyles();

  /** Give the fill style names */
  static Description GetValidFillStyles();

  /** Give the point style names */
  static Description GetValidPointStyles();

  /** Convert an RGB triplet to a valid hexadecimal code */
  static String ConvertFromRGB(const UnsignedLong red,
                               const UnsignedLong green,
                               const UnsignedLong blue);

  /** Convert an RGBA quadruplet to a valid hexadecimal code */
  static String ConvertFromRGBA(const UnsignedLong red,
                                const UnsignedLong green,
                                const UnsignedLong blue,
                                const UnsignedLong alpha);

  /** Convert an RGB triplet to a valid hexadecimal code */
  static String ConvertFromRGB(const NumericalScalar red,
                               const NumericalScalar green,
                               const NumericalScalar blue);

  /** Convert an RGBA quadruplet to a valid hexadecimal code */
  static String ConvertFromRGBA(const NumericalScalar red,
                                const NumericalScalar green,
                                const NumericalScalar blue,
                                const NumericalScalar alpha);

  /** Convert an HSV triplet into an RGB triplet */
  static NumericalPoint ConvertFromHSVIntoRGB(const NumericalScalar hue,
                                              const NumericalScalar saturation,
                                              const NumericalScalar value);

  /** Convert an HSV triplet to a valid hexadecimal code */
  static String ConvertFromHSV(const NumericalScalar hue,
                               const NumericalScalar saturation,
                               const NumericalScalar value);

  /** Convert an HSVA quadruplet to a valid hexadecimal code */
  static String ConvertFromHSVA(const NumericalScalar hue,
                                const NumericalScalar saturation,
                                const NumericalScalar value,
                                const NumericalScalar alpha);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() stores the object through the StorageManager */
  void load(Advocate & adv);

protected:
  DrawableImplementation()
    : PersistentObject(),
      legendName_(""),
      data_(),
      color_(ResourceMap::Get("DrawableImplementation-DefaultColor")),
      fillStyle_(ResourceMap::Get("DrawableImplementation-DefaultFillStyle")),
      lineStyle_(ResourceMap::Get("DrawableImplementation-DefaultLineStyle")),
      pointStyle_(ResourceMap::Get("DrawableImplementation-DefaultPointStyle")),
      lineWidth_(ResourceMap::GetAsUnsignedLong("DrawableImplementation-DefaultLineWidth")),
      dataFileName_("")
  {};
  friend class Factory<DrawableImplementation>;

  static Bool IsFirstInitialization;

  /** A map matching keys with R codes for point symbols */
  static std::map<String, UnsignedLong> SymbolCodes;

  /** Valid colors */
  static Description ValidColors;

  /** Valid line styles */
  static Description ValidLineStyles;

  /** Valid fill styles */
  static Description ValidFillStyles;

  /** Bounding box size */
  static const UnsignedLong BoundingBoxSize;

  /** Data accessor */
  virtual void setData(const NumericalSample & data);
  virtual void setData(const NumericalPoint & data);

  /** The name of the drawable, to be displayed in the legend */
  String legendName_;

  /** The data to be plotted */
  NumericalSample data_;

  /** Color of the curve */
  String color_;

  /** Fill style */
  String fillStyle_;

  /** Line style of the curve */
  String lineStyle_;

  /** Line style of the curve */
  String pointStyle_;

  /** Line width of the curve */
  UnsignedLong lineWidth_;

  /** Initialize Valid parameter list */
  static void InitializeValidParameterList();

private:
  /** Temporary file name for the exchange with R */
  mutable String dataFileName_;

}; /* class DrawableImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DRAWABLEIMPLEMENTATION_HXX */
