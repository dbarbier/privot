//                                               -*- C++ -*-
/**
 *  @file  Drawable.cxx
 *  @brief Abstract top-level class for all Drawable
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
 */
#include "Drawable.hxx"
#include "Curve.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Drawable);

/* Default constructor */
Drawable::Drawable():
  TypedInterfaceObject<DrawableImplementation>(Curve(NumericalSample(1, 2)).clone())
{
  // Nothing to do
}

/* Default constructor */
Drawable::Drawable(const DrawableImplementation & implementation):
  TypedInterfaceObject<DrawableImplementation>(implementation.clone())
{
  // Nothing to do
}

/* String converter */
String Drawable::__repr__() const
{
  OSS oss;
  oss << "class=" << Drawable::GetClassName()
      << " name=" << getName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* Here is the interface */

/* Accessor for bounding box of the drawable */
NumericalPoint Drawable::getBoundingBox() const
{
  return getImplementation()->getBoundingBox();
}

/* Accessor for legend name */
String Drawable::getLegendName() const
{
  return getImplementation()->getLegendName();
}

/* Accessor for legend name */
void Drawable::setLegendName(const String & legendName)
{
  copyOnWrite();
  getImplementation()->setLegendName(legendName);
}

/* Accessor for legend line style */
String Drawable::getLineStyle() const
{
  return getImplementation()->getLineStyle();
}

/* Accessor for legend point style */
void Drawable::setLineStyle(const String & lineStyle)
{
  copyOnWrite();
  getImplementation()->setLineStyle(lineStyle);
}

/* Accessor for legend point style */
String Drawable::getPointStyle() const
{
  return getImplementation()->getPointStyle();
}

/* Accessor for legend point style */
void Drawable::setPointStyle(const String & pointStyle)
{
  copyOnWrite();
  getImplementation()->setPointStyle(pointStyle);
}

/* Accessor for legend fill style */
String Drawable::getFillStyle() const
{
  return getImplementation()->getFillStyle();
}

/* Accessor for legend fill style */
void Drawable::setFillStyle(const String & fillStyle)
{
  copyOnWrite();
  getImplementation()->setFillStyle(fillStyle);
}

/* Accessor for color */
String Drawable::getColor() const
{
  return getImplementation()->getColor();
}

String Drawable::getColorCode() const
{
  return getImplementation()->getColorCode();
}

/* Accessor for color */
void Drawable::setColor(const String & color)
{
  copyOnWrite();
  getImplementation()->setColor(color);
}

/* Accessor for line width */
UnsignedLong Drawable::getLineWidth() const
{
  return getImplementation()->getLineWidth();
}

/* Accessor for line width */
void Drawable::setLineWidth(const UnsignedLong lineWidth)
{
  copyOnWrite();
  getImplementation()->setLineWidth(lineWidth);
}

/* Accessor for pattern */
String Drawable::getPattern() const
{
  return getImplementation()->getPattern();
}

void Drawable::setPattern(const String style)
{
  copyOnWrite();
  getImplementation()->setPattern(style);
}

/* Accessor for center */
NumericalPoint Drawable::getCenter() const
{
  return getImplementation()->getCenter();
}

void Drawable::setCenter(const NumericalPoint & center)
{
  copyOnWrite();
  getImplementation()->setCenter(center);
}

/* Accessor for radius */
NumericalScalar Drawable::getRadius() const
{
  return getImplementation()->getRadius();
}

void Drawable::setRadius(const NumericalScalar radius)
{
  copyOnWrite();
  getImplementation()->setRadius(radius);
}

/* Accessor for labels */
Description Drawable::getLabels() const
{
  return getImplementation()->getLabels();
}

void Drawable::setLabels(const Description & labels)
{
  copyOnWrite();
  getImplementation()->setLabels(labels);
}

/* Accessor for color palette */
Description Drawable::getPalette() const
{
  return getImplementation()->getPalette();
}

void Drawable::setPalette(const Description & palette)
{
  copyOnWrite();
  getImplementation()->setPalette(palette);
}

/* Accessor for origin */
NumericalScalar Drawable::getOrigin() const
{
  return getImplementation()->getOrigin();
}

void Drawable::setOrigin(const NumericalScalar origin)
{
  copyOnWrite();
  getImplementation()->setOrigin(origin);
}

/* Accessor for first coordinate */
NumericalSample Drawable::getX() const
{
  return getImplementation()->getX();
}

void Drawable::setX(const NumericalSample & x)
{
  copyOnWrite();
  getImplementation()->setX(x);
}

/* Accessor for second coordinate */
NumericalSample Drawable::getY() const
{
  return getImplementation()->getY();
}

void Drawable::setY(const NumericalSample & y)
{
  copyOnWrite();
  getImplementation()->setY(y);
}

/* Accessor for levels */
NumericalPoint Drawable::getLevels() const
{
  return getImplementation()->getLevels();
}

void Drawable::setLevels(const NumericalPoint & levels)
{
  copyOnWrite();
  getImplementation()->setLevels(levels);
}

/* Accessor for drawLabels */
Bool Drawable::getDrawLabels() const
{
  return getImplementation()->getDrawLabels();
}

void Drawable::setDrawLabels(const Bool & drawLabels)
{
  copyOnWrite();
  getImplementation()->setDrawLabels(drawLabels);
}

/* Accessor for data */
NumericalSample Drawable::getData() const
{
  return getImplementation()->getData();
}

/* Generate de R commands for plotting  the graphic */
String Drawable::draw() const
{
  return getImplementation()->draw();
}

/* Clean all the temporary data created by draw() method */
void Drawable::clean() const
{
  return getImplementation()->clean();
}

/* Get R point code from key */
UnsignedLong Drawable::getPointCode(const String key) const
{
  return getImplementation()->getPointCode(key);
}

/* Give the colors name */
Description Drawable::GetValidColors()
{
  return DrawableImplementation::GetValidColors();
}

/* Give the line style names */
Description Drawable::GetValidLineStyles()
{
  return DrawableImplementation::GetValidLineStyles();
}

/* Give the fill style names */
Description Drawable::GetValidFillStyles()
{
  return DrawableImplementation::GetValidFillStyles();
}

/* Give the point style names */
Description Drawable::GetValidPointStyles()
{
  return DrawableImplementation::GetValidPointStyles();
}

/* Convert an RGB triplet to a valid hexadecimal code */
String Drawable::ConvertFromRGB(const UnsignedLong red,
                                const UnsignedLong green,
                                const UnsignedLong blue)
{
  return DrawableImplementation::ConvertFromRGB(red, green, blue);
}

/* Convert an RGBA quadruplet to a valid hexadecimal code */
String Drawable::ConvertFromRGBA(const UnsignedLong red,
                                 const UnsignedLong green,
                                 const UnsignedLong blue,
                                 const UnsignedLong alpha)
{
  return DrawableImplementation::ConvertFromRGBA(red, green, blue, alpha);
}

/* Convert an RGB triplet to a valid hexadecimal code */
String Drawable::ConvertFromRGB(const NumericalScalar red,
                                const NumericalScalar green,
                                const NumericalScalar blue)
{
  return DrawableImplementation::ConvertFromRGB(red, green, blue);
}

/* Convert an RGBA quadruplet to a valid hexadecimal code */
String Drawable::ConvertFromRGBA(const NumericalScalar red,
                                 const NumericalScalar green,
                                 const NumericalScalar blue,
                                 const NumericalScalar alpha)
{
  return DrawableImplementation::ConvertFromRGBA(red, green, blue, alpha);
}

/* Convert an HSV triplet into an RGB triplet */
NumericalPoint Drawable::ConvertFromHSVIntoRGB(const NumericalScalar hue,
                                               const NumericalScalar saturation,
                                               const NumericalScalar value)
{
  return DrawableImplementation::ConvertFromHSVIntoRGB(hue, saturation, value);
}

/* Convert an HSV triplet to a valid hexadecimal code */
String Drawable::ConvertFromHSV(const NumericalScalar hue,
                                const NumericalScalar saturation,
                                const NumericalScalar value)
{
  return DrawableImplementation::ConvertFromHSV(hue, saturation, value);
}

/* Convert an HSVA quadruplet to a valid hexadecimal code */
String Drawable::ConvertFromHSVA(const NumericalScalar hue,
                                 const NumericalScalar saturation,
                                 const NumericalScalar value,
                                 const NumericalScalar alpha)
{
  return DrawableImplementation::ConvertFromHSVA(hue, saturation, value, alpha);
}

END_NAMESPACE_OPENTURNS
