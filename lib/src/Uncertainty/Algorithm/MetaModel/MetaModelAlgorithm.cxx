//                                               -*- C++ -*-
/**
 *  @file  MetaModelAlgorithm.cxx
 *  @brief Implementation for metamodel algorithms
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
 *  @date   2008-06-05 12:11:56 +0200 (Thu, 05 Jun 2008)
 */

#include "MetaModelAlgorithm.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(MetaModelAlgorithm);

static Factory<MetaModelAlgorithm> RegisteredFactory("MetaModelAlgorithm");

/* Default constructor */
MetaModelAlgorithm::MetaModelAlgorithm(const String & name)
  : PersistentObject( name )
{
}

/* Constructor with parameters */
MetaModelAlgorithm::MetaModelAlgorithm(const Distribution & distribution,
                                       const NumericalMathFunction & model,
                                       const String & name)
  : PersistentObject( name ),
    distribution_(distribution),
    model_(model)
{
}

/* Virtual constructor */
MetaModelAlgorithm * MetaModelAlgorithm::clone() const
{
  return new MetaModelAlgorithm(*this);
}

/* String converter */
String MetaModelAlgorithm::__repr__() const
{
  OSS oss;
  oss << "class=" << GetClassName()
      << " name=" << getName();
  return oss;
}



/* Distribution accessor */
void MetaModelAlgorithm::setDistribution(const Distribution & distribution)
{
  distribution_ = distribution;
}

Distribution MetaModelAlgorithm::getDistribution() const
{
  return distribution_;
}

/* Response surface computation */
void MetaModelAlgorithm::run()
{
  throw NotYetImplementedException(HERE) << " in MetaModelAlgorithm::run()";
}


NumericalSample MetaModelAlgorithm::getInputSample() const
{
  throw NotYetImplementedException(HERE);
  return NumericalSample();
}


NumericalSample MetaModelAlgorithm::getOutputSample() const
{
  throw NotYetImplementedException(HERE);
  return NumericalSample();
}


/* Method save() stores the object through the StorageManager */
void MetaModelAlgorithm::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "distribution_", distribution_ );
  adv.saveAttribute( "model_", model_ );

}

/* Method load() reloads the object from the StorageManager */
void MetaModelAlgorithm::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "distribution_", distribution_ );
  adv.loadAttribute( "model_", model_ );
}

END_NAMESPACE_OPENTURNS
