//                                               -*- C++ -*-
/**
 *  @file  MixtureClassifier.cxx
 *  @brief Associate to a given point its class index
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
 *  @date   2010-11-09 13:44:00 +0100 (Tue, 09 Nov 2010)
 */

#include "MixtureClassifier.hxx"
#include "PersistentObjectFactory.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MixtureClassifier);

static Factory<MixtureClassifier> RegisteredFactory("MixtureClassifier");

/* Default constructor */
MixtureClassifier::MixtureClassifier()
  : ClassifierImplementation()
{
  // Nothing to do
}

/* Default constructor */
MixtureClassifier::MixtureClassifier(const Mixture & mixture)
  : ClassifierImplementation()
  , mixture_(mixture)
{
  // Nothing to do
}

/* Virtual constructor */
MixtureClassifier * MixtureClassifier::clone() const
{
  return new MixtureClassifier( *this );
}

/* String converter */
String MixtureClassifier::__repr__() const
{
  return OSS() << "class=" << GetClassName()
               << " mixture=" << mixture_;
}

UnsignedLong MixtureClassifier::classify(const NumericalPoint& inP) const
{
  if (inP.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "Error: the point to classify has dimension=" << inP.getDimension() << " but the classifier expects dimension=" << mixture_.getDimension();
  const UnsignedLong size(mixture_.getDistributionCollection().getSize());
  UnsignedLong bestClass(0);
  Distribution atom(mixture_.getDistributionCollection()[0]);
  NumericalScalar bestGrade(log(atom.getWeight()) + atom.computeLogPDF( inP ));
  for ( UnsignedLong classIndex = 1; classIndex < size; ++classIndex )
    {
      atom = mixture_.getDistributionCollection()[classIndex];
      const NumericalScalar grade(log(atom.getWeight()) + atom.computeLogPDF( inP ));
      if ( grade > bestGrade )
        {
          bestClass = classIndex;
          bestGrade = grade;
        }
    }
  return bestClass;
}

NumericalScalar MixtureClassifier::grade(const NumericalPoint& inP,
                                         const UnsignedLong outC) const
{
  const UnsignedLong size(mixture_.getDistributionCollection().getSize());
  if (outC >= size) throw InvalidDimensionException(HERE) << "Class number (=" << outC << ") must be lower than size (=" << size << ").";
  return log(mixture_.getDistributionCollection()[outC].getWeight()) + mixture_.getDistributionCollection()[outC].computeLogPDF( inP );
}

/* Mixture accessors */
Mixture MixtureClassifier::getMixture() const
{
  return mixture_;
}

void MixtureClassifier::setMixture(const Mixture & mixture)
{
  mixture_ = mixture;
}

/* Dimension accessor */
UnsignedLong MixtureClassifier::getDimension() const
{
  return mixture_.getDimension();
}

/* Method save() stores the object through the StorageManager */
void MixtureClassifier::save(Advocate & adv) const
{
  ClassifierImplementation::save(adv);
  adv.saveAttribute( "mixture_", mixture_ );
}

/* Method load() reloads the object from the StorageManager */
void MixtureClassifier::load(Advocate & adv)
{
  ClassifierImplementation::load(adv);
  adv.loadAttribute( "mixture_", mixture_ );
}

END_NAMESPACE_OPENTURNS
