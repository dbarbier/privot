//                                               -*- C++ -*-
/**
 *  @file  InverseNatafIndependentCopulaHessian.cxx
 *  @brief Class for the InverseNataf transformation evaluation for independent
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
#include <cmath>
#include "InverseNatafIndependentCopulaHessian.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class InverseNatafIndependentCopulaHessian
 *
 * This class offers an interface for the InverseNataf function for independent copula
 */

CLASSNAMEINIT(InverseNatafIndependentCopulaHessian);

static Factory<InverseNatafIndependentCopulaHessian> RegisteredFactory("InverseNatafIndependentCopulaHessian");

/* Default constructor */
InverseNatafIndependentCopulaHessian::InverseNatafIndependentCopulaHessian():
  NumericalMathHessianImplementation(),
  dimension_()
{
  // Nothing to do
}

/* Parameter constructor */
InverseNatafIndependentCopulaHessian::InverseNatafIndependentCopulaHessian(const UnsignedLong dimension):
  NumericalMathHessianImplementation(),
  dimension_(dimension)
{
  // Nothing to do
}

/* Virtual constructor */
InverseNatafIndependentCopulaHessian * InverseNatafIndependentCopulaHessian::clone() const
{
  return new InverseNatafIndependentCopulaHessian(*this);
}

/* String converter */
String InverseNatafIndependentCopulaHessian::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseNatafIndependentCopulaHessian::GetClassName()
      << " dimension=" << dimension_;

  return oss;
}

/*
 * Evaluation
 * The inverse Nataf transform S reads:
 * Si(u) = Phi(ui), where Phi is the CDF of the standard normal distribution
 * Hijk = d2Si/dujduk = Phi''(ui) if i = j = k
 *                    = 0 else
 */
SymmetricTensor InverseNatafIndependentCopulaHessian::hessian(const NumericalPoint & inP) const
{
  SymmetricTensor result(dimension_, dimension_);
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      NumericalScalar x(inP[i]);
      // 0.3989422804014326779399462 = 1/sqrt(2*Pi)
      result(i, i, i) = -0.3989422804014326779399462 * x * exp(-0.5 * x * x);
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong InverseNatafIndependentCopulaHessian::getInputDimension() const
{
  return dimension_;
}

/* Accessor for output point dimension */
UnsignedLong InverseNatafIndependentCopulaHessian::getOutputDimension() const
{
  return dimension_;
}

/* Method save() stores the object through the StorageManager */
void InverseNatafIndependentCopulaHessian::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "dimension_", dimension_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseNatafIndependentCopulaHessian::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "dimension_", dimension_ );
}

END_NAMESPACE_OPENTURNS
