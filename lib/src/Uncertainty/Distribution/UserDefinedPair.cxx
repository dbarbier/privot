//                                               -*- C++ -*-
/**
 *  @file  UserDefinedPair.cxx
 *  @brief The UserDefinedPair class.
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
 *  @date   2010-11-21 19:45:57 +0100 (dim. 21 nov. 2010)
 */
#include "UserDefinedPair.hxx"
#include "OSS.hxx"
#include "PersistentObjectFactory.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(UserDefinedPair);


static Factory<UserDefinedPair> RegisteredFactory("UserDefinedPair");

/* Default constructor */
UserDefinedPair::UserDefinedPair()
  : x_(1, 0.0),
    p_(1.0)
{
  // Nothing to do
}

/* Parameters constructor */
UserDefinedPair::UserDefinedPair(const NumericalPoint & x,
                                 const NumericalScalar p)
  : x_(x),
    p_(0.0)
{
  setP(p);
}

/* X accessor */
void UserDefinedPair::setX(const NumericalPoint & x)
{
  x_ = x;
}

NumericalPoint UserDefinedPair::getX() const
{
  return x_;
}

/* P accessor */
void UserDefinedPair::setP(const NumericalScalar p)
{
  if ((p < 0.0) || (p > 1.0)) throw InvalidArgumentException(HERE) << "Error: the probability of a UserDefinedPair must be in [0, 1].";
  p_ = p;
}

NumericalScalar UserDefinedPair::getP() const
{
  return p_;
}

/* Comparison operator */
Bool UserDefinedPair::operator == (const UserDefinedPair & other) const
{
  if (this == &other) return true;
  return (x_ == other.x_) && (p_ == other.p_);
}

/* Virtual constructor */
UserDefinedPair * UserDefinedPair::clone() const
{
  return new UserDefinedPair(*this);
}

String UserDefinedPair::__repr__() const
{
  OSS oss;
  oss << "(" << x_ << "," << p_ << ")";
  return oss;
}

/* Method save() stores the object through the StorageManager */
void UserDefinedPair::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "x_", x_ );
  adv.saveAttribute( "p_", p_ );
}

/* Method load() reloads the object from the StorageManager */
void UserDefinedPair::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "x_", x_ );
  adv.loadAttribute( "p_", p_ );
}

END_NAMESPACE_OPENTURNS
