//                                               -*- C++ -*-
/**
 *  @file  Contour.cxx
 *  @brief Contour class for contourchart plots
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
#include <cstdio>
#include <cstdlib>

#include "Contour.hxx"
#include "PersistentObjectFactory.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Contour);

static Factory<Contour> RegisteredFactory("Contour");

/* Default constructor */
Contour::Contour(const UnsignedLong dimX,
                 const UnsignedLong dimY,
                 const NumericalSample & data,
                 const String & legend)
  : DrawableImplementation(data, legend),
    x_(NumericalSample(dimX, 1)),
    y_(NumericalSample(dimY, 1)),
    levels_(NumericalPoint(ResourceMap::GetAsUnsignedLong( "Contour-DefaultLevelsNumber" ))),
    labels_(ResourceMap::GetAsUnsignedLong( "Contour-DefaultLevelsNumber" )),
    drawLabels_(true)
{
  if (dimX < 2) throw InvalidArgumentException(HERE) << "Error: the x dimension must be greater or equal to 2";
  if (dimY < 2) throw InvalidArgumentException(HERE) << "Error: the y dimension must be greater or equal to 2";
  if (dimX * dimY != data.getSize()) throw InvalidArgumentException(HERE) << "Error: the given dimensions are not compatible with the data";
  // Check data validity
  setData(data);
  // By default, x is assumed to be equally spaced in [0, 1]
  for (UnsignedLong i = 0; i < dimX; ++i) x_[i][0] = NumericalScalar(i) / (dimX - 1.0);
  // By default, y is assumed to be equally spaced in [0, 1]
  for (UnsignedLong i = 0; i < dimY; ++i) y_[i][0] = NumericalScalar(i) / (dimY - 1.0);
  // Build the levels
  buildDefaultLevels();
  // Build the labels
  buildDefaultLabels();
}

/* Constructor with parameters */
Contour::Contour(const NumericalSample & x,
                 const NumericalSample & y,
                 const NumericalSample & data,
                 const NumericalPoint & levels,
                 const Description & labels,
                 const Bool drawLabels,
                 const String & legend)
  : DrawableImplementation(data, legend),
    x_(x),
    y_(y),
    levels_(levels),
    labels_(labels),
    drawLabels_(drawLabels)
{
  if (levels.getDimension() == 0) buildDefaultLevels();
  if (drawLabels && (labels.getSize() == 0)) buildDefaultLabels();
  if (drawLabels && (levels.getDimension() > 0) && (labels.getSize() > 0) && (levels.getDimension() != labels.getSize())) throw InvalidArgumentException(HERE) << "Error: the levels are incompatible with the labels";
  // Check data validity
  setData(data);
}

