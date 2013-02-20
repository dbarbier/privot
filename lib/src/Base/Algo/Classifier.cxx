//                                               -*- C++ -*-
/**
 *  @file  Classifier.cxx
 *  @brief Classification algorithm interface
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "Classifier.hxx"
#include "ClassifierImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Classifier);

/* Constructor from implementation */
Classifier::Classifier()
  : TypedInterfaceObject<ClassifierImplementation>( new ClassifierImplementation() )
{
  // Nothing to do
}

/* Constructor from implementation */
Classifier::Classifier(const ClassifierImplementation & implementation)
  : TypedInterfaceObject<ClassifierImplementation>( implementation.clone() )
{
  // Nothing to do
}

/* Constructor from implementation */
Classifier::Classifier(const Implementation & p_implementation)
  : TypedInterfaceObject<ClassifierImplementation>( p_implementation )
{
  // Nothing to do
}

/* Constructor from implementation */
Classifier::Classifier(const NumericalSample & inS, const Indices & outC)
  : TypedInterfaceObject<ClassifierImplementation>( new ClassifierImplementation(inS, outC) )
{
  // Nothing to do
}

/* Classify a point */
UnsignedLong Classifier::classify(const NumericalPoint & inP) const
{
  return getImplementation()->classify(inP);
}

/* Classify a sample */
Indices Classifier::classify(const NumericalSample & inS) const
{
  return getImplementation()->classify(inS);
}

/* Grade a point */
NumericalScalar Classifier::grade(const NumericalPoint & inP, UnsignedLong hClass) const
{
  return getImplementation()->grade(inP, hClass);
}

/* Grade a sample */
NumericalPoint Classifier::grade(const NumericalSample & inS, const Indices & hClass) const
{
  return getImplementation()->grade(inS, hClass);
}

/* Verbosity accessor */
void Classifier::setVerbose(const Bool verbose)
{
  copyOnWrite();
  getImplementation()->setVerbose(verbose);
}

Bool Classifier::getVerbose() const
{
  return getImplementation()->getVerbose();
}

/* Input space dimension */
UnsignedLong Classifier::getDimension() const
{
  return getImplementation()->getDimension();
}

/* String converter */
String Classifier::__repr__() const
{
  return getImplementation()->__repr__();
}

String Classifier::__str__(const String & offset) const
{
  return getImplementation()->__str__( offset );
}


END_NAMESPACE_OPENTURNS
