//                                               -*- C++ -*-
/**
 *  @file  FFT.hxx
 *  @brief  This class is enables to launch an FFT transformation /inverse transformation
 *  This is the interface class
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
 *  Id      FFT.hxx 2491 2012-04-18 15:56:46Z schueller
 */
#ifndef OPENTURNS_FFT_HXX
#define OPENTURNS_FFT_HXX

#include "PersistentObject.hxx"
#include "FFTImplementation.hxx"
#include "TypedInterfaceObject.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class FFT
 */

class FFT
  : public TypedInterfaceObject<FFTImplementation>
{

  CLASSNAME;

public:

  typedef Pointer<FFTImplementation>    Implementation;
  typedef Collection<NumericalScalar>   NumericalScalarCollection;
  typedef Collection<NumericalComplex>  NumericalComplexCollection;

  /** Default onstructor */
  FFT();

  /** Copy constructors */
  FFT(const FFTImplementation & implementation,
      const String & name = DefaultName);

  /** Constructor from implementation */
  FFT(const Implementation & p_implementation,
      const String & name = DefaultName);

#ifndef SWIG
  /** Constructor from implementation pointer */
  FFT(FFTImplementation * p_implementation,
      const String & name = DefaultName);
#endif

  /** FFT transformation on real
   * Given the real sequence X_n, compute the sequence Z_k such that:
   * Z_k = \sum_{n=0}^{N-1} X_n\exp(-\frac{2i\pi kn}{N})
   */
  NumericalComplexCollection transform(const NumericalScalarCollection & collection) const;

  /** FFT transformation on complex
   * Given the complex sequence Y_n, compute the sequence Z_k such that:
   * Z_k = \sum_{n=0}^{N-1} Y_n\exp(-\frac{2i\pi kn}{N})
   */
  NumericalComplexCollection transform(const NumericalComplexCollection & collection) const;

  /** FFT transformation on complex - For some FFT implementation, the transformation is applied on a part of the collection */
  NumericalComplexCollection transform(const NumericalComplexCollection & collection,
                                       const UnsignedLong first,
                                       const UnsignedLong size) const;

  /** FFT transformation on complex - For some FFT implementation, the need is to transform a regular sequence of the collection (between first and last, by step = step)*/
  NumericalComplexCollection transform(const NumericalComplexCollection & collection,
                                       const UnsignedLong first,
                                       const UnsignedLong step,
                                       const UnsignedLong last) const;

  /** FFT inverse transformation
   * Given the complex sequence Z_n, compute the sequence Y_k such that:
   * Y_k = \frac{1}{N}\sum_{n=0}^{N-1} Z_n\exp(\frac{2i\pi kn}{N})
   */
  NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection) const;

  /** FFT inverse transformation - The transformation is applied on a part of the collection */
  NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection,
                                              const UnsignedLong first,
                                              const UnsignedLong size) const;

  /** FFT inverse transformation on a regular sequence of the collection (between first and last, by step = step)*/
  NumericalComplexCollection inverseTransform(const NumericalComplexCollection & collection,
                                              const UnsignedLong first,
                                              const UnsignedLong step,
                                              const UnsignedLong last) const;

  /** String converter */
  String __repr__() const;

  /** String converter */
  String __str__(const String & offset = "") const;


} ; /* class FFT */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_FFT_HXX */
