//                                               -*- C++ -*-
/**
 *  @file  LinearNumericalMathGradientImplementation.cxx
 *  @brief Class for a quadratic numerical math gradient implementation
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
#include "LinearNumericalMathGradientImplementation.hxx"
#include "PersistentObjectFactory.hxx"


BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(LinearNumericalMathGradientImplementation);

static Factory<LinearNumericalMathGradientImplementation> RegisteredFactory("LinearNumericalMathGradientImplementation");

/* Default constructor */
LinearNumericalMathGradientImplementation::LinearNumericalMathGradientImplementation()
  : NumericalMathGradientImplementation()
{
  // Nothing to do
}

/* Parameter constructor */
LinearNumericalMathGradientImplementation::LinearNumericalMathGradientImplementation(const NumericalPoint & center,
                                                                                     const Matrix & constant,
                                                                                     const SymmetricTensor & linear)
  : NumericalMathGradientImplementation(),
    center_(center),
    constant_(constant),
    linear_(linear)
{
  /* Check if the dimensions of the constant term is compatible with the linear term */
  if ((constant.getNbRows() != linear.getNbRows()) || (constant.getNbColumns() != linear.getNbSheets())) throw InvalidDimensionException(HERE) << "Constant term dimensions are incompatible with the linear term";
  /* Check if the dimensions of the center term is compatible with the linear term */
  if ((center.getDimension() != constant.getNbRows()) || (center.getDimension() != linear.getNbRows())) throw InvalidDimensionException(HERE) << "Center term dimensions are incompatible with the constant term or the linear term";
}

/* Virtual constructor */
LinearNumericalMathGradientImplementation * LinearNumericalMathGradientImplementation::clone() const
{
  return new LinearNumericalMathGradientImplementation(*this);
}

/* Comparison operator */
Bool LinearNumericalMathGradientImplementation::operator ==(const LinearNumericalMathGradientImplementation & other) const
{
  return ((linear_ == other.linear_) && (constant_ == other.constant_) && (center_ == other.center_));
}

/* String converter */
String LinearNumericalMathGradientImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << LinearNumericalMathGradientImplementation::GetClassName()
      << " name=" << getName()
      << " center=" << center_.__repr__()
      << " constant=" << constant_.__repr__()
      << " linear=" << linear_;
  return oss;
}

/* Accessor for the center */
NumericalPoint LinearNumericalMathGradientImplementation::getCenter() const
{
  return center_;
}

/* Accessor for the constant term */
Matrix LinearNumericalMathGradientImplementation::getConstant() const
{
  return constant_;
}

/* Accessor for the linear term */
SymmetricTensor LinearNumericalMathGradientImplementation::getLinear() const
{
  return linear_;
}

/* Here is the interface that all derived class must implement */

/* Gradient() */
Matrix LinearNumericalMathGradientImplementation::gradient(const NumericalPoint & inP) const
{
  if (inP.getDimension() != constant_.getNbRows()) throw InvalidArgumentException(HERE) << "Invalid input dimension";
  Matrix value(constant_);
  // Add the linear term <linear, x>
  for(UnsignedLong i = 0; i < linear_.getNbRows(); ++i)
    {
      for(UnsignedLong j = 0; j < linear_.getNbColumns(); ++j)
        {
          for(UnsignedLong k = 0; k < linear_.getNbSheets(); ++k)
            {
              value(i, k) += (inP[j] - center_[j]) * linear_(i, j, k);
            }
        }
    }
  ++callsNumber_;
  return value;
}

/* Accessor for input point dimension */
UnsignedLong LinearNumericalMathGradientImplementation::getInputDimension() const
{
  return center_.getDimension();
}

/* Accessor for output point dimension */
UnsignedLong LinearNumericalMathGradientImplementation::getOutputDimension() const
{
  return constant_.getNbColumns();
}

/* Method save() stores the object through the StorageManager */
void LinearNumericalMathGradientImplementation::save(Advocate & adv) const
{
  NumericalMathGradientImplementation::save(adv);
  adv.saveAttribute( "center_", center_ );
  adv.saveAttribute( "constant_", constant_ );
  adv.saveAttribute( "linear_", linear_ );
}

/* Method load() reloads the object from the StorageManager */
void LinearNumericalMathGradientImplementation::load(Advocate & adv)
{
  NumericalMathGradientImplementation::load(adv);
  adv.loadAttribute( "center_", center_ );
  adv.loadAttribute( "constant_", constant_ );
  adv.loadAttribute( "linear_", linear_ );
}

END_NAMESPACE_OPENTURNS
