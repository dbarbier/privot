//                                               -*- C++ -*-
/**
 *  @file  ProductPolynomialGradientImplementation.cxx
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
#include "ProductPolynomialGradientImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ProductPolynomialGradientImplementation);

static Factory<ProductPolynomialGradientImplementation> RegisteredFactory("ProductPolynomialGradientImplementation");


/* Default constructor */
ProductPolynomialGradientImplementation::ProductPolynomialGradientImplementation()
  : NumericalMathGradientImplementation(),
    polynomials_()
{
  // Nothing to do
}


/* Constructor */
ProductPolynomialGradientImplementation::ProductPolynomialGradientImplementation(const PolynomialCollection & coll)
  : NumericalMathGradientImplementation(),
    polynomials_(coll)
{
  // Nothing to do
  Description description(0);
  for (UnsignedLong i = 0; i < getInputDimension(); ++i) description.add(OSS() << "x" << i);
  for (UnsignedLong i = 0; i < getOutputDimension(); ++i) description.add(OSS() << "y" << i);
  setDescription(description);
}


/* Virtual constructor */
ProductPolynomialGradientImplementation * ProductPolynomialGradientImplementation::clone() const
{
  return new ProductPolynomialGradientImplementation(*this);
}


/* String converter */
String ProductPolynomialGradientImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}


/* Compute the gradient of a product of univariate polynomials */
Matrix ProductPolynomialGradientImplementation::gradient (const NumericalPoint & inP) const
{
  const UnsignedLong inDimension(inP.getDimension());
  if (inDimension != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a ProductPolynomialFunction with an argument of invalid dimension";
  NumericalScalar productEvaluation(1.0);
  NumericalPoint evaluations(inDimension);
  NumericalPoint derivatives(inDimension);
  for (UnsignedLong i = 0; i < inDimension; ++i)
    {
      const NumericalScalar x(inP[i]);
      const NumericalScalar y(polynomials_[i](x));
      const NumericalScalar dy(polynomials_[i].derivative(x));
      evaluations[i] = y;
      derivatives[i] = dy;
      productEvaluation *= y;
    }
  Matrix gradient(inDimension, 1);
  // Usual case: productEvaluation <> 0
  if (productEvaluation != 0.0)
    {
      for (UnsignedLong i = 0; i < inDimension; ++i)
        {
          gradient(i, 0) = derivatives[i] * (productEvaluation / evaluations[i]);
        }
    }
  // Must compute the gradient in a more expensive way
  else
    {
      for (UnsignedLong i = 0; i < inDimension; ++i)
        {
          gradient(i, 0) = derivatives[i];
          for (UnsignedLong j = 0; j < i; ++j) gradient(i, 0) *= evaluations[j];
          for (UnsignedLong j = i + 1; j < inDimension; ++j) gradient(i, 0) *= evaluations[j];
        }
    }
  return gradient;
}


/* Accessor for input point dimension */
UnsignedLong ProductPolynomialGradientImplementation::getInputDimension() const
{
  return polynomials_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong ProductPolynomialGradientImplementation::getOutputDimension() const
{
  return 1;
}


/* Method save() stores the object through the StorageManager */
void ProductPolynomialGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute( "polynomials_", polynomials_ );
}

/* Method load() reloads the object from the StorageManager */
void ProductPolynomialGradientImplementation::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute( "polynomials_", polynomials_ );
}


END_NAMESPACE_OPENTURNS
