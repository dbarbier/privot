//                                               -*- C++ -*-
/**
 *  @file  WelchFactory.cxx
 *  @brief Top-level class for all spectral model factories
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
 *  @author schueller
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "WelchFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"
#include "NumericalSample.hxx"
#include "Collection.hxx"
#include "HermitianMatrix.hxx"
#include "NumericalPoint.hxx"
#include "Hamming.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(WelchFactory);
static Factory<WelchFactory> RegisteredFactory("WelchFactory");

typedef Collection<NumericalComplex> NumericalComplexCollection;
typedef Collection<HermitianMatrix>  HermitianMatrixCollection;

/* Default constructor */
WelchFactory::WelchFactory(const String & name)
  : SpectralModelFactoryImplementation(name),
    window_(Hamming()),
    blockNumber_(1),
    overlap_(0.0)
{
  // Nothing to do
}

WelchFactory::WelchFactory(const FilteringWindows & window,
                           const UnsignedLong blockNumber,
                           const NumericalScalar overlap,
                           const String & name)
  : SpectralModelFactoryImplementation(name),
    window_(window),
    blockNumber_(0),
    overlap_(0.0)
{
  setBlockNumber(blockNumber);
  setOverlap(overlap);
}

/* Virtual constructor */
WelchFactory * WelchFactory::clone() const
{
  return new WelchFactory(*this);
}

/* FilteringWindows accessor */
FilteringWindows WelchFactory::getFilteringWindows() const
{
  return window_;
}

void WelchFactory::setFilteringWindows(const FilteringWindows & window)
{
  window_ = window;
}

/* String converter */
String WelchFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << WelchFactory::GetClassName()
      << " window = " << window_
      << " blockNumber = " << blockNumber_
      << " overlap = " << overlap_;
  return oss;
}

/* String converter */
String WelchFactory::__str__(const String & offset) const
{
  return __repr__();
}

/* Number of blockNumber accessor */
UnsignedLong WelchFactory::getBlockNumber() const
{
  return blockNumber_;
}

void WelchFactory::setBlockNumber(const UnsignedLong blockNumber)
{
  if (blockNumber < 1) throw InvalidArgumentException(HERE) << "Error: the number of blocks should be at least 1";
  blockNumber_ = blockNumber;
}

/* Overlap accessor */
NumericalScalar WelchFactory::getOverlap() const
{
  return overlap_;
}

void WelchFactory::setOverlap(const NumericalScalar overlap)
{
  if ((overlap < 0.0) || (overlap > 0.5)) throw InvalidArgumentException(HERE) << "Error: the overlap must be in [0, 0.5], here overlap=" << overlap;
  overlap_ = overlap;
}

