//                                               -*- C++ -*-
/**
 *  @file  ProcessSample.cxx
 *  @brief ProcessSample Class
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

#include "ProcessSample.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"
#include "Drawable.hxx"
#include "Description.hxx"

BEGIN_NAMESPACE_OPENTURNS


TEMPLATE_CLASSNAMEINIT(PersistentCollection<ProcessSample>);
TEMPLATE_CLASSNAMEINIT(PersistentCollection<TimeSeries>);
static Factory<PersistentCollection<TimeSeries> > RegisteredFactory1("PersistentCollection<TimeSeries>");


CLASSNAMEINIT(ProcessSample);

static Factory<ProcessSample> RegisteredFactory("ProcessSample");

ProcessSample::ProcessSample()
  : PersistentObject(),
    dimension_(1),
    timeGrid_(),
    data_(0)
{
  // Nothing to do
}

/* Default constructor */
ProcessSample::ProcessSample(const UnsignedLong size,
                             const TimeSeries & timeSeries)
  : PersistentObject(),
    dimension_(timeSeries.getDimension()),
    timeGrid_(timeSeries.getTimeGrid()),
    data_(TimeSeriesPersistentCollection(size, timeSeries))
{
  // Nothing to do
}


ProcessSample::ProcessSample(const RegularGrid & timeGrid,
                             const UnsignedLong size,
                             const UnsignedLong dimension)
  : PersistentObject(),
    dimension_(dimension),
    timeGrid_(timeGrid),
    data_(TimeSeriesPersistentCollection(size, TimeSeries(timeGrid, NumericalSample(timeGrid.getN(), dimension))))
{
  // Nothing to do
}

ProcessSample::ProcessSample(const TimeSeriesCollection & collection)
  : PersistentObject(),
    dimension_(0),
    timeGrid_(),
    data_(0)
{
  const UnsignedLong collectionSize(collection.getSize());
  for (UnsignedLong i = 0; i < collectionSize; ++i ) add(collection[i]);
}

/* Virtual constructor */
ProcessSample * ProcessSample::clone() const
{
  return new ProcessSample(*this);
}

/* String converter */
String ProcessSample::__repr__() const
{
  OSS oss;
  oss << "class=" << ProcessSample::GetClassName()
      << " timeGrid = " << timeGrid_
      << " values = " << data_;
  return oss;
}

String ProcessSample::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << "[";
  String separator("");
  for (UnsignedLong i = 0; i < data_.getSize(); ++i, separator = "\n") oss << separator << offset << "time series " << i << ":\n" << (*this)[i].__str__(offset);
  oss << "]";
  return oss;
}

void ProcessSample::add(const TimeSeries & timeSeries)
{
  if (getSize() == 0)
    {
      data_.add(timeSeries);
      timeGrid_ = timeSeries.getTimeGrid();
      dimension_ = timeSeries.getDimension();
    }
  else if ((dimension_ == timeSeries.getDimension()) && (timeGrid_ == timeSeries.getTimeGrid())) data_.add(timeSeries);
  else throw InvalidArgumentException(HERE) << "Error; could not add the timeSeries. Either its dimenson or its time grid are incompatible.";
}


/* Operators accessors */
TimeSeries & ProcessSample::operator [] (const UnsignedLong index)
{
  if (index > getSize() - 1) throw InvalidArgumentException(HERE)  << " Error - index should be between 0 and " << getSize() - 1;
  return data_[index];
}

const TimeSeries & ProcessSample::operator [] (const UnsignedLong index) const
{
  return data_[index];
}

/* Method __getitem__() - Python use */
TimeSeries ProcessSample::__getitem__ (const UnsignedLong i) const
{
  return (*this)[i];
}

/* Method __setitem__() is for Python */
void ProcessSample::__setitem__(const UnsignedLong i, const TimeSeries & timeSeries)
{
  (*this)[i] = timeSeries;
}

/* Time grid accessors */
RegularGrid ProcessSample::getTimeGrid() const
{
  return timeGrid_;
}

/* Dimension accessors */
UnsignedLong ProcessSample::getDimension() const
{
  return dimension_;
}

/* Dimension accessors */
UnsignedLong ProcessSample::getSize() const
{
  return data_.getSize();
}

TimeSeries ProcessSample::computeMean() const
{
  const UnsignedLong size(getSize());
  if (size == 0) return TimeSeries();
  if (size == 1) return data_[0];
  TimeSeries result(data_[0]);
  const UnsignedLong length(result.getSize());
  // Aggregate the mean on the fly
  for (UnsignedLong k = 1; k < size; ++k)
    for (UnsignedLong t = 0; t < length ; ++t)
      result.getValueAtIndex(t) += data_[k].getValueAtIndex(t);
  // Normalize the result
  const NumericalScalar factor(1.0 / size);
  for (UnsignedLong t = 0; t < length ; ++t)
    result.getValueAtIndex(t) *= factor;
  return result;
}

/* Compute the sample of temporal means of each time series */
NumericalSample ProcessSample::computeTemporalMean() const
{
  const UnsignedLong size(getSize());
  const UnsignedLong dimension(getDimension());
  NumericalSample result(size, dimension);
  for (UnsignedLong i = 0; i < size; ++i) result[i] = data_[i].getTemporalMean();
  return result;
}

/*
 * Method computeQuantilePerComponent() gives the quantile per component of the sample
 */
TimeSeries ProcessSample::computeQuantilePerComponent(const NumericalScalar prob) const
{
  const UnsignedLong size(getSize());
  if (size == 0) return TimeSeries();
  if (size == 1) return data_[0];
  // This initialization set the correct time grid into result
  TimeSeries result(timeGrid_, dimension_);
  const UnsignedLong length(data_[0].getSize());
  // Loop over the time indices
  for (UnsignedLong i = 0; i < length; ++i)
    {
      NumericalSample dataI(size, dimension_);
      for (UnsignedLong j = 0; j < size; ++j)
        dataI[j] = data_[j].getValueAtIndex(i);
      result.getValueAtIndex(i) = dataI.computeQuantilePerComponent(prob);
    }
  return result;
}

/* Draw a marginal of the ProcessSample, ie the collection of all the TimeSeries marginals */
Graph ProcessSample::drawMarginal(const UnsignedLong index) const
{
  if (index > getDimension() - 1 ) throw InvalidArgumentException(HERE) << "Error : indice should be between [0, " << getDimension() - 1 << "]";

  // Discretization of the x axis
  const String title(OSS() << getName() << " - " << index << " marginal" );
  Graph graph(title, "Time", "Values", true, "topright");
  const UnsignedLong size(data_.getSize());
  const Description colors(Drawable::BuildDefaultPalette(size));
  for (UnsignedLong i = 0; i < size; ++i)
    {
      Drawable drawable(data_[i].drawMarginal(index).getDrawable(0));
      drawable.setColor(colors[i]);
      graph.add(drawable);
    }
  return graph;
}

/* Method save() stores the object through the StorageManager */
void ProcessSample::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "dimension_", dimension_);
  adv.saveAttribute( "timeGrid_", timeGrid_);
  adv.saveAttribute( "data_", data_ );
}

/* Method load() reloads the object from the StorageManager */
void ProcessSample::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "dimension_", dimension_);
  adv.loadAttribute( "timeGrid_", timeGrid_);
  adv.loadAttribute( "data_", data_ );
}

END_NAMESPACE_OPENTURNS
