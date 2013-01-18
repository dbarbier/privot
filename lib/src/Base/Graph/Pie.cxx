//                                               -*- C++ -*-
/**
 *  @file  Pie.cxx
 *  @brief Pie class for piechart plots
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
#include <cstdlib>

#include "Pie.hxx"
#include "PersistentObjectFactory.hxx"
#include "ResourceMap.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Pie);

static Factory<Pie> RegisteredFactory("Pie");

/* Default constructor */
Pie::Pie(const NumericalPoint & data)
  : DrawableImplementation()
  , palette_(0)
  , radius_(1.0)
  , center_(2, 0.0)
  , labels_(0)
{
  // Check data validity
  setData(data);
  buildDefaultPalette();
  buildDefaultLabels();
}

Pie::Pie(const NumericalSample & data)
  : DrawableImplementation()
  , palette_(0)
  , radius_(1.0)
  , center_(2, 0.0)
  , labels_(0)
{
  LOGUSER(OSS() << "In classPie, constructor Pie(const NumericalSample & data) is deprecated in favor of Pie(const NumericalPoint & data).");
  // Check data validity
  setData(data);
  buildDefaultPalette();
  buildDefaultLabels();
}

/* Constructor with parameters */
Pie::Pie(const NumericalPoint & data,
         const Description & labels)
  : DrawableImplementation()
  , palette_()
  , radius_(1)
  , center_(2, 0.0)
  , labels_(labels)
{
  // Check data validity
  setData(data);
  buildDefaultPalette();
}

/* Constructor with parameters */
Pie::Pie(const NumericalPoint & data,
         const Description & labels,
         const NumericalPoint & center,
         const NumericalScalar & radius,
         const Description & palette)
  : DrawableImplementation()
  , palette_()
  , radius_(radius)
  , center_(center)
  , labels_(labels)
{
  if(!IsValidColorPalette(palette)) throw InvalidArgumentException(HERE) << "Given color palette = " << palette << " is incorrect";
  // Check data validity
  setData(data);
  palette_ = palette;
}

Pie::Pie(const NumericalSample & data,
         const Description & labels,
         const NumericalPoint & center,
         const NumericalScalar & radius,
         const Description & palette)
  : DrawableImplementation()
  , palette_()
  , radius_(radius)
  , center_(center)
  , labels_(labels)
{
  LOGUSER(OSS() << "In classPie, constructor Pie(const NumericalSample & data, const Description & labels, const NumericalPoint & center, const NumericalScalar & radius, const Description & palette) is deprecated in favor of Pie(const NumericalPoint & data, const Description & labels, const NumericalPoint & center, const NumericalScalar & radius, const Description & palette).");
  if(!IsValidColorPalette(palette)) throw InvalidArgumentException(HERE) << "Given color palette = " << palette << " is incorrect";
  // Check data validity
  setData(data);
  palette_ = palette;
}

