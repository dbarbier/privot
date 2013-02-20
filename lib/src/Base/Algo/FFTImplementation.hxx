//                                               -*- C++ -*-
/**
 *  @file  FFTImplementation.hxx
 *  @brief This class is enables to launch an FFT transform/inverse transform
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
 *  Id      FFTImplementation.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FFTIMPLEMENTATION_HXX
#define OPENTURNS_FFTIMPLEMENTATION_HXX

#include "PersistentObject.hxx"
#include "Collection.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FFTImplementation
 */

class FFTImplementation
  : public PersistentObject
{

  CLASSNAME;

public:

  typedef Collection<NumericalScalar>   NumericalScalarCollection;
  typedef Collection<NumericalComplex>  NumericalComplexCollection;

  /** Default constructor without parameters */
  FFTImplementation(const String & name = DefaultName);

  /** Virtual copy constructor */
  virtual FFTImplementation * clone() const;

  /** FFT transformation on real */
  virtual NumericalComplexCollection transform(const NumericalScalarCollection & collection) const;

  /** FFT transformation on complex */
  virtual NumericalComplexCollection transform(const NumericalComplexCollection & collection) const;

  /** FFT transformation on complex - For some FFT implementation, the transformation is applied on a part of the collection */
  virtual NumericalComplexCollection transform(const NumericalComplexCollection & collection,
                                               const UnsignedLong first,
                                               const UnsignedLong size) const;

  /** FFT transformation on complex - For some FFT implementation, the need is to transform a regular sequence of the collection (between first and last, by step = step)*/
  virtual NumericalComplexCollection transform(const NumericalComplexCollection & collection,
                                               const UnsignedLong first,
                                               const UnsignedLong step,
                                               const UnsignedLong last) const;

  /** FFT inverse transformation */
  virtual NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection) const;

  /** FFT inverse transformation - The transformation is applied on a part of the collection */
  virtual NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection,
                                                      const UnsignedLong first,
                                                      const UnsignedLong size) const;

  /** FFT transformation on a regular sequence of the collection (between first and last, spearated  by step)*/
  virtual NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection,
                                                      const UnsignedLong first,
                                                      const UnsignedLong step,
                                                      const UnsignedLong last) const;

  /** String converter */
  virtual String __repr__() const;

  /** String converter */
  virtual String __str__(const String & offset = "") const;

} ; /* class FFTImplementation */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FFTIMPLEMENTATION_HXX */
