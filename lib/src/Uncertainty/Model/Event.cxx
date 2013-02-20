//                                               -*- C++ -*-
/**
 *  @file  Event.cxx
 *  @brief The class that implements all random vectors
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
 *  @date   2012-02-17 19:35:43 +0100 (Fri, 17 Feb 2012)
 */

#include "Event.hxx"
#include "CompositeRandomVector.hxx"
#include "Less.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Event);

/* Default constructor for save/load */
Event::Event()
  : RandomVector(CompositeRandomVector(NumericalMathFunction("x", "x"), RandomVector(NumericalPoint(1, 0.0))), Less(), 0.0)
{
  // Nothing to do
}

/* Constructor from RandomVector */
Event::Event(const RandomVector & antecedent,
             const ComparisonOperator & op,
             const NumericalScalar threshold,
             const String & name)
  : RandomVector(antecedent, op, threshold, name)
{
  // Nothing to do
}

/* Constructor from RandomVector */
Event::Event(const RandomVector & antecedent,
             const Domain & domain,
             const String & name)
  : RandomVector(antecedent, domain, name)
{
  // Nothing to do
}


/* Constructor from RandomVector */
Event::Event(const Process & process,
             const Domain & domain,
             const String & name)
  : RandomVector(process, domain, name)
{
  // Nothing to do
}

/* String converter */
String Event::__repr__() const
{
  OSS oss;
  oss << "class=" << Event::GetClassName()
      << " name=" << getName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}

/* String converter */
String Event::__str__(const String & offset) const
{
  return __repr__();
}

END_NAMESPACE_OPENTURNS