/* String converter */
String Contour::__repr__() const
{
  OSS oss;
  oss << "class=" << Contour::GetClassName()
      << " name=" << getName()
      << " x=" << x_
      << " y=" << y_
      << " levels=" << levels_
      << " labels=" << labels_
      << " show labels=" << drawLabels_
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Accessor for first coordinate */
NumericalSample Contour::getX() const
{
  return x_;
}

void Contour::setX(const NumericalSample & x)
{
  x_ = x;
}

/* Accessor for second coordinate */
NumericalSample Contour::getY() const
{
  return y_;
}

void Contour::setY(const NumericalSample & y)
{
  y_ = y;
}

/* Accessor for levels */
NumericalPoint Contour::getLevels() const
{
  return levels_;
}

void Contour::setLevels(const NumericalPoint & levels)
{
  levels_ = levels;
  if (levels.getDimension() != labels_.getSize()) buildDefaultLabels();
}

/* Accessor for labels */
Description Contour::getLabels() const
{
  return labels_;
}

void Contour::setLabels(const Description & labels)
{
  if (labels.getSize() != levels_.getDimension()) throw InvalidArgumentException(HERE) << "Error: the labels size must be equal to the levels dimension";
  labels_ = labels;
}

/* Accessor for drawLabels */
Bool Contour::getDrawLabels() const
{
  return drawLabels_;
}

void Contour::setDrawLabels(const Bool & drawLabels)
{
  drawLabels_ = drawLabels;
}

/* Accessor for boundingbox */
Contour::BoundingBox Contour::getBoundingBox() const
{
  BoundingBox boundingBox(BoundingBoxSize);
  boundingBox[0] = x_.getMin()[0];
  boundingBox[1] = x_.getMax()[0];
  boundingBox[2] = y_.getMin()[0];
  boundingBox[3] = y_.getMax()[0];
  return boundingBox;
}

/* Clean all the temporary data created by draw() method */
void Contour::clean() const
{
  DrawableImplementation::clean();
  if ((xFileName_ != "") && (remove(xFileName_.c_str()) == -1)) LOGWARN(OSS() << "GraphImplementation: error trying to remove file " << xFileName_);
  if ((yFileName_ != "") && (remove(yFileName_.c_str()) == -1)) LOGWARN(OSS() << "GraphImplementation: error trying to remove file " << yFileName_);
}

/* Draw method */
String Contour::draw() const
{
  xFileName_ = "";
  yFileName_ = "";
  OSS oss;
  // Stores the data in a temporary file
  // For a contour, it is a matrix of values
  oss << DrawableImplementation::draw() << "\n";
  // The specific R command for drawing
  // Here we store the discretizations of the axes
  if (x_.getDimension() * x_.getSize() > ResourceMap::GetAsUnsignedLong("DrawableImplementation-DataThreshold"))
    {
      xFileName_ = x_.storeToTemporaryFile();
      yFileName_ = y_.storeToTemporaryFile();
      oss << "x <- dataOT.matrix(read.table(\"" << xFileName_ << "\"))\n"
          << "y <- dataOT.matrix(read.table(\"" << yFileName_ << "\"))\n";
    }
  else
    {
      oss << "x <- " << x_.streamToRFormat() << "\n"
          << "y <- " << y_.streamToRFormat() << "\n";
    }
  oss << "dataOT <- matrix(dataOT, length(x), length(y))\n"
      << "levels=c(";
  const UnsignedLong length(levels_.getSize() - 1);
  for(UnsignedLong i = 0; i < length; ++i)
    {
      oss << levels_[i] << ",";
    }
  oss << levels_[length] << ")\n"
      << "labels=c(\"";
  for(UnsignedLong i = 0; i < length; ++i)
    {
      oss << labels_[i] << "\",\"";
    }
  oss << labels_[length] << "\")\n";
  oss << "contour(x, y, dataOT, "
      << "levels=levels,"
      << "labels=labels,"
      << "drawlabels=" << (drawLabels_ ? "TRUE" : "FALSE") << ","
      << "lty=\"" << lineStyle_ << "\","
      << "col=\"" << color_ << "\","
      << "lwd=" << lineWidth_
      << ", add=TRUE, axes=FALSE)";
  return oss;
}

/* Clone method */
Contour * Contour::clone() const
{
  return new Contour(*this);
}

/* Check for data validity */
Bool Contour::isValidData(const NumericalSample & data) const
{
  return (data.getDimension() == 1);
}

/* Build default levels using quantiles associated with regularly spaced probability levels
 */
void Contour::buildDefaultLevels(const UnsignedLong number)
{
  // Use the empirical quantiles
  const NumericalSample sortedData(data_.sort());
  const UnsignedLong size(data_.getSize());
  levels_ = NumericalPoint(number);
  for (UnsignedLong i = 0; i < number; ++i)
    {
      levels_[i] = sortedData[static_cast<UnsignedLong>(size * (i + 0.5) / number)][0];
    }
}

/* Build default labels by taking the level values */
void Contour::buildDefaultLabels()
{
  const UnsignedLong number(levels_.getDimension());
  labels_ = Description(number);
  for (UnsignedLong i = 0; i < number; ++i)
    {
      labels_[i] = OSS() << levels_[i];
    }
}

/* Method save() stores the object through the StorageManager */
void Contour::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
  adv.saveAttribute( "x_", x_ );
  adv.saveAttribute( "y_", y_ );
  adv.saveAttribute( "levels_", levels_ );
  adv.saveAttribute( "labels_", labels_ );
  adv.saveAttribute( "drawLabels_", drawLabels_ );
}

/* Method load() reloads the object from the StorageManager */
void Contour::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
  adv.loadAttribute( "x_", x_ );
  adv.loadAttribute( "y_", y_ );
  adv.loadAttribute( "levels_", levels_ );
  adv.loadAttribute( "labels_", labels_ );
  adv.loadAttribute( "drawLabels_", drawLabels_ );
}



END_NAMESPACE_OPENTURNS
