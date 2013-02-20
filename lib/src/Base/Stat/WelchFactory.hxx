//                                               -*- C++ -*-
/**
 *  @file  WelchFactory.hxx
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 *  Id      WelchFactory.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_WELCHFACTORY_HXX
#define OPENTURNS_WELCHFACTORY_HXX

#include "SpectralModelFactoryImplementation.hxx"
#include "UserDefinedSpectralModel.hxx"
#include "FilteringWindows.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class WelchFactory
 */
class WelchFactory
  : public SpectralModelFactoryImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  WelchFactory(const String & name = DefaultName);

  /** Standard constructor using a filtering windows and a frequency limit*/
  WelchFactory(const FilteringWindows & window,
               const UnsignedLong blockNumber,
               const NumericalScalar overlap = 0.5,
               const String & name = DefaultName);

  /** Virtual constructor */
  virtual WelchFactory * clone() const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

  /** FilteringWindows accessor */
  FilteringWindows getFilteringWindows() const;
  void setFilteringWindows(const FilteringWindows & window);

  /** Number of block accessor */
  UnsignedLong getBlockNumber() const;
  void setBlockNumber(const UnsignedLong blockNumber);

  /** Overlap accessor */
  NumericalScalar getOverlap() const;
  void setOverlap(const NumericalScalar overlap);

  /** Build a a spectral model based on a sample */
  UserDefinedSpectralModel * build(const ProcessSample & sample) const;

  /** Build a a spectral model based on a TimeSeries */
  UserDefinedSpectralModel * build(const TimeSeries & timeSerie) const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);

private:

  /** FilteringWindows */
  FilteringWindows window_;

  /** Size of a block */
  UnsignedLong blockNumber_;

  /** Overlap percentage for Welch method */
  NumericalScalar overlap_;

}; /* class WelchFactory */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_WELCHFACTORY_HXX */
