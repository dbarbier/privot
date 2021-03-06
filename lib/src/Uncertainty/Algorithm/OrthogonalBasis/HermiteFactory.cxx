//                                               -*- C++ -*-
/**
 *  @file  HermiteFactory.cxx
 *  @brief Hermite polynomial factory
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "HermiteFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Normal.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(HermiteFactory);

static Factory<HermiteFactory> RegisteredFactory("HermiteFactory");


/* Default constructor */
HermiteFactory::HermiteFactory()
  : OrthogonalUniVariatePolynomialFactory(Normal())
{
  initializeCache();
}


/* Virtual constructor */
HermiteFactory * HermiteFactory::clone() const
{
  return new HermiteFactory(*this);
}


/* Calculate the coefficients of recurrence a0n, a1n, a2n such that
   Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
HermiteFactory::Coefficients HermiteFactory::getRecurrenceCoefficients(const UnsignedLong n) const
{
  Coefficients recurrenceCoefficients(3, 0.0);
  if (n == 0)
    {
      recurrenceCoefficients[0] = 1.0;
      recurrenceCoefficients[1] = 0.0;
      // Conventional value of 0.0 for recurrenceCoefficients[2]
      return recurrenceCoefficients;
    }
  recurrenceCoefficients[0] = 1.0 / sqrt(n + 1);
  recurrenceCoefficients[1] = 0.0;
  recurrenceCoefficients[2] = -sqrt(1.0 - 1.0 / (n + 1));
  return recurrenceCoefficients;
}


/* String converter */
String HermiteFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " measure=" << measure_;
}


/* Method save() stores the object through the StorageManager */
void HermiteFactory::save(Advocate & adv) const
{
  OrthogonalUniVariatePolynomialFactory::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void HermiteFactory::load(Advocate & adv)
{
  OrthogonalUniVariatePolynomialFactory::load(adv);
}

END_NAMESPACE_OPENTURNS
