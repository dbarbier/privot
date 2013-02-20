//                                               -*- C++ -*-
/**
 *  @file  ProductPolynomialHessianImplementation.cxx
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
#include "ProductPolynomialHessianImplementation.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(ProductPolynomialHessianImplementation);

static Factory<ProductPolynomialHessianImplementation> RegisteredFactory("ProductPolynomialHessianImplementation");


/* Default constructor */
ProductPolynomialHessianImplementation::ProductPolynomialHessianImplementation()
  : NumericalMathHessianImplementation(),
    polynomials_()
{
  // Nothing to do
}


/* Constructor */
ProductPolynomialHessianImplementation::ProductPolynomialHessianImplementation(const PolynomialCollection & coll)
  : NumericalMathHessianImplementation(),
    polynomials_(coll)
{
  // Nothing to do
  Description description(0);
  for (UnsignedLong i = 0; i < getInputDimension(); ++i) description.add(OSS() << "x" << i);
  for (UnsignedLong i = 0; i < getOutputDimension(); ++i) description.add(OSS() << "y" << i);
  setDescription(description);
}


/* Virtual constructor */
ProductPolynomialHessianImplementation * ProductPolynomialHessianImplementation::clone() const
{
  return new ProductPolynomialHessianImplementation(*this);
}


/* String converter */
String ProductPolynomialHessianImplementation::__repr__() const
{
  return OSS() << "class=" << GetClassName();
}


/* Compute the hessian of a product of univariate polynomials */
SymmetricTensor ProductPolynomialHessianImplementation::hessian (const NumericalPoint & inP) const
{
  const UnsignedLong inDimension(inP.getDimension());
  if (inDimension != getInputDimension()) throw InvalidArgumentException(HERE) << "Error: trying to evaluate a ProductPolynomialFunction with an argument of invalid dimension";
  NumericalScalar productEvaluation(1.0);
  NumericalPoint evaluations(inDimension);
  NumericalPoint derivatives(inDimension);
  NumericalPoint secondDerivatives(inDimension);
  for (UnsignedLong i = 0; i < inDimension; ++i)
    {
      const NumericalScalar x(inP[i]);
      const NumericalScalar y(polynomials_[i](x));
      const NumericalScalar dy(polynomials_[i].derivative(x));
      const NumericalScalar d2y(polynomials_[i].derivate().derivative(x));
      evaluations[i] = y;
      derivatives[i] = dy;
      secondDerivatives[i] = d2y;
      productEvaluation *= y;
    }
  SymmetricTensor hessian(inDimension, 1);
  // Usual case: productEvaluation <> 0
  if (productEvaluation != 0.0)
    {
      for (UnsignedLong i = 0; i < inDimension; ++i)
        {
          const NumericalScalar dyi(derivatives[i] * (productEvaluation / evaluations[i]));
          for (UnsignedLong j = 0; j < i; ++j)
            {
              hessian(i, j, 0) = derivatives[j] * (dyi / evaluations[j]);
            }
          hessian(i, i, 0) = secondDerivatives[i] * (productEvaluation / evaluations[i]);
        }
    }
  // Must compute the hessian in a more expensive way
  else
    {
      for (UnsignedLong i = 0; i < inDimension; ++i)
        {
          for (UnsignedLong j = 0; j < i; ++j)
            {
              hessian(i, j, 0) = derivatives[i] * derivatives[j];
              for (UnsignedLong k = 0; k < j; ++k) hessian(i, j, 0) *= evaluations[k];
              for (UnsignedLong k = j + 1; k < i; ++k) hessian(i, j, 0) *= evaluations[k];
              for (UnsignedLong k = i + 1; k < inDimension; ++k) hessian(i, j, 0) *= evaluations[k];
            }
          hessian(i, i, 0) = secondDerivatives[i];
          for (UnsignedLong k = 0; k < i; ++k) hessian(i, i, 0) *= evaluations[k];
          for (UnsignedLong k = i + 1; k < inDimension; ++k) hessian(i, i, 0) *= evaluations[k];
        }
    }
  return hessian;
}

/* Accessor for input point dimension */
UnsignedLong ProductPolynomialHessianImplementation::getInputDimension() const
{
  return polynomials_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong ProductPolynomialHessianImplementation::getOutputDimension() const
{
  return 1;
}


/* Method save() stores the object through the StorageManager */
void ProductPolynomialHessianImplementation::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "polynomials_", polynomials_ );
}

/* Method load() reloads the object from the StorageManager */
void ProductPolynomialHessianImplementation::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "polynomials_", polynomials_ );
}


END_NAMESPACE_OPENTURNS
