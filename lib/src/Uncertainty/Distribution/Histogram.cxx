//                                               -*- C++ -*-
/**
 *  @file  Histogram.cxx
 *  @brief The Histogram distribution
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
#include "Histogram.hxx"
#include "BarPlot.hxx"
#include "Curve.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



TEMPLATE_CLASSNAMEINIT(PersistentCollection<HistogramPair>);
static Factory<PersistentCollection<HistogramPair> > RegisteredFactory("PersistentCollection<HistogramPair>");




CLASSNAMEINIT(Histogram);

static Factory<Histogram> RegisteredFactory_alt("Histogram");

/* Default constructor */
Histogram::Histogram()
  : NonEllipticalDistribution("Histogram"),
    first_(0.0),
    collection_(0),
    cumulatedWidth_(0),
    surface_(0.0)
{
  // This call set also the range.
  setPairCollection(HistogramPairCollection(1, HistogramPair(1.0, 1.0)));
  setDimension( 1 );
}

/* Parameters constructor */
Histogram::Histogram(const NumericalScalar first,
                     const HistogramPairCollection & collection)
  : NonEllipticalDistribution("Histogram"),
    first_(first),
    collection_(0),
    cumulatedWidth_(0),
    surface_(0.0)
{
  // This call set also the range.
  setPairCollection(collection);
  setDimension( 1 );
}

/* Comparison operator */
Bool Histogram::operator ==(const Histogram & other) const
{
  if (this == &other) return true;
  return (first_ == other.first_) && (collection_ == other.collection_);
}

/* String converter */
String Histogram::__repr__() const
{
  OSS oss;
  oss << "class=" << Histogram::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " first=" << first_
      << " pair collection=" << collection_;
  return oss;
}

String Histogram::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(origin = " << first_;
  for (UnsignedLong i = 0; i < collection_.getSize(); ++i) oss << ", {w" << i << " = " << collection_[i].getWidth() << ", h" << i << " = " << collection_[i].getHeight() << "}";
  oss << ")";
  return oss;
}

/* Virtual constructor */
Histogram * Histogram::clone() const
{
  return new Histogram(*this);
}

/* Compute the numerical range of the distribution given the parameters values */
void Histogram::computeRange()
{
  const UnsignedLong size(cumulatedWidth_.getSize());
  if (size == 0) return;
  setRange(Interval(first_, first_ + cumulatedWidth_[size - 1]));
}

/* Get one realization of the distribution */
NumericalPoint Histogram::getRealization() const
{
  return computeQuantile(RandomGenerator::Generate());
}


/* Get the DDF of the distribution */
NumericalPoint Histogram::computeDDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  return NumericalPoint(1, 0.0);
}


/* Get the PDF of the distribution */
NumericalScalar Histogram::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - first_);
  const UnsignedLong size(collection_.getSize());
  if ((x <= 0.0) || (x >= cumulatedWidth_[size - 1])) return 0.0;
  NumericalScalar lower(0.0);
  // We shift x bin by bin until x falls in the current bin.
  for (UnsignedLong i = 0; i < size; ++i)
    {
      x -= lower;
      lower = collection_[i].getWidth();
      if (x < lower) return collection_[i].getHeight();
    }
  // Should never go there
  return 0.0;
}


/* Get the CDF of the distribution */
NumericalScalar Histogram::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  NumericalScalar x(point[0] - first_);
  const UnsignedLong size(collection_.getSize());
  if (x <= 0.0) return 0.0;
  if (x >= cumulatedWidth_[size - 1]) return 1.0;
  NumericalScalar lower(0.0);
  NumericalScalar cdf(0.0);
  // We shift x bin by bin until x falls in the current bin. We aggregate the contribution of all the bins and make a proportional contribution for the last bin.
  for (UnsignedLong i = 0; i < size; ++i)
    {
      x -= lower;
      lower = collection_[i].getWidth();
      cdf += collection_[i].getSurface();
      if (x <= lower)
        {
          const NumericalScalar value(cdf + (x - lower) * collection_[i].getHeight());
          return value;
        }
    }
  // Should never go there
  return 1.0;
}

