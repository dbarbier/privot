//                                               -*- C++ -*-
/**
 *  @file  Pairs.cxx
 *  @brief Pairs class for contourchart plots
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
 */
#include <cstdio>
#include <cstdlib>


#include "PersistentObjectFactory.hxx"
#include "Log.hxx"
#include "ResourceMap.hxx"
#include "Pairs.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Pairs);


static Factory<Pairs> RegisteredFactory("Pairs");

/* Default constructor */
Pairs::Pairs(const NumericalSample & data,
             const String & title)
  : DrawableImplementation(data)
{
  if(!isValidData(data)) throw InvalidArgumentException(HERE) << "Error: the data dimension must be greater or equal to 2";
  labels_ = data_.getDescription();
  if(labels_.getSize() == 0) buildDefaultLabels();
}

/* Constructor with parameters*/
Pairs::Pairs(const NumericalSample & data,
             const String & title,
             const Description labels,
             const String color,
             const String pointStyle)
  : DrawableImplementation(data)
{
  if(!isValidData(data)) throw InvalidArgumentException(HERE) << "Error: the data dimension must be greater or equal to 2";
  setTitle(title);
  setColor(color);
  setLabels(labels);
  setPointStyle(pointStyle);
}


/* String converter */
String Pairs::__repr__() const
{
  OSS oss;
  oss << "class=" << Pairs::GetClassName()
      << " name=" << getName()
      << " data=" << data_
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Accessor for first coordinate */
NumericalSample Pairs::getData() const
{
  return data_;
}

void Pairs::setData(const NumericalSample & data)
{
  if(!isValidData(data)) throw InvalidArgumentException(HERE) << "Error: the data dimension must be greater or equal to 2";
  data_ = data;
}

/* Accessor for labels */
Description Pairs::getLabels() const
{
  return labels_;
}

void Pairs::setLabels(const Description & labels)
{
  if (labels.getSize() != data_.getDimension()) throw InvalidArgumentException(HERE) << "Error: the labels size must be equal equal to the data dimension";
  labels_ = labels;
}

/* Clean all the temporary data created by draw() method */
void Pairs::clean() const
{
  DrawableImplementation::clean();
  if ((dataFileName_ != "") && (remove(dataFileName_.c_str()) == -1)) LOGWARN(OSS() << "Could not remove file " << dataFileName_);
}

/* Draw method */
String Pairs::draw() const
{
  dataFileName_ = "";
  OSS oss;
  // Stores the data in a temporary file
  // The specific R command for drawing
  oss << "dim_ <- " << data_.getDimension() << "\n" ;
  oss << "size_ <- " << data_.getSize() << "\n";
  oss << DrawableImplementation::draw() << "\n";
  oss << "description=c(";
  const UnsignedLong length(data_.getDimension());
  for(UnsignedLong i = 0; i < length - 1; ++i) oss << "\"" << labels_[i] << "\"" << ",";
  oss << "\"" << labels_[length - 1] << "\"" << ") \n";
  // DataFrame organisation
  const String code((OSS() << getPointCode(pointStyle_)));
  oss << "dataOT = data.frame(dataOT) \n";
  oss << "names(dataOT) <- description \n";
  oss << "points(pairs(dataOT "
      << ",pch=" << (pointStyle_ == "dot" ? "\".\"" : code)
      << ",col=\"" << color_ << "\""
      << ",main=\"" << getTitle() << "\"))";
  return oss;
}

/* Clone method */
Pairs * Pairs::clone() const
{
  return new Pairs(*this);
}

String Pairs::getTitle() const
{
  return title_;
}
void Pairs::setTitle(const String & title)
{
  title_ = title;
}


/* Check for data validity */
Bool Pairs::isValidData(const NumericalSample & data) const
{
  return (data.getDimension() >= 2);
}

/* Build default labels by taking the level values */
void Pairs::buildDefaultLabels()
{
  const UnsignedLong number(data_.getDimension());
  labels_ = Description(number);
  for (UnsignedLong i = 0; i < number; ++i) labels_[i] = OSS() << "V" << i + 1;
}

/* Method save() stores the object through the StorageManager */
void Pairs::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
  adv.saveAttribute( "title_", title_ );
  adv.saveAttribute( "labels_", labels_ );
}

/* Method load() reloads the object from the StorageManager */
void Pairs::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
  adv.loadAttribute( "title_", title_ );
  adv.loadAttribute( "labels_", labels_ );
}



END_NAMESPACE_OPENTURNS
