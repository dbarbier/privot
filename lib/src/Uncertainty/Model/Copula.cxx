//                                               -*- C++ -*-
/**
 *  @file  Copula.cxx
 *  @brief Abstract top-level class for all copulas
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
#include "Copula.hxx"
#include "IndependentCopula.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Copula);


/* Default constructor */
Copula::Copula()
  : Distribution(IndependentCopula(1))
{
  // Nothing to do
}

/* Constructor from distribution */
Copula::Copula(const Distribution & distribution)
  : Distribution(distribution)
{
  // Nothing to do
}

/* Constructor from implementation */
Copula::Copula(const DistributionImplementation & implementation,
               const String & name)
  : Distribution(implementation, name)
{
  // Nothing to do
}

/* Constructor from implementation */
Copula::Copula(const Implementation & p_implementation,
               const String & name)
  : Distribution(p_implementation, name)
{
  // Nothing to do
}

/* Constructor from implementation */
Copula::Copula(DistributionImplementation * p_implementation,
               const String & name)
  : Distribution(p_implementation, name)
{
  // Nothing to do
}

/* String converter */
String Copula::__repr__() const
{
  return Distribution::__repr__();
}

/* String converter */
String Copula::__str__(const String & offset) const
{
  return Distribution::__str__(offset);
}


END_NAMESPACE_OPENTURNS
