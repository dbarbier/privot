//                                               -*- C++ -*-
/**
 *  @file  StratifiedExperiment.hxx
 *  @brief Abstract top-level view of an stratifiedExperiment plane
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
 *  @author dutka
 *  @date   2008-06-26 13:50:17 +0200 (jeu, 26 jun 2008)
 *  Id      StratifiedExperiment.hxx 862 2008-06-26 11:50:17Z dutka
 */
#ifndef OPENTURNS_STRATIFIEDEXPERIMENT_HXX
#define OPENTURNS_STRATIFIEDEXPERIMENT_HXX

#include "ExperimentImplementation.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class StratifiedExperiment
 *
 * The class describes the probabilistic concept of stratifiedExperiment plan
 */
class StratifiedExperiment
  : public ExperimentImplementation
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit StratifiedExperiment(const String & name = DefaultName);

  /** Constructor with parameters */
  explicit StratifiedExperiment(const NumericalPoint & center,
                                const NumericalPoint & levels,
                                const String & name = DefaultName);

  /** String converter */
  virtual String __repr__() const;

  /** Center accessor */
  virtual void setCenter(const NumericalPoint & center);

  /** Center accessor */
  virtual NumericalPoint getCenter() const;

  /** Levels accessor */
  virtual void setLevels(const NumericalPoint & levels);

  /** Levels accessor */
  virtual NumericalPoint getLevels() const;

protected:

  NumericalPoint center_;
  NumericalPoint levels_;

}; /* class StratifiedExperiment */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_STRATIFIEDEXPERIMENT_HXX */
