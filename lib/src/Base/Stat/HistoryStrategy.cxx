//                                               -*- C++ -*-
/**
 *  @file  HistoryStrategy.cxx
 *  @brief This class is a top-level class for the history mechanism.
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
#include "HistoryStrategy.hxx"
#include "Null.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class HistoryStrategy
 */

CLASSNAMEINIT(HistoryStrategy);

/* Defaultonstructor */
HistoryStrategy::HistoryStrategy()
  : TypedInterfaceObject<HistoryStrategyImplementation>(Null().clone())
{
  // Nothing to do
}

/* Constructor with parameters */
HistoryStrategy::HistoryStrategy(Implementation & p_implementation)
  : TypedInterfaceObject<HistoryStrategyImplementation>(p_implementation)
{
  // Nothing to do
}

/* Constructor with parameters */
HistoryStrategy::HistoryStrategy(const HistoryStrategyImplementation & implementation)
  : TypedInterfaceObject<HistoryStrategyImplementation>(implementation.clone())
{
  // Nothing to do
}

/* Store the point according to the strategy */
void HistoryStrategy::store(const NumericalPoint & point)
{
  copyOnWrite();
  getImplementation()->store(point);
}

void HistoryStrategy::store(const NumericalSample & sample)
{
  copyOnWrite();
  getImplementation()->store(sample);
}

/* Clear the history storage */
void HistoryStrategy::clear()
{
  copyOnWrite();
  getImplementation()->clear();
}

/* Clear the history storage */
void HistoryStrategy::reset()
{
  copyOnWrite();
  getImplementation()->reset();
}

/* History sample accessor */
NumericalSample HistoryStrategy::getSample() const
{
  return getImplementation()->getSample();
}

/* String converter */
String HistoryStrategy::__repr__() const
{
  OSS oss;
  oss << "class=" << HistoryStrategy::GetClassName()
      << " implementation=" << getImplementation()->__repr__();
  return oss;
}



END_NAMESPACE_OPENTURNS
