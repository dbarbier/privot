//                                               -*- C++ -*-
/**
 *  @file  Cloud.cxx
 *  @brief Cloud class for cloud plots
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
#include "Cloud.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Cloud);

static Factory<Cloud> RegisteredFactory("Cloud");

/* Default constructor */
Cloud::Cloud(const NumericalSample & data,
             const String & legend)
  : DrawableImplementation(data, legend)
{
  // Check data validity
  setData(data);
}

/* Constructor from complex numbers */
Cloud::Cloud(const NumericalComplexCollection & data,
             const String & legend)
  : DrawableImplementation(NumericalSample(2, 0), legend)
{
  // Convert the complex numbers into a NumericalSample
  const UnsignedLong size(data.getSize());
  NumericalSample sample(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      sample[i][0] = data[i].real();
      sample[i][1] = data[i].imag();
    }
  // Check data validity
  setData(sample);
}

/* Contructor from 2 data sets */
Cloud::Cloud(const NumericalSample & dataX,
             const NumericalSample & dataY,
             const String & legend):
  DrawableImplementation(NumericalSample(2, 0), legend)
{
  const UnsignedLong size(dataX.getSize());
  if (dataY.getSize() != size) throw InvalidArgumentException(HERE) << "Error: cannot build a Cloud based on two numerical samples with different size.";
  if ((dataX.getDimension() != 1) || (dataY.getDimension() != 1)) throw InvalidArgumentException(HERE) << "Error: cannot build a Cloud based on two numerical samples of dimension greater than 1.";
  NumericalSample dataFull(size, 2);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      dataFull[i][0] = dataX[i][0];
      dataFull[i][1] = dataY[i][0];
    }
  // Check data validity
  setData(dataFull);
}

/* Constructor with parameters */
Cloud::Cloud(const NumericalSample & data,
             const String & color,
             const String & pointStyle,
             const String & legend)
  : DrawableImplementation(data, legend)
{
  // Check data validity
  setData(data);
  setColor(color);
  setPointStyle(pointStyle);
}

/* String converter */
String Cloud::__repr__() const
{
  OSS oss;
  oss << "class=" << Cloud::GetClassName()
      << " name=" << getName()
      << " derived from " << DrawableImplementation::__repr__();
  return oss;
}

/* Draw method */
String Cloud::draw() const
{
  dataFileName_ = "";
  OSS oss;
  if (pointStyle_ != "none")
    {
      // Stores the data in a temporary file
      oss << DrawableImplementation::draw() << "\n";
      // The specific R command for drawing
      oss << "points(dataOT[,1], dataOT[,2]";
      const String code((OSS() << getPointCode(pointStyle_)));
      oss << ",pch=" << (pointStyle_ == "dot" ? "\".\"" : code)
	  << ",col=\"" << color_
	  << "\",lwd=" << lineWidth_
	  << ")";
    }
  return oss;
}

/* Clone method */
Cloud * Cloud::clone() const
{
  return new Cloud(*this);
}

/* Check validity of data */
Bool Cloud::isValidData(const NumericalSample & data) const
{
  return (data.getDimension() == 2);
}

/* Method save() stores the object through the StorageManager */
void Cloud::save(Advocate & adv) const
{
  DrawableImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void Cloud::load(Advocate & adv)
{
  DrawableImplementation::load(adv);
}



END_NAMESPACE_OPENTURNS