/** Get the PDFGradient of the distribution */
NumericalPoint Histogram::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/** Get the CDFGradient of the distribution */
NumericalPoint Histogram::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/* Get the quantile of the distribution */
NumericalScalar Histogram::computeScalarQuantile(const NumericalScalar prob,
                                                 const Bool tail,
                                                 const NumericalScalar precision) const
{
  const NumericalScalar p(tail ? 1.0 - prob : prob);
  const UnsignedLong size(collection_.getSize());
  // Research of the containing bin
  UnsignedLong index(UnsignedLong(p * size));
  NumericalScalar currentProba(cumulatedSurface_[index]);
  UnsignedLong currentIndex(index);
  // Basic search: upper bound. The loop must end because cumulatedSurface_[size - 1] = 1.0 and prob < 1.0
  while (p >= currentProba)
    {
      ++currentIndex;
      currentProba = cumulatedSurface_[currentIndex];
    }
  // At the end of the loop, we are sure that currentProba > p
  // If index < currentIndex, it means that p is associated with bin number currentIndex. Do a linear interpolation.
  if (index < currentIndex)
    {
      // currentIndex is now the number of the bin associated with prob
      return first_ + cumulatedWidth_[currentIndex] + collection_[currentIndex].getWidth() * (p - currentProba) / collection_[currentIndex].getSurface();
    }
  // Here we know that we have to go downstairs. We must check that currentIndex remains >= 0 in the loop.
  while ((p < currentProba) && (currentIndex > 0))
    {
      --currentIndex;
      currentProba = cumulatedSurface_[currentIndex];
    }
  // At the end of the loop, either p < cumulatedSurface_[0], which means that prob is associated with the first bin...
  if (currentIndex == 0) return first_ + collection_[0].getWidth() * p / currentProba;
  // ... or p >= cumulatedSurface_[currentIndex], which means that p is associated with the bin number currentIndex + 1. Do a linear interpolation.
  return first_ + cumulatedWidth_[currentIndex] + collection_[currentIndex + 1].getWidth() * (p - currentProba) / collection_[currentIndex + 1].getSurface();
}

/* Compute the mean of the distribution */
void Histogram::computeMean() const
{
  NumericalScalar mean(first_);
  const UnsignedLong size(collection_.getSize());
  NumericalScalar lower(0.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalScalar width(collection_[i].getWidth());
      NumericalScalar upper(lower + width);
      mean += 0.5 * collection_[i].getSurface() * (lower + upper);
      lower = upper;
    }
  mean_ = NumericalPoint(1, mean);
  isAlreadyComputedMean_ = true;
}

/* Compute the covariance of the distribution */
void Histogram::computeCovariance() const
{
  NumericalScalar value(0.0);
  const UnsignedLong size(collection_.getSize());
  covariance_ = CovarianceMatrix(1);
  // Since variance is invariant by translation, we center the data for numerical stability
  NumericalScalar lower(first_ - getMean()[0]);
  for (UnsignedLong i = 0; i < size; i++)
    {
      const NumericalScalar width(collection_[i].getWidth());
      NumericalScalar upper(lower + width);
      value += collection_[i].getSurface() * (lower * lower + lower * upper + upper * upper);
      lower = upper;
    }
  covariance_(0, 0) = value / 3.0;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
Histogram::NumericalPointWithDescriptionCollection Histogram::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  const UnsignedLong size(collection_.getSize());
  NumericalPointWithDescription point(1 + 2 * size);
  Description description(1 + 2 * size);
  point[0] = first_;
  description[0] = "first";
  for (UnsignedLong i = 0; i < size; ++i)
    {
      point[2 * i + 1] = collection_[i].getWidth();
      point[2 * i + 2] = collection_[i].getHeight();
      {
        OSS oss;
        oss << "width_" << i;
        description[2 * i + 1] = oss;
      }
      {
        OSS oss;
        oss << "height_" << i;
        description[2 * i + 2] = oss;
      }
    }
  point.setDescription(description);
  point.setName(getDescription()[0]);
  parameters[0] = point;
  return parameters;
}




/* Interface specific to Histogram */

/* First point accessor */
void Histogram::setFirst(const NumericalScalar first)
{
  if (first != first_)
    {
      first_ = first;
      isAlreadyComputedMean_ = false;
      // The covariance does not depend on first
      computeRange();
    }
}

