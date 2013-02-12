//                                               -*- C++ -*-
/**
 *  @file  Staircase.cxx
 *  @brief Staircase class for staircase plots
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
 */
#include "Staircase.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Staircase);

static Factory<Staircase> RegisteredFactory("Staircase");

/* Default constructor */
Staircase::Staircase():
  DrawableImplementation(),
  pattern_()
{
  // Nothing to do
}

/* Constructor with parameters */
Staircase::Staircase(const NumericalSample & data,
                     const String & legend):
  DrawableImplementation(data, legend),
  pattern_(ResourceMap::Get("DrawableImplementation-DefaultPattern"))
{
  // Check data validity
  setData(data);
}

/* Constructor with parameters */
Staircase::Staircase(const NumericalSample & data,
                     const String & color,
                     const String & lineStyle,
                     const UnsignedLong lineWidth,
                     const String & pattern,
                     const String & legend)
  : DrawableImplementation(data, legend)
{
  setColor(color);
  setLineStyle(lineStyle);
  setPattern(pattern);
  setLineWidth(lineWidth);
  // Check data validity
  setData(data);
}

/* Constructor with old parameters */
Staircase::Staircase(const NumericalSample & data,
                     const String & color,
                     const String & lineStyle,
                     const String & pattern,
                     const String & legend)
  : DrawableImplementation(data, legend)
{
  setColor(color);
  setLineStyle(lineStyle);
  setPattern(pattern);
  setLineWidth(1);
  // Check data validity
  setData(data);
}

/* String converter */
String Staircase::__repr__() const
{
  OSS oss;
  oss << "class=" << Staircase::GetClassName()
      << " name=" << getName()
      << " pattern=" << pattern_
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Draw method */
String Staircase::draw() const
{
  dataFileName_ = "";
  OSS oss;
  // Stores the data in a temporary file
  oss << DrawableImplementation::draw() << "\n";
  // The specific R command for drawing
  oss << "lines(dataOT[, 1], dataOT[, 2]"
      << ", lty=\"" << lineStyle_
      << "\", col=\"" << color_
      << "\", lwd=" << lineWidth_
      << ", type=\"" << pattern_
      << "\")";

  return oss;
}

/* Clone method */
Staircase * Staircase::clone() const
{
  return new Staircase(*this);
}

/* Accessor for pattern */
String Staircase::getPattern() const
{
  return pattern_;
}

/* Accessor for pattern */
void Staircase::setPattern(const String style)
{
  if(!IsValidPattern(style)) throw InvalidArgumentException(HERE) << "Given staircase pattern = " << style << " is incorrect";
  pattern_ = style;
}

/* Check validity of data */
Bool Staircase::isValidData(const NumericalSample & data) const
{
  return (data.getDimension() == 2);
}

/* Method save() stores the object through the StorageManager */
void Staircase::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
  adv.saveAttribute( "pattern_", pattern_ );
}

/* Method load() reloads the object from the StorageManager */
void Staircase::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
  adv.loadAttribute( "pattern_", pattern_ );
}



END_NAMESPACE_OPENTURNS
