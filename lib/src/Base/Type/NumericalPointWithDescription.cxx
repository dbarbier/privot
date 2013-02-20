//                                               -*- C++ -*-
/**
 *  @file  NumericalPointWithDescription.cxx
 *  @brief NumericalPointWithDescription extends the classical mathematical point with a description of the coordinates
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
#include "NumericalPointWithDescription.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(NumericalPointWithDescription);
TEMPLATE_CLASSNAMEINIT(PersistentCollection<NumericalPointWithDescription>);

static Factory<NumericalPointWithDescription> RegisteredFactory1("NumericalPointWithDescription");
static Factory<PersistentCollection<NumericalPointWithDescription> > RegisteredFactory2("PersistentCollection<NumericalPointWithDescription>");

/* Default constructor */
NumericalPointWithDescription::NumericalPointWithDescription()
  : NumericalPoint(),
    description_()
{
  // Nothing to do
}

/* Constructor with size */
NumericalPointWithDescription::NumericalPointWithDescription(const UnsignedLong size,
                                                             const NumericalScalar value)
  : NumericalPoint(size, value),
    description_(size)
{
  // Nothing to do
}



/* Constructor from a collection */
NumericalPointWithDescription::NumericalPointWithDescription(const Collection<NumericalScalar> & coll)
  : NumericalPoint(coll),
    description_(coll.getSize())
{
  // Nothing to do
}

/* Constructor from a base class */
NumericalPointWithDescription::NumericalPointWithDescription(const Pointer<NumericalPoint> & p_base)
  : NumericalPoint(*p_base),
    description_(p_base->getSize())
{
  // Nothing to do
}



/* Virtual constructor */
NumericalPointWithDescription * NumericalPointWithDescription::clone() const
{
  return new NumericalPointWithDescription(*this);
}


/* Destructor */
NumericalPointWithDescription::~NumericalPointWithDescription() throw()
{
  // Nothing to do
}


/* String converter */
String NumericalPointWithDescription::__repr__() const
{
  return OSS() << "class=" << NumericalPointWithDescription::GetClassName()
               << " name=" << getName()
               << " dimension=" << getDimension()
               << " description=" << getDescription()
               << " values=" << PersistentCollection<NumericalScalar>::__repr__();
}

class KeyValuePair : public Object
{
  String key_;
  NumericalScalar value_;
public:
  KeyValuePair(const String & key = "", NumericalScalar value = 0.): key_(key), value_(value) {}
  String __repr__() const
  {
    return OSS() << key_ << " : " << value_;
  }
  String __str__(const String & offset) const
  {
    return OSS() << key_ << " : " << value_;
  }
};

String NumericalPointWithDescription::__str__(const String & offset) const
{
  const UnsignedLong size = getSize();
  const Description desc = getDescription();
  std::vector<KeyValuePair> assoc(size);
  for (UnsignedLong i = 0; i < size; ++i) assoc[i] = KeyValuePair( desc[i], (*this)[i] );

  OSS oss(false);
  oss << "[";
  std::copy( assoc.begin(), assoc.end(), OSS_iterator<KeyValuePair>(oss, ", ") );
  oss << "]";
  return oss;
}




/* Description Accessor */
void NumericalPointWithDescription::setDescription(const Description & description)
{
  if (description.getSize() != getDimension())
    throw InvalidArgumentException(HERE) << "Description has incorrect dimension (" << description.getSize()
                                         << "). Expected " << getDimension();
  description_ = description;
}


/* Description Accessor */
Description NumericalPointWithDescription::getDescription() const
{
  return description_;
}




/* Method save() stores the object through the StorageManager */
void NumericalPointWithDescription::save(Advocate & adv) const
{
  NumericalPoint::save(adv);
  adv.saveAttribute( "description_", description_ );
}


/* Method load() reloads the object from the StorageManager */
void NumericalPointWithDescription::load(Advocate & adv)
{
  NumericalPoint::load(adv);
  adv.loadAttribute( "description_", description_ );
}


END_NAMESPACE_OPENTURNS
