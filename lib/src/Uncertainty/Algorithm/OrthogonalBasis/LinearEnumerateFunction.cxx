//                                               -*- C++ -*-
/**
 *  @file  LinearEnumerateFunction.cxx
 *  @brief The bijective function to select polynomials in the orthogonal basis
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
#include "LinearEnumerateFunction.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(LinearEnumerateFunction);

static Factory<LinearEnumerateFunction> RegisteredFactory("LinearEnumerateFunction");



/* Default constructor */
LinearEnumerateFunction::LinearEnumerateFunction()
  : EnumerateFunctionImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
LinearEnumerateFunction::LinearEnumerateFunction(const UnsignedLong dimension)
  : EnumerateFunctionImplementation(dimension)
{
  // Nothing to do
}


/* Virtual constrcutor */
LinearEnumerateFunction * LinearEnumerateFunction::clone() const
{
  return new LinearEnumerateFunction(*this);
}


/* String converter */
String LinearEnumerateFunction::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " dimension=" << getDimension();
}


/* Find the smallest n such that Binomial(n, n + dimension) > index and also returns the value of Binomial(n - 1, n - 1 + dimension) */
UnsignedLong LinearEnumerateFunction::findBinomialCoefficient(const UnsignedLong index,
                                                              const UnsignedLong dimension,
                                                              UnsignedLong & binomialCoefficient) const
{
  UnsignedLong n(0);
  binomialCoefficient = 1;
  // Use floating point arithmetic to avoid overflow
  NumericalScalar newBinomialCoefficient(1.0);
  // Special treatment to avoid roundoff error during conversion
  while(static_cast<UnsignedLong>(round(newBinomialCoefficient)) <= index)
    {
      ++n;
      binomialCoefficient = static_cast<UnsignedLong>(round(newBinomialCoefficient));
      // Using integer arithmetic, the computation should have been implemented this way to avoid truncation:
      // newBinomialCoefficient = (newBinomialCoefficient * (n + dimension)) / n;
      // but for large n it should lead to an overflow.
      // Instead, we use floating point arithmetic
      newBinomialCoefficient *= 1.0 + static_cast<NumericalScalar>(dimension) / static_cast<NumericalScalar>(n);
    }
  return n;
}

/* The bijective association between an integer and a set of indices.
   First, we look for the total degree of the associated polynom. Then, we
   compute recursively the complement by looping over the degree of the remainder.
   For a given index I, we have:
   I = Binomial(n_1, d) + ... + Binomial(n_{d-1}, 1)
   where Binomial(n_1, d_1) is
*/
Indices LinearEnumerateFunction::operator() (const UnsignedLong index) const
{
  const UnsignedLong dimension(getDimension());
  Indices result(dimension, 0);
  if (index == 0) return result;
  UnsignedLong binomialCoefficient(0);
  UnsignedLong degree(findBinomialCoefficient(index, dimension, binomialCoefficient));
  // Loop over the dimension of the remaining polynomial
  UnsignedLong currentIndex(index);
  for (UnsignedLong i = 0; i < dimension - 1; ++i)
    {
      // Early exit if the remaining polynomial is constant
      if (currentIndex <= binomialCoefficient)
        {
          result[i] = degree;
          return result;
        }
      currentIndex -= binomialCoefficient;
      const UnsignedLong remainingDegree(findBinomialCoefficient(currentIndex, dimension - i - 1, binomialCoefficient));
      result[i] = degree - remainingDegree;
      degree = remainingDegree;
    }
  result[dimension - 1] = degree;
  return result;
}

/* The inverse of the association */
UnsignedLong LinearEnumerateFunction::inverse(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  const UnsignedLong size(indices.getSize());
  if (size != dimension) throw InvalidArgumentException(HERE)  << "Error: the size of the given indices must match the dimension.";
  // Quick return for dimension == 1 case
  if (size == 1) return indices[0];
  UnsignedLong totalDegree(0);
  for (UnsignedLong i = 0; i < size; ++i) totalDegree += indices[i];
  UnsignedLong result(0);
  // Loop over the marginal degrees
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // Early return if nothing is left
      if (totalDegree == 0) return result;
      result += LinearEnumerateFunction(dimension - i).getStrataCumulatedCardinal(totalDegree - 1);
      totalDegree -= indices[i];
    }
  return result;
}


/* The cardinal of the given strata
 * = C(strataIndex, dimension - 1 + strataIndex)
 * = (dimension - 1 + strataIndex) ! / (dimension - 1)!.strataIndex!)
 */
UnsignedLong LinearEnumerateFunction::getStrataCardinal(const UnsignedLong strataIndex) const
{
  const UnsignedLong dimension(getDimension());
  return static_cast<UnsignedLong>(round(exp(SpecFunc::LnGamma(dimension + strataIndex) - SpecFunc::LnGamma(dimension) - SpecFunc::LnGamma(strataIndex + 1))));
}

/* The cardinal of the cumulated strata less or equal to the given strata
 * = C(strataIndex, dimension + strataIndex)
 * = (dimension + strataIndex)! / (dimension!.strataIndex!)
 */
UnsignedLong LinearEnumerateFunction::getStrataCumulatedCardinal(const UnsignedLong strataIndex) const
{
  const UnsignedLong dimension(getDimension());
  return static_cast<UnsignedLong>(round(exp(SpecFunc::LnGamma(dimension + strataIndex + 1) - SpecFunc::LnGamma(dimension + 1) - SpecFunc::LnGamma(strataIndex + 1))));
}


/* Method save() stores the object through the StorageManager */
void LinearEnumerateFunction::save(Advocate & adv) const
{
  EnumerateFunctionImplementation::save(adv);
}


/* Method load() reloads the object from the StorageManager */
void LinearEnumerateFunction::load(Advocate & adv)
{
  EnumerateFunctionImplementation::load(adv);
}


END_NAMESPACE_OPENTURNS
