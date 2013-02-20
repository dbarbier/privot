//                                               -*- C++ -*-
/**
 *  @file  Axial.hxx
 *  @brief Axial experiment plane
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
 *  Id      Axial.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_AXIAL_HXX
#define OPENTURNS_AXIAL_HXX

#include "StratifiedExperiment.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class Axial
 *
 * The class describes the probabilistic concept of axial experiment plan
 */
class Axial
  : public StratifiedExperiment
{
  CLASSNAME;
public:


  /** Default constructor */
  explicit Axial(const String & name = DefaultName);

  /** Constructor with parameters */
  explicit Axial(const NumericalPoint & center,
                 const NumericalPoint & levels,
                 const String & name = DefaultName);

  /** Constructor with parameters */
  explicit Axial(const UnsignedLong dimension,
                 const NumericalPoint & levels,
                 const String & name = DefaultName);

  /** Virtual constructor */
  virtual Axial * clone() const;

  /** Experiment plane generation */
  virtual NumericalSample generate();

  /** String converter */
  virtual String __repr__() const;

}; /* class Axial */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_AXIAL_HXX */
