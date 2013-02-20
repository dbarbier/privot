//                                               -*- C++ -*-
/**
 *  @file  SamplerImplementation.cxx
 *  @brief Abstract top-level class for all SamplerImplementation
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
#include "SamplerImplementation.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(SamplerImplementation);

static Factory<SamplerImplementation> RegisteredFactory("SamplerImplementation");

/* Default constructor */
SamplerImplementation::SamplerImplementation()
  : PersistentObject()
  , verbose_(false)
{

}

/* String converter */
String SamplerImplementation::__repr__() const
{
  return OSS() << "class=" << SamplerImplementation::GetClassName()
               << " name=" << getName();
}


SamplerImplementation* SamplerImplementation::clone() const
{
  return new SamplerImplementation(*this);
}


void SamplerImplementation::setVerbose(const Bool verbose)
{
  verbose_ = verbose;
}


Bool SamplerImplementation::getVerbose() const
{
  return verbose_;
}

/* Here is the interface that all derived class must implement */


UnsignedLong SamplerImplementation::getDimension() const
{
  throw NotYetImplementedException(HERE);
}


NumericalPoint SamplerImplementation::getRealization() const
{
  throw NotYetImplementedException(HERE);
}


NumericalSample SamplerImplementation::getSample(const UnsignedLong size) const
{
  NumericalSample sample(size, getDimension());
  for (UnsignedLong i = 0; i < size; ++ i)
    {
      NumericalPoint realization(getRealization());
      sample[i] = realization;
    }
  return sample;
}


/* Method save() stores the object through the StorageManager */
void SamplerImplementation::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute("verbose_", verbose_);
}


/* Method load() reloads the object from the StorageManager */
void SamplerImplementation::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute("verbose_", verbose_);
}


END_NAMESPACE_OPENTURNS
