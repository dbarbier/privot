//                                               -*- C++ -*-
/**
 *  @file  RosenblattEvaluation.cxx
 *  @brief Class for the Nataf transformation evaluation for elliptical
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
 *  @author lebrun
 *  @date   2008-07-03 08:31:34 +0200 (jeu, 03 jui 2008)
 */
#include "RosenblattEvaluation.hxx"
#include "DistFunc.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RosenblattEvaluation);


static Factory<RosenblattEvaluation> RegisteredFactory("RosenblattEvaluation");

/* Default constructor */
RosenblattEvaluation::RosenblattEvaluation():
  NumericalMathEvaluationImplementation(),
  distribution_()
{
  // Nothing to do
}

/* Parameter constructor */
RosenblattEvaluation::RosenblattEvaluation(const Distribution & distribution):
  NumericalMathEvaluationImplementation(),
  distribution_(distribution)
{
  Description description(distribution.getDescription());
  const UnsignedLong size(description.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      OSS oss;
      oss << "y" << i;
      description.add(oss);
    }
  setDescription(description);
}

/* Virtual constructor */
RosenblattEvaluation * RosenblattEvaluation::clone() const
{
  return new RosenblattEvaluation(*this);
}

/* Evaluation */
NumericalPoint RosenblattEvaluation::operator () (const NumericalPoint & inP) const
{
  const UnsignedLong dimension(getOutputDimension());
  NumericalPoint result(dimension);
  NumericalPoint y(0);
  // Apply Phi^{-1} o conditional CDF over the components
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      result[i] = DistFunc::qNormal(distribution_.computeConditionalCDF(inP[i], y));
      y.add(inP[i]);
    }
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Gradient according to the marginal parameters. */
Matrix RosenblattEvaluation::parametersGradient(const NumericalPoint & inP) const
{
  throw NotYetImplementedException(HERE);
}

/* Accessor for input point dimension */
UnsignedLong RosenblattEvaluation::getInputDimension() const
{
  return distribution_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong RosenblattEvaluation::getOutputDimension() const
{
  return distribution_.getDimension();
}

/* String converter */
String RosenblattEvaluation::__repr__() const
{
  OSS oss;
  oss << "class=" << RosenblattEvaluation::GetClassName()
      << " description=" << getDescription()
      << " distribution=" << distribution_;
  return oss;
}

/* Method save() stores the object through the StorageManager */
void RosenblattEvaluation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
}

/* Method load() reloads the object from the StorageManager */
void RosenblattEvaluation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
}

END_NAMESPACE_OPENTURNS