NumericalScalar Histogram::getFirst() const
{
  return first_;
}


/* Collection accessor */
void Histogram::setPairCollection(const HistogramPairCollection & collection)
{
  surface_ = 0.0;
  const UnsignedLong size(collection.getSize());
  cumulatedWidth_ = NumericalPoint(size);
  cumulatedSurface_ = NumericalPoint(size);
  // first, check that all the heights and widths are >=0
  for (UnsignedLong i = 0; i < size; ++i)
    {
      NumericalScalar height(collection[i].getHeight());
      if (height < 0.0) throw InvalidArgumentException(HERE) << "Error: all the heights must be >= 0, here values=" << collection;
      NumericalScalar width(collection[i].getWidth());
      if (width <= 0.0) throw InvalidArgumentException(HERE) << "Error: all the widths must be > 0, here value=" << collection;
      surface_ += height * width;
      cumulatedWidth_[i] = width + (i == 0 ? 0 : cumulatedWidth_[i - 1]);
      cumulatedSurface_[i] = surface_;
    }
  // Check if the surface is strictly positive
  if (surface_ < ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultCDFEpsilon")) throw InvalidArgumentException(HERE) << "Error: the surface of the histogram is zero.";
  // Normalization
  collection_ = HistogramPairCollection(size);
  NumericalScalar normalizationFactor(1.0 / surface_);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      collection_[i] = HistogramPair(collection[i].getWidth(), collection[i].getHeight() * normalizationFactor);
      cumulatedSurface_[i] *= normalizationFactor;
    }
  isAlreadyComputedMean_ = false;
  isAlreadyComputedCovariance_ = false;
  computeRange();
}

Histogram::HistogramPairCollection Histogram::getPairCollection() const
{
  return collection_;
}

/** Draw the PDF of the Histogram using a specific presentation */
Graph Histogram::drawPDF() const
{
  const UnsignedLong lastIndex(cumulatedWidth_.getSize() - 1);
  // Must prefix explicitely by the class name in order to avoid conflict with the methods in the upper class
  return Histogram::drawPDF(first_ - 0.5 * collection_[0].getWidth(), first_ + cumulatedWidth_[lastIndex] + 0.5 * collection_[lastIndex].getWidth());
}

