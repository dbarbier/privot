//                                               -*- C++ -*-
/**
 *  @file  KissFFT.hxx
 *  @brief This class is enables to build an exponential covariance
 *  model, a second order model's implementation
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
 *  Id      KissFFT.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_KISSFFT_HXX
#define OPENTURNS_KISSFFT_HXX

#include "PersistentObject.hxx"
#include "FFTImplementation.hxx"


BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KissFFT
 */

class KissFFT
  : public FFTImplementation
{

  CLASSNAME;

public:


  /** Default constructor without parameters */
  KissFFT(const String & name = DefaultName);

  /** Virtual copy constructor */
  KissFFT * clone() const;

  /** FFT transformation on complex */
  NumericalComplexCollection transform(const NumericalComplexCollection & collection) const;

  /** FFT transformation  with application on a block-part of the collection */
  NumericalComplexCollection transform(const NumericalComplexCollection & collection,
                                       const UnsignedLong first,
                                       const UnsignedLong size) const;

  /** FFT inverse transformation */
  NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection) const;

  /** FFT inverse transformation with application on a block-part of the collection */
  NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection,
                                              const UnsignedLong first,
                                              const UnsignedLong size) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;

} ; /* class KissFFT */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_KISSFFT_HXX */
