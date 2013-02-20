//                                               -*- C++ -*-
/**
 *  @file  CharlierFactory.cxx
 *  @brief Charlier polynomial factory
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */
#include "CharlierFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Poisson.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(CharlierFactory);

static Factory<CharlierFactory> RegisteredFactory("CharlierFactory");



/* Default constructor, associated with the default Poisson distribution which is equal to the Exponential distribution */
CharlierFactory::CharlierFactory()
  : OrthogonalUniVariatePolynomialFactory(Poisson()),
    lambda_(1.0)
{
  initializeCache();
}


/* Parameter constructor: lambda is the order of the generalized Charlier polynomial, associated with the Poisson(lambda) distribution */
CharlierFactory::CharlierFactory(const NumericalScalar lambda)
  : OrthogonalUniVariatePolynomialFactory( Poisson(lambda) ),
    lambda_(lambda)
{
  if (lambda <= 0.0) throw InvalidArgumentException(HERE) << "Error: must have lambda>0 to build Charlier polynomials.";
  initializeCache();
}

/* Virtual constructor */
CharlierFactory * CharlierFactory::clone() const
{
  return new CharlierFactory(*this);
}


/* Calculate the coefficients of recurrence a0n, a1n, a2n such that
   Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
CharlierFactory::Coefficients CharlierFactory::getRecurrenceCoefficients(const UnsignedLong n) const
{
  Coefficients recurrenceCoefficients(3, 0.0);
  if (n == 0)
    {
      const NumericalScalar factor(sqrt(lambda_));
      recurrenceCoefficients[0] = -1.0 / factor;
      recurrenceCoefficients[1] = factor;
      // Conventional value of 0.0 for recurrenceCoefficients[2]
      return recurrenceCoefficients;
    }
  const NumericalScalar factor(1.0 / sqrt((n + 1.0) * lambda_));
  recurrenceCoefficients[0] = -factor;
  recurrenceCoefficients[1] = (n + lambda_) * factor;
  recurrenceCoefficients[2] = -sqrt(1.0 - 1.0 / (n + 1.0));
  return recurrenceCoefficients;
}

/* LAMBDA accessor */
NumericalScalar CharlierFactory::getLambda() const
{
  return lambda_;
}

/* String converter */
String CharlierFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " lambda=" << lambda_
               << " measure=" << measure_;
}


/* Method save() stores the object through the StorageManager */
void CharlierFactory::save(Advocate & adv) const
{
  OrthogonalUniVariatePolynomialFactory::save(adv);
  adv.saveAttribute( "lambda_", lambda_ );
}


/* Method load() reloads the object from the StorageManager */
void CharlierFactory::load(Advocate & adv)
{
  OrthogonalUniVariatePolynomialFactory::load(adv);
  adv.loadAttribute( "lambda_", lambda_ );
}

END_NAMESPACE_OPENTURNS