/** Draw the PDF of the Histogram using a specific presentation */
Graph Histogram::drawPDF(const NumericalScalar xMin,
                         const NumericalScalar xMax,
                         const UnsignedLong pointNumber) const
{
  if (xMax <= xMin) throw InvalidArgumentException(HERE) << "Error: cannot draw a PDF with xMax >= xMin, here xmin=" << xMin << " and xmax=" << xMax;
  const String title(OSS() << getName() << " PDF");
  const String xName(getDescription()[0]);
  Graph graphPDF(title, xName, "PDF", true, "topright");
  const UnsignedLong size(collection_.getSize());
  // Check for the border cases
  // If the histogram is completely at the right or at the left of the plot range,
  // just draw an horizontal line
  if ((xMax < first_) || (xMin > first_ + cumulatedWidth_[size - 1]))
    {
      NumericalSample data(2, 2);
      data[0][0] = xMin;
      data[0][1] = 0.0;
      data[1][0] = xMax;
      data[1][1] = 0.0;
      graphPDF.add(Curve(data, "red", "solid", 2, title));
      return graphPDF;
    }
  // Find the index of the left bar to draw
  UnsignedLong indexLeft(0);
  while (first_ + cumulatedWidth_[indexLeft] < xMin) ++indexLeft;
  // Another special case: the plot range covers only partially a unique bar
  if (first_ + cumulatedWidth_[indexLeft] >= xMax)
    {
      NumericalSample data(2, 2);
      data[0][0] = xMin;
      data[0][1] = collection_[indexLeft].getHeight();
      data[1][0] = xMax;
      data[1][1] = collection_[indexLeft].getHeight();
      graphPDF.add(Curve(data, "red", "solid", 2, title));
      return graphPDF;
    }
  // Find the index of the right bar to draw
  UnsignedLong indexRight(indexLeft);
  UnsignedLong shiftFull(0);
  while ((indexRight < size) && (first_ + cumulatedWidth_[indexRight] < xMax)) ++indexRight;
  // The graph is made of full bars for the class indices between indexLeft and indexRight
  // The graph can include completely the first class or not. It is checked using first_.
  // The graph can include completely the last class or not. It is checked usind indexRight == size.
  // !!! Only the first part of the graph has a label !!!
  // The first class is completely included
  if (xMin <= first_)
    {
      NumericalSample data(2, 2);
      data[0][0] = xMin;
      data[0][1] = 0.0;
      data[1][0] = first_;
      data[1][1] = 0.0;
      graphPDF.add(Curve(data, "red", "solid", 2, title));
    }
  // The first class that appears in the graph is only partially included
  else
    {
      NumericalSample data(3, 2);
      data[0][0] = xMin;
      data[0][1] = collection_[indexLeft].getHeight();
      data[1][0] = first_ + cumulatedWidth_[indexLeft];
      data[1][1] = data[0][1];
      data[2][0] = data[1][0];
      data[2][1] = 0.0;
      graphPDF.add(Curve(data, "red", "solid", 2, title));
      shiftFull = 1;
    }
#ifdef USEBARPLOT
  // Central part of the graph
  NumericalSample data(0, 2);
  for (UnsignedLong i = indexLeft + shiftFull; i < indexRight; ++i)
    {
      NumericalPoint point(2);
      point[0] = collection_[i].getWidth();
      point[1] = collection_[i].getHeight();
      data.add(point);
    }
  // If there is at least one full bar to draw, do it using the BarPlot object
  if (data.getSize() > 0)
    graphPDF.add(BarPlot(data, first_, "red", "solid", "solid", ""));
#else
  // Central part of the graph
  NumericalScalar startX(first_);
  if (indexLeft + shiftFull > 0) startX += cumulatedWidth_[indexLeft + shiftFull];
  for (UnsignedLong i = indexLeft + shiftFull; i < indexRight; ++i)
    {
      NumericalSample data(4, 2);
      data[0][0] = startX;
      data[0][1] = 0.0;
      data[1][0] = startX;
      data[1][1] = collection_[i].getHeight();
      startX += collection_[i].getWidth();
      data[2][0] = startX;
      data[2][1] = collection_[i].getHeight();
      data[3][0] = startX;
      data[3][1] = 0.0;
      graphPDF.add(Curve(data, "red", "solid", 2, ""));
    }
#endif
  // The last class is completely included
  if (indexRight == size)
    {
      NumericalSample data(2, 2);
      data[0][0] = first_ + cumulatedWidth_[size - 1];
      data[0][1] = 0.0;
      data[1][0] = xMax;
      data[1][1] = 0.0;
      graphPDF.add(Curve(data, "red", "solid", 2, ""));
    }
  // The last class that appears in the graph is only partially included
  else
    {
      NumericalSample data(3, 2);
      data[0][0] = first_ + cumulatedWidth_[indexRight - 1];
      data[0][1] = 0.0;
      data[1][0] = data[0][0];
      data[1][1] = collection_[indexRight].getHeight();
      data[2][0] = xMax;
      data[2][1] = collection_[indexRight].getHeight();
      graphPDF.add(Curve(data, "red", "solid", 2, ""));
    }
  return graphPDF;
}

/* Method save() stores the object through the StorageManager */
void Histogram::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
  adv.saveAttribute( "first_", first_);
  adv.saveAttribute( "collection_", collection_);
  adv.saveAttribute( "cumulatedWidth_", cumulatedWidth_);
  adv.saveAttribute( "cumulatedSurface_", cumulatedSurface_);
  adv.saveAttribute( "surface_", surface_);
}

/* Method load() reloads the object from the StorageManager */
void Histogram::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
  adv.loadAttribute( "first_", first_);
  adv.loadAttribute( "collection_", collection_);
  adv.loadAttribute( "cumulatedWidth_", cumulatedWidth_);
  adv.loadAttribute( "cumulatedSurface_", cumulatedSurface_);
  adv.loadAttribute( "surface_", surface_);
  computeRange();
}

END_NAMESPACE_OPENTURNS
