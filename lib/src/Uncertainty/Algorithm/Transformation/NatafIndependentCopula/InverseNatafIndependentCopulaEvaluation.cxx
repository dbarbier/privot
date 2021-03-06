//                                               -*- C++ -*-
/**
 *  @file  InverseNatafIndependentCopulaEvaluation.cxx
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
#include "InverseNatafIndependentCopulaEvaluation.hxx"
#include "DistFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class InverseNatafIndependentCopulaEvaluation
 *
 * This class offers an interface for the InverseNataf function for independent copula
 */


CLASSNAMEINIT(InverseNatafIndependentCopulaEvaluation);

/* Parameter constructor */
InverseNatafIndependentCopulaEvaluation::InverseNatafIndependentCopulaEvaluation(const UnsignedLong dimension):
  NumericalMathEvaluationImplementation(),
  dimension_(dimension)
{
  Description description;
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      OSS oss;
      oss << "x" << i;
      description.add(oss);
    }
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      OSS oss;
      oss << "y" << i;
      description.add(oss);
    }
  setDescription(description);
}

/* Virtual constructor */
InverseNatafIndependentCopulaEvaluation * InverseNatafIndependentCopulaEvaluation::clone() const
{
  return new InverseNatafIndependentCopulaEvaluation(*this);
}

/* String converter */
String InverseNatafIndependentCopulaEvaluation::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseNatafIndependentCopulaEvaluation::GetClassName()
      << " description=" << getDescription()
      << " dimension=" << dimension_;

  return oss;
}

/*
 * Evaluation
 * The inverse Nataf transform S reads:
 * Si(u) = Phi(ui), where Phi is the CDF of the standard normal distribution
 */
NumericalPoint InverseNatafIndependentCopulaEvaluation::operator () (const NumericalPoint & inP) const
{
  NumericalPoint result(dimension_);
  for (UnsignedLong i = 0; i < dimension_; ++i)
    {
      result[i] = DistFunc::pNormal(inP[i]);
    }
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Gradient according to the marginal parameters. Currently, the dependence parameter are not taken into account. */

Matrix InverseNatafIndependentCopulaEvaluation::parametersGradient(const NumericalPoint & inP) const
{
  return Matrix(0, dimension_);
}

/* Accessor for input point dimension */
UnsignedLong InverseNatafIndependentCopulaEvaluation::getInputDimension() const
{
  return dimension_;
}

/* Accessor for output point dimension */
UnsignedLong InverseNatafIndependentCopulaEvaluation::getOutputDimension() const
{
  return dimension_;
}

END_NAMESPACE_OPENTURNS
