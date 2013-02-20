//                                               -*- C++ -*-
/**
 * @file  EventProcess.cxx
 * @brief The class that implements an event random vector (not necessary composite)
 * built upon a process and a domain.
 * The class check that a realization of a stochastic process is in a domain \mathcal{D}
 * using contains method
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
 * @author schueller
 * @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */

#include "EventProcess.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(EventProcess);

static Factory<EventProcess> RegisteredFactory("EventProcess");

/* Default constructor */
EventProcess::EventProcess()
  : RandomVectorImplementation()
{
  // Nothing to do
}

/* Constructor from RandomVector */
EventProcess::EventProcess(const Process & process,
                           const Domain & domain,
                           const String & name)
  : RandomVectorImplementation(name),
    process_(process),
    domain_(domain)
{
  // Nothing to do
}

EventProcess * EventProcess::clone() const
{
  return new EventProcess(*this);
}

/* String converter */
String EventProcess::__repr__() const
{
  OSS oss;
  oss << "class=" << EventProcess::GetClassName()
      << " domain=" << domain_
      << " process = " << process_ ;
  return oss;
}

/* Dimension accessor */
UnsignedLong EventProcess::getDimension() const
{
  return 1;
}

/* Domain accessor */
Domain EventProcess::getDomain() const
{
  return domain_;
}

/* Process accessor */
Process EventProcess::getProcess() const
{
  return process_;
}

/* check that a values of a time serie are /are not in the domain */
Bool EventProcess::checkTimeSerieValuesInDomain(const TimeSeries & ts) const
{
  const NumericalSample values(ts.getSample());
  for (UnsignedLong i = 0; i < values.getSize(); ++i) if (domain_.contains(values[i])) return true;
  return false;
}

/* Realization accessor */
NumericalPoint EventProcess::getRealization() const
{
  return NumericalPoint(1, checkTimeSerieValuesInDomain(process_.getRealization()));
}

/* Method save() stores the object through the StorageManager */
void EventProcess::save(Advocate & adv) const
{
  RandomVectorImplementation::save(adv);
  adv.saveAttribute( "domain_", domain_ );
  adv.saveAttribute( "process_", process_ );
}

/* Method load() reloads the object from the StorageManager */
void EventProcess::load(Advocate & adv)
{
  RandomVectorImplementation::load(adv);
  adv.loadAttribute( "domain_", domain_ );
  adv.loadAttribute( "process_", process_ );
}

END_NAMESPACE_OPENTURNS
