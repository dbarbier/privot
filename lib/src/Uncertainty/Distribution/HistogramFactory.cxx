//                                               -*- C++ -*-
/**
 *  @file  HistogramFactory.cxx
 *  @brief Factory for Histogram distribution
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include "HistogramFactory.hxx"
#include "HistogramPair.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(HistogramFactory);

/* Default constructor */
HistogramFactory::HistogramFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
HistogramFactory * HistogramFactory::clone() const
{
  return new HistogramFactory(*this);
}


/* Here is the interface that all derived class must implement */

HistogramFactory::Implementation HistogramFactory::build(const NumericalSample & sample) const
{
  return buildAsHistogram(sample).clone();
}

HistogramFactory::Implementation HistogramFactory::build() const
{
  return buildAsHistogram().clone();
}

Histogram HistogramFactory::buildAsHistogram(const NumericalSample & sample) const
{
  if (sample.getDimension() != 1) throw InvalidDimensionException(HERE) << "Error: can build an Histogram only if dimension equals 1, here dimension=" << sample.getDimension();
  // Construct the histogram
  // It will extends from min to max.
  const NumericalScalar min(sample.getMin()[0]);
  const NumericalScalar max(sample.getMax()[0]);
  if (max == min) throw InvalidArgumentException(HERE) << "Error: cannot build an Histogram for a sample with constant realizations";
  const UnsignedLong size(sample.getSize());
  NumericalScalar hOpt(sample.computeStandardDeviationPerComponent()[0] * pow(24.0 * sqrt(M_PI) / size, 1.0 / 3.0));
  const UnsignedLong barNumber(static_cast<UnsignedLong>(ceil((max - min) / hOpt + 0.5)));
  // Adjust the bin with in order to match the bin number. Add a small adjustment in order to have bins defined as [x_k, x_k+1[ intervals
  const NumericalScalar delta(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon") * (max - min));
  hOpt = ((max - min) + delta) / barNumber;
  NumericalPoint heights(barNumber, 0.0);
  const NumericalScalar step(1.0 / hOpt);
  // Aggregate the realizations into the bins
  for(UnsignedLong i = 0; i < size; ++i)
    {
      // The index takes values in [[0, barNumber-1]] because min <= sample[i][0] <= max and step < barNumber / (max - min)
      const UnsignedLong index(static_cast<UnsignedLong>(floor((sample[i][0] - min) * step)));
      heights[index] += 1.0;
    }
  const NumericalScalar inverseArea(1.0 / (hOpt * size));
  Histogram::HistogramPairCollection collection(barNumber);
  for(UnsignedLong i = 0; i < barNumber; ++i) collection[i] = HistogramPair(hOpt, heights[i] * inverseArea);
  return Histogram(min, collection);
}

Histogram HistogramFactory::buildAsHistogram() const
{
  return Histogram();
}

END_NAMESPACE_OPENTURNS