UserDefinedSpectralModel * WelchFactory::build(const ProcessSample & sample) const
{
  const UnsignedLong dimension(sample.getDimension());
  const UnsignedLong sampleSize(sample.getSize());
  const RegularGrid timeGrid(sample.getTimeGrid());
  const UnsignedLong N(timeGrid.getN());
  const NumericalScalar timeStep(timeGrid.getStep());
  const NumericalScalar T(timeGrid.getEnd());
  // Preprocessing: the scaling factor, including the tappering window
  NumericalComplexCollection alpha(N);
  const NumericalScalar factor(timeStep / sqrt(sampleSize * T));
  for (UnsignedLong m = 0; m < N; ++m)
    {
      // The window argument is normalized on [0, 1]
      const NumericalScalar xiM(static_cast<NumericalScalar>(m) / N);
      // Phase shift
      const NumericalScalar theta(M_PI * (N - 1) * xiM);
      alpha[m] = factor * window_(xiM) * NumericalComplex(cos(theta), sin(theta));
    }
  // The DSP estimate will be done over a regular frequency grid containing only
  // nonnegative frequency values. It is then extended as a stepwise function of
  // the frequency on positive and negative values using the hermitian symmetry
  // If N is even, kMax = N / 2, else kMax = (N + 1) / 2.
  UnsignedLong kMax(N / 2);
  const NumericalScalar frequencyStep(1.0 / T);
  NumericalScalar frequencyMin(0.5 * frequencyStep);
  // Adjust kMax and frequencyMin if N is odd
  if (N % 2 == 1)
    {
      ++kMax;
      frequencyMin = 0.0;
    }
  const RegularGrid frequencyGrid(frequencyMin, frequencyStep, kMax);
  HermitianMatrixCollection DSPCollection(kMax, HermitianMatrix(dimension));
  // Loop over the time series
  for (UnsignedLong l = 0 ; l < sampleSize; ++l)
    {
      // The current time series values are stored into a ComplexMatrix in order to
      // have a two-indices access and an internal linear storage. The data are stored column-wise, i.e the column elements are contiguous in memory
      ComplexMatrix zHat(kMax, dimension);
      // Loop over the dimension
      // For each component, compute the FFT of the time series component
      for (UnsignedLong p = 0; p < dimension; ++p)
        {
          // Loop over the time stamps
          NumericalComplexCollection zP(N);
          for (UnsignedLong m = 0; m < N; ++m) zP[m] = alpha[m] * sample[l][m][p + 1]; // The first component of the value of a time series at a given index is the time value
          // Perform the FFT direct transform of the tapered data
          const NumericalComplexCollection zPHat(fftAlgorithm_.transform(zP));
          // Stores the result. Only the values associated with nonnegative frequency values are stored.
          for (UnsignedLong k = 0; k < kMax; ++k) zHat(k, p) = zPHat[N - kMax + k];
        }
      // Now, we can estimate the spectral density over the reduced frequency grid
      for (UnsignedLong k = 0; k < kMax; ++k)
        {
          // Perform the row-wise kronecker product
          for (UnsignedLong p = 0; p < dimension; ++p)
            for (UnsignedLong q = 0; q <= p; ++q)
              DSPCollection[k](p, q) += zHat(k, p) * std::conj(zHat(k, q));
        }
    } // Loop over the time series
  return UserDefinedSpectralModel(frequencyGrid, DSPCollection).clone();
}

UserDefinedSpectralModel * WelchFactory::build(const TimeSeries & timeSeries) const
{
  // We split the time series into overlaping blockNumbers that are used as a ProcessSample
  const UnsignedLong size(timeSeries.getSize());
  const UnsignedLong dimension(timeSeries.getDimension());
  // First, compute the block size
  const UnsignedLong blockSize(static_cast<UnsignedLong>(size / (1.0 + (blockNumber_ - 1.0) * (1.0 - overlap_))));
  // Then, compute the associated hop size, even if it is not exactly associated with the overlap value
  // No hop size if only one block
  const UnsignedLong hopSize(blockNumber_ == 1 ? 0 : (size - blockSize) / (blockNumber_ - 1));
  const RegularGrid timeGrid(timeSeries.getTimeGrid());
  // Initialize the equivalent process sample with the correct time grid
  ProcessSample sample(blockNumber_, TimeSeries(RegularGrid(timeGrid.getStart(), timeGrid.getStep(), blockSize), dimension));
  for (UnsignedLong blockIndex = 0; blockIndex < blockNumber_; ++blockIndex)
    {
      for (UnsignedLong timeIndex = 0; timeIndex < blockSize; ++timeIndex)
        {
          for (UnsignedLong i = 1; i <= dimension; ++i)
            sample[blockIndex][timeIndex][i] = timeSeries[blockIndex * hopSize + timeIndex][i];
        } // Loop on the time index
    } // Loop on the blocks
  return build(sample);
}

/* Method save() stores the object through the StorageManager */
void WelchFactory::save(Advocate & adv) const
{
  SpectralModelFactoryImplementation::save(adv);
  adv.saveAttribute( "window_", window_);
  adv.saveAttribute( "blockNumber_", blockNumber_);
  adv.saveAttribute( "overlap_", overlap_);
}

/* Method load() reloads the object from the StorageManager */
void WelchFactory::load(Advocate & adv)
{
  SpectralModelFactoryImplementation::load(adv);
  adv.loadAttribute( "window_", window_);
  adv.loadAttribute( "blockNumber_", blockNumber_);
  adv.loadAttribute( "overlap_", overlap_);
}
END_NAMESPACE_OPENTURNS