/* String converter */
String Pie::__repr__() const
{
  OSS oss;
  oss << "class=" << Pie::GetClassName()
      << " name=" << getName()
      << " labels=" << labels_
      << " radius=" << radius_
      << " center=" << center_
      << " color palette=" << palette_
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Accessor for center */
NumericalPoint Pie::getCenter() const
{
  return center_;
}
void Pie::setCenter(const NumericalPoint & center)
{
  center_ = center;
}

/* Accessor for radius */
NumericalScalar Pie::getRadius() const
{
  return radius_;
}
void Pie::setRadius(const NumericalScalar radius)
{
  radius_ = radius;
}

/* Accessor for labels */
Description Pie::getLabels() const
{
  return labels_;
}
void Pie::setLabels(const Description & labels)
{
  labels_ = labels;
}

/* Accessor for color palette */
Description Pie::getPalette() const
{
  return palette_;
}
void Pie::setPalette(const Description & palette)
{
  if(!IsValidColorPalette(palette)) throw InvalidArgumentException(HERE) << "Given color palette = " << palette << " is incorrect";
  palette_ = palette;
}

/* Accessor for boundingbox */
Pie::BoundingBox Pie::getBoundingBox() const
{
  BoundingBox boundingBox(BoundingBoxSize);
  boundingBox[0] = center_[0] - (1.0 + ResourceMap::GetAsNumericalScalar("Pie-HorizontalMargin")) * radius_;
  boundingBox[1] = center_[0] + (1.0 + ResourceMap::GetAsNumericalScalar("Pie-HorizontalMargin")) * radius_;
  boundingBox[2] = center_[1] - (1.0 + ResourceMap::GetAsNumericalScalar("Pie-VerticalMargin")) * radius_;
  boundingBox[3] = center_[1] + (1.0 + ResourceMap::GetAsNumericalScalar("Pie-VerticalMargin")) * radius_;
  return boundingBox;
}

/* Draw method */
String Pie::draw() const
{
  //pie(xpi,center=center,radius=rayon,labels=noms)
  OSS oss;
  // Stores the data in a temporary file
  oss << DrawableImplementation::draw() << "\n";
  // The specific R command for drawing
  // Labels are drawn only if the associated data shares a sufficient amount of the total
  NumericalScalar labelThreshold(data_.getMax()[0] * ResourceMap::GetAsNumericalScalar("Pie-LabelThreshold"));
  oss << "pie(dataOT[,1],"
      << "center=c(" << center_[0] << "," << center_[1]
      << "),radius=" << radius_;
  UnsignedLong size(labels_.getSize());
  // If there is any label defined
  if (size > 0)
    {
      oss << ",labels=c(\"";
      String separator("");
      for(UnsignedLong i = 0; i < size; ++i, separator = "\",\"") oss << separator << (data_[i][0] >= labelThreshold ? labels_[i] : "");
      oss << "\")";
    }
  size = palette_.getSize();
  // If there is any color defined
  if (size > 0)
    {
      oss << ",col=c(\"";
      String separator("");
      for(UnsignedLong i = 0; i < size; ++i, separator = "\",\"") oss << separator << palette_[i];
      oss << "\")";
    }
  oss << ")";
  return oss;
}

/* Clone method */
Pie * Pie::clone() const
{
  return new Pie(*this);
}

/* Check for color palette validity */
Bool Pie::IsValidColorPalette(const Description & palette)
{
  Bool IsValid = true;
  Description::const_iterator it;
  for(it = palette.begin(); it != palette.end(); ++it)
    IsValid &= IsValidColor(*it);
  return IsValid;
}

/* Check for data validity */
Bool Pie::isValidData(const NumericalSample & data) const
{
  if (data.getDimension() != 1) return false;
  const UnsignedLong size(data.getSize());
  NumericalPoint newData(size);
  for (UnsignedLong i = 0; i < size; ++i) newData[i] = data[i][0];
  return isValidData(newData);
}

Bool Pie::isValidData(const NumericalPoint & data) const
{
  const UnsignedLong size(data.getSize());
  // Check if there is any data to display
  if (size == 0) return false;
  // Then, check the positivity of the data
  NumericalScalar max(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar x(data[i]);
      if (x < 0.0) return false;
      if (x > max) max = x;
    }
  // Check if there is at least one strictly positive data
  if (max == 0.0) return false;
  // Everything is ok
  return true;
}

/* Build default palette
   Cycle through the hue wheel with 10 nuances and increasing darkness */
void Pie::buildDefaultPalette()
{
  const UnsignedLong size(data_.getSize());
  palette_ = Description(size);
  UnsignedLong cycles(size / 12 + 1);
  UnsignedLong paletteIndex(0);
  for (UnsignedLong iCycle = 0; iCycle < cycles; ++iCycle)
    {
      const NumericalScalar value(1.0 - iCycle / static_cast< NumericalScalar >(cycles));
      const UnsignedLong iHueMax(std::min(size - paletteIndex, static_cast< UnsignedLong >(12)));
      for (UnsignedLong iHue = 0; iHue < iHueMax; ++iHue)
        {
          const NumericalScalar hue(30.0 * iHue);
          palette_[paletteIndex] = ConvertFromHSV(hue, 1.0, value);
          ++paletteIndex;
        }
    }
}

/* Build default labels */
void Pie::buildDefaultLabels()
{
  const UnsignedLong size(data_.getSize());
  labels_ = Description(size);
  for (UnsignedLong i = 0; i < size; ++i) labels_[i] = String(OSS() << "L" << i);
}

/* Method save() stores the object through the StorageManager */
void Pie::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
  adv.saveAttribute( "palette_", palette_ );
  adv.saveAttribute( "radius_", radius_ );
  adv.saveAttribute( "center_", center_ );
  adv.saveAttribute( "labels_", labels_ );
}

/* Method load() reloads the object from the StorageManager */
void Pie::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
  adv.loadAttribute( "palette_", palette_ );
  adv.loadAttribute( "radius_", radius_ );
  adv.loadAttribute( "center_", center_ );
  adv.loadAttribute( "labels_", labels_ );
}

END_NAMESPACE_OPENTURNS
