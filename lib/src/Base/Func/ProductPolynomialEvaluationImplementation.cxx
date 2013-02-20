//                                               -*- C++ -*-
/**
 *  @file  ProductPolynomialEvaluationImplementation.cxx
 *  @brief This is a nD polynomial build as a product of n 1D polynomial
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
#include "ProductPolynomialEvaluationImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



TEMPLATE_CLASSNAMEINIT(PersistentCollection<UniVariatePolynomial>);

static Factory<PersistentCollection<UniVariatePolynomial> > RegisteredFactory_PC_UVP("PersistentCollection<UniVariatePolynomial>");


CLASSNAMEINIT(ProductPolynomialEvaluationImplementation);

static Factory<ProductPolynomialEvaluationImplementation> RegisteredFactory_PPEI("ProductPolynomialEvaluationImplementation");


/* Default constructor */
ProductPolynomialEvaluationImplementation::ProductPolynomialEvaluationImplementation()
  : NumericalMathEvaluationImplementation(),
    polynomials_()
{
  // Nothing to do
}


/* Constructor */
ProductPolynomialEvaluationImplementation::ProductPolynomialEvaluationImplementation(const PolynomialCollection & coll)
  : NumericalMathEvaluationImplementation(),
    polynomials_(coll)
{
  // Nothing to do
  Description description(0);
  for (UnsignedLong i = 0; i < getInputDimension(); ++i) description.add(OSS() << "x" << i);
  for (UnsignedLong i = 0; i < getOutputDimension(); ++i) description.add(OSS() << "y" << i);
  setDescription(description);
}


/* Virtual constructor */
ProductPolynomialEvaluationImplementation * ProductPolynomialEvaluationImplementation::clone() const
{
  return new ProductPolynomialEvaluationImplementation(*this);
}


/* String converter */
String ProductPolynomialEvaluationImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << ", polynomials=" << polynomials_;
}

String ProductPolynomialEvaluationImplementation::__str__(const String & offset) const
{
  OSS oss(false);
  oss << offset;
  const UnsignedLong size(polynomials_.getSize());
  if (size == 0) return oss;
  const Description description(getInputDescription());
  if (size == 1) return (oss << polynomials_[0].__str__(description[0], ""));
  Bool allScalar(true);
  NumericalScalar scalarValue(1.0);
  Bool onlyOneNotScalar(false);
  UnsignedLong indexNotScalar(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const UnsignedLong degree(polynomials_[i].getDegree());
      const Bool isScalar(degree == 0);
      // Only one non-scalar so far, and the current one is not scalar
      if (onlyOneNotScalar && !isScalar) onlyOneNotScalar = false;
      // Only scalars so far, and the current is not scalar
      if (allScalar && !isScalar)
        {
          onlyOneNotScalar = true;
          indexNotScalar = i;
        }
      if (isScalar) scalarValue *= polynomials_[i].getCoefficients()[0];
      allScalar = allScalar && isScalar;
    }
  // Scalar polynomial
  if (allScalar) oss << scalarValue;
  // Only one no unit polynomial in the product
  else if (onlyOneNotScalar) oss << (polynomials_[indexNotScalar] * scalarValue).__str__(description[indexNotScalar], "");
  // At least two non-scalar factors
  else
    {
      const String scalarValueString(OSS(false) << scalarValue);
      Bool first(scalarValueString == "1");
      // There is a non-unit factor
      if (!first) oss << scalarValue;
      for (UnsignedLong i = 0; i < size; ++i)
        {
          const UnsignedLong degree(polynomials_[i].getDegree());
          // All the degree 0 factors have already been taken into account
          if (degree > 0)
            {
              if (!first) oss << " * ";
              oss << "(" << polynomials_[i].__str__(description[i], "") << ")";
              first = false;
            }
        } // Loop over the factors
    } // At least two non-scalar factors
  return oss;
}


/* Operator (): Evaluate a product of 1D polynomials for one sample */
NumericalPoint ProductPolynomialEvaluationImplementation::operator() (const NumericalPoint & inP) const
{
  const UnsignedLong inDimension(inP.getDimension());
  if (inDimension != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a ProductPolynomialFunction with an argument of invalid dimension";
  NumericalScalar productEvaluation(1.0) ;
  for (UnsignedLong i = 0; i < inDimension; ++i) productEvaluation *= polynomials_[i](inP[i]);
  const NumericalPoint result(1, productEvaluation);
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong ProductPolynomialEvaluationImplementation::getInputDimension() const
{
  return polynomials_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong ProductPolynomialEvaluationImplementation::getOutputDimension() const
{
  return 1;
}


/* Method save() stores the object through the StorageManager */
void ProductPolynomialEvaluationImplementation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "polynomials_", polynomials_ );
}

/* Method load() reloads the object from the StorageManager */
void ProductPolynomialEvaluationImplementation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "polynomials_", polynomials_ );
}


END_NAMESPACE_OPENTURNS
