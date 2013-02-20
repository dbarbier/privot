//                                             -*- C++ -*-
/**
 *  @file  BlendedStep.cxx
 *  @brief Implementation of BlendedStep
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
 *  @author slama
 *  @date   2008-11-19 16:10:42 +0100 (mer, 19 nov 2008)
 */

#include "BlendedStep.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(BlendedStep);


static Factory<BlendedStep> RegisteredFactory("BlendedStep");


/* Default constructor */
BlendedStep::BlendedStep()
  : FiniteDifferenceStepImplementation()
{
  // Nothing to do
}

/* Parameters constructor */
BlendedStep::BlendedStep(const NumericalPoint & epsilon, const NumericalScalar eta)
  : FiniteDifferenceStepImplementation(epsilon)
{
  setEta( NumericalPoint( epsilon.getDimension(), eta ) );
}

/* Parameters constructor */
BlendedStep::BlendedStep(const NumericalPoint & epsilon, const NumericalPoint & eta)
  : FiniteDifferenceStepImplementation(epsilon)
{
  setEta( eta );
}

/* String converter */
String BlendedStep::__repr__() const
{
  OSS oss;
  oss << "class=" << BlendedStep::GetClassName()
      << " derived from " << FiniteDifferenceStepImplementation::__repr__()
      << " eta=" << eta_;
  return oss;
}

/* Virtual constructor */
BlendedStep * BlendedStep::clone() const
{
  return new BlendedStep(*this);
}

/* Compute step */
NumericalPoint BlendedStep::operator()(const NumericalPoint & inP) const
{
  const UnsignedLong dimension( epsilon_.getDimension() );
  if (dimension != inP.getDimension()) throw InvalidArgumentException(HERE) << "Invalid dimension eps:" << dimension << " x:" << inP.getDimension();
  NumericalPoint result( epsilon_ );
  for (UnsignedLong i = 0; i < dimension; ++ i)
    {
      result[i] *= ( fabs( inP[i] ) + eta_[i] );
      if (result[i] < SpecFunc::MinNumericalScalar ) throw InvalidArgumentException(HERE) << "Nul step for component " << i << ": eps=" << epsilon_[i] << " x=" << inP[i];
    }
  return result;
}

/* Method save() stores the object through the StorageManager */
void BlendedStep::save(Advocate & adv) const
{
  FiniteDifferenceStepImplementation::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void BlendedStep::load(Advocate & adv)
{
  FiniteDifferenceStepImplementation::load(adv);
}

/* Eta accessor */
void BlendedStep::setEta(const NumericalPoint & eta)
{
  const UnsignedLong dimension( epsilon_.getDimension() );
  if (eta.getDimension() != epsilon_.getDimension()) throw InvalidArgumentException(HERE) << "Invalid dimension: eta dimension doesn't match epsilon dimension";
  for( UnsignedLong i = 0; i < dimension; ++ i )
    {
      if ( eta[i] < 0.0 ) throw InvalidArgumentException(HERE) << "Negative eta component " << i;
    }
  eta_ = eta;
}

NumericalPoint BlendedStep::getEta() const
{
  return eta_;
}

END_NAMESPACE_OPENTURNS


