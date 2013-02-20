//                                               -*- C++ -*-
/**
 *  @file  FFTImplementation.cxx
 *  @brief
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
 *  @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */
#include "FFT.hxx"
#include "PersistentObjectFactory.hxx"
#include "KissFFT.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FFT);

/* Constructor with parameters */
FFT::FFT()
  : TypedInterfaceObject<FFTImplementation>(new KissFFT())
{
  // Nothing to do
}

/* Parameters constructor */
FFT::FFT(const FFTImplementation & implementation,
         const String & name)
  : TypedInterfaceObject<FFTImplementation>(implementation.clone())
{
  getImplementation()->setName(name);
}

/* Constructor from implementation */
FFT::FFT(const Implementation & p_implementation,
         const String & name)
  : TypedInterfaceObject<FFTImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}

/* Constructor from implementation pointer */
FFT::FFT(FFTImplementation * p_implementation,
         const String & name)
  : TypedInterfaceObject<FFTImplementation>(p_implementation)
{
  getImplementation()->setName(name);
}


/* FFT transformation on real */
FFT::NumericalComplexCollection FFT::transform(const NumericalScalarCollection & collection) const
{
  return getImplementation()->transform(collection);
}

/* FFT transformation on complex */
FFT::NumericalComplexCollection FFT::transform(const NumericalComplexCollection & collection) const
{
  return getImplementation()->transform(collection);
}

/* FFT transformation on complex. The transformation is applied on a part of the collection */
FFT::NumericalComplexCollection FFT::transform(const NumericalComplexCollection & collection,
                                               const UnsignedLong first,
                                               const UnsignedLong size) const
{
  return getImplementation()->transform(collection, first, size);
}


/* FFT transformation on complex. This transforms a regular sequence of the collection (between first and last, with elements separated by step)*/
FFT::NumericalComplexCollection FFT::transform(const NumericalComplexCollection & collection,
                                               const UnsignedLong first,
                                               const UnsignedLong step,
                                               const UnsignedLong last) const
{
  return getImplementation()->transform(collection, first, step, last);
}


/* FFT inverse transformation */
FFT::NumericalComplexCollection FFT::inverseTransform(const NumericalComplexCollection & collection) const
{
  return getImplementation()->inverseTransform(collection);
}

/* FFT transformation on complex. The transformation is applied on a part of the collection */
FFT::NumericalComplexCollection FFT::inverseTransform(const NumericalComplexCollection & collection,
                                                      const UnsignedLong first,
                                                      const UnsignedLong size) const
{
  return getImplementation()->inverseTransform(collection, first, size);
}


/* FFT transformation on complex. This transforms a regular sequence of the collection (between first and last, with elements separated by step)*/
FFT::NumericalComplexCollection FFT::inverseTransform(const NumericalComplexCollection & collection,
                                                      const UnsignedLong first,
                                                      const UnsignedLong step,
                                                      const UnsignedLong last) const
{
  return getImplementation()->inverseTransform(collection, first, step, last);
}


/* String converter */
String FFT::__repr__() const
{
  OSS oss;
  oss << "class=" << FFT::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String FFT::__str__(const String & offset) const
{
  OSS oss;
  oss << "class=" << FFT::GetClassName()
      << " implementation=" << getImplementation()->__str__(offset);
  return oss;
}

END_NAMESPACE_OPENTURNS
