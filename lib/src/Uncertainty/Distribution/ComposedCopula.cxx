//                                               -*- C++ -*-
/**
 *  @file  ComposedCopula.cxx
 *  @brief Abstract top-level class for all ComposedCopulas
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cmath>

#include "ComposedCopula.hxx"
#include "PersistentObjectFactory.hxx"
#include "IndependentCopula.hxx"
#include "Exception.hxx"
#include "Log.hxx"
#include "Normal.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<Copula>);
static Factory<PersistentCollection<Copula> > RegisteredFactory2("PersistentCollection<Copula>");

CLASSNAMEINIT(ComposedCopula);

static Factory<ComposedCopula> RegisteredFactory("ComposedCopula");

/* Default constructor */
ComposedCopula::ComposedCopula()
  : CopulaImplementation("ComposedCopula")
{
  setDimension(1);
  CopulaCollection coll(1);
  coll[0] = IndependentCopula(1);
  setCopulaCollection( coll );
}

/* Default constructor */
ComposedCopula::ComposedCopula(const CopulaCollection & coll)
  : CopulaImplementation("ComposedCopula")
  , copulaCollection_()
{
  // We assign the copula collection through the accessor in order to compute the composed copula dimension
  setCopulaCollection(coll);
}

/* Comparison operator */
Bool ComposedCopula::operator ==(const ComposedCopula & other) const
{
  if (this == &other) return true;
  return copulaCollection_ == other.copulaCollection_;
}

/* String converter */
String ComposedCopula::__repr__() const
{
  OSS oss;
  oss << "class=" << ComposedCopula::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  for (UnsignedLong i = 0; i < copulaCollection_.getSize(); ++i)
    oss << " copula[" << i << "]=" << copulaCollection_[i];
  return oss;
}

String ComposedCopula::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "(";
  String separator("");
  for (UnsignedLong i = 0; i < copulaCollection_.getSize(); ++i)
    {
      oss << separator << copulaCollection_[i].__str__();
      separator = ", ";
    }
  oss << ")";
  return oss;
}



/* Distribution collection accessor */
void ComposedCopula::setCopulaCollection(const CopulaCollection & coll)
{
  // Check if the collection is not empty
  const UnsignedLong size(coll.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Collection of distributions is empty";
  copulaCollection_ = coll;
  Description description(0);
  UnsignedLong dimension(0);
  // Compute the dimension and build the description
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const UnsignedLong copulaDimension(coll[i].getDimension());
      dimension += copulaDimension;
      const Description copulaDescription(coll[i].getDescription());
      for (UnsignedLong j = 0; j < copulaDimension; ++j)
        {
          description.add(copulaDescription[j]);
        }
    }
  isAlreadyComputedCovariance_ = false;
  // One MUST set the dimension BEFORE the description, else an error occurs
  setDimension(dimension);
  setDescription(description);
}


/* Distribution collection accessor */
const ComposedCopula::CopulaCollection & ComposedCopula::getCopulaCollection() const
{
  return copulaCollection_;
}

/* Virtual constructor */
ComposedCopula * ComposedCopula::clone() const
{
  return new ComposedCopula(*this);
}

/* Get one realization of the ComposedCopula */
NumericalPoint ComposedCopula::getRealization() const
{
  const UnsignedLong dimension(getDimension());
  const UnsignedLong size(copulaCollection_.getSize());
  NumericalPoint result(dimension);
  UnsignedLong index(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint realization(copulaCollection_[i].getRealization());
      const UnsignedLong copulaDimension(copulaCollection_[i].getDimension());
      for (UnsignedLong j = 0; j < copulaDimension; ++j)
        {
          result[index] = realization[j];
          ++index;
        }
    }
  return result;
}

/* Get the DDF of the ComposedCopula */
NumericalPoint ComposedCopula::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if(point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "The given point has a wrong dimension";
  NumericalPoint DDF(getDimension());
  const UnsignedLong size(copulaCollection_.getSize());
  NumericalPoint copulaPDF(size);
  NumericalPointCollection copulaDDF(size);
  NumericalScalar productPDF(1.0);
  UnsignedLong index(0);
  // First, compute the several elements
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // If one component is outside of the support, the PDF is null
      if ((point[i] <= 0.0) || (point[i] >= 1.0)) return NumericalPoint(dimension);
      const Copula copula(copulaCollection_[i]);
      const UnsignedLong copulaDimension(copula.getDimension());
      NumericalPoint component(copulaDimension);
      for (UnsignedLong j = 0; j < copulaDimension; ++j)
        {
          component[j] = point[index];
          ++index;
        }
      copulaPDF[i] = copula.computePDF(component);
      productPDF *= copulaPDF[i];
      copulaDDF[i] = copula.computeDDF(component);
    }
  // Then, compute the DDF
  index = 0;
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint currentDDF(copulaDDF[i] * (productPDF / copulaPDF[i]));
      const UnsignedLong currentDimension(currentDDF.getDimension());
      for (UnsignedLong j = 0; j < currentDimension; ++j)
        {
          DDF[index] = currentDDF[j];
          ++index;
        }
    }
  return DDF;
}

/* Get the PDF of the ComposedCopula */
NumericalScalar ComposedCopula::computePDF(const NumericalPoint & point) const
{
  /* PDF = PDF_copula1x...xPDF_copula_n */
  if(point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "The given point has a wrong dimension";
  const UnsignedLong size(copulaCollection_.getSize());
  NumericalScalar productPDF(1.0);
  UnsignedLong index(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // If one component is outside of the support, the PDF is null
      if ((point[i] <= 0.0) || (point[i] >= 1.0)) return 0.0;
      const UnsignedLong copulaDimension(copulaCollection_[i].getDimension());
      NumericalPoint component(copulaDimension);
      for (UnsignedLong j = 0; j < copulaDimension; ++j)
        {
          component[j] = point[index];
          ++index;
        }
      productPDF *= copulaCollection_[i].computePDF(component);
    }
  return productPDF;
}

/* Get the CDF of the ComposedCopula */
NumericalScalar ComposedCopula::computeCDF(const NumericalPoint & point) const
{
  /* CDF = CDF_copula1x...xCDF_copula_n */
  if(point.getDimension() != getDimension()) throw InvalidArgumentException(HERE) << "The given point has a wrong dimension";
  const UnsignedLong size(copulaCollection_.getSize());
  NumericalScalar productCDF(1.0);
  UnsignedLong index(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // If one component is at the left of the support of its matginal distribution, the CDF is null
      if (point[i] <= 0.0) return 0.0;
      // If the component is inside of the support, update the CDF value
      if (point[i] < 1.0)
        {
          const UnsignedLong copulaDimension(copulaCollection_[i].getDimension());
          NumericalPoint component(copulaDimension);
          for (UnsignedLong j = 0; j < copulaDimension; ++j)
            {
              component[j] = point[index];
              ++index;
            }
	  const NumericalScalar cdf(copulaCollection_[i].computeCDF(component));
          productCDF *= cdf;
        }
    }
  return productCDF;
}

/* Compute the probability content of an interval */
NumericalScalar ComposedCopula::computeProbability(const Interval & interval) const
{
  const UnsignedLong dimension(getDimension());
  // Reduce the given interval to the support of the distribution, which is the nD unit cube
  const Interval intersect(interval.intersect(Interval(dimension)));
  // If the intersection is empty
  if (intersect.isNumericallyEmpty()) return 0.0;
  const NumericalPoint lowerIntersect(intersect.getLowerBound());
  const NumericalPoint upperIntersect(intersect.getUpperBound());
  const UnsignedLong size(copulaCollection_.getSize());
  NumericalScalar value(1.0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const Copula copula(copulaCollection_[i]);
      const UnsignedLong copulaDimension(copula.getDimension());
      NumericalPoint lower(copulaDimension);
      NumericalPoint upper(copulaDimension);
      for (UnsignedLong j = 0; j < copulaDimension; ++j)
        {
          lower[j] = lowerIntersect[j];
          upper[j] = upperIntersect[j];
        }
      value *= copula.computeProbability(Interval(lower, upper));
    }
  return value;
}

/* Get the PDF gradient of the distribution */
NumericalPoint ComposedCopula::computePDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the CDF gradient of the distribution */
NumericalPoint ComposedCopula::computeCDFGradient(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ComposedCopula::computeConditionalPDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return Distribution(getMarginal(conditioningDimension)).computePDF(x);
  // General case
  UnsignedLong copulaIndex(0);
  UnsignedLong partialDimension(copulaCollection_[copulaIndex].getDimension());
  while (partialDimension < conditioningDimension)
    {
      ++copulaIndex;
      partialDimension += copulaCollection_[copulaIndex].getDimension();
    }
  // Extract the relevant part of the conditioning
  const UnsignedLong conditioningSize(partialDimension - conditioningDimension);
  NumericalPoint conditioningVector(conditioningSize);
  for (UnsignedLong i = 0; i < conditioningSize; ++i)
    {
      conditioningVector[i] = y[conditioningDimension - conditioningSize + i];
    }
  return copulaCollection_[copulaIndex].computeConditionalPDF(x, conditioningVector);
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ComposedCopula::computeConditionalCDF(const NumericalScalar x, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return Distribution(getMarginal(conditioningDimension)).computeCDF(x);
  // General case
  UnsignedLong copulaIndex(0);
  UnsignedLong partialDimension(copulaCollection_[copulaIndex].getDimension());
  while (partialDimension < conditioningDimension)
    {
      ++copulaIndex;
      partialDimension += copulaCollection_[copulaIndex].getDimension();
    }
  // Extract the relevant part of the conditioning
  const UnsignedLong conditioningSize(partialDimension - conditioningDimension);
  NumericalPoint conditioningVector(conditioningSize);
  for (UnsignedLong i = 0; i < conditioningSize; ++i)
    {
      conditioningVector[i] = y[conditioningDimension - conditioningSize + i];
    }
  return copulaCollection_[copulaIndex].computeConditionalCDF(x, conditioningVector);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ComposedCopula::computeConditionalQuantile(const NumericalScalar q, const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension == 0) return q;
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if (conditioningDimension == 0) return q;
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  if (q == 0.0) return 0.0;
  if (q == 1.0) return 1.0;
  // General case
  UnsignedLong copulaIndex(0);
  UnsignedLong partialDimension(copulaCollection_[copulaIndex].getDimension());
  while (partialDimension < conditioningDimension)
    {
      ++copulaIndex;
      partialDimension += copulaCollection_[copulaIndex].getDimension();
    }
  // Extract the relevant part of the conditioning
  const UnsignedLong conditioningSize(partialDimension - conditioningDimension);
  NumericalPoint conditioningVector(conditioningSize);
  for (UnsignedLong i = 0; i < conditioningSize; ++i)
    {
      conditioningVector[i] = y[conditioningDimension - conditioningSize + i];
    }
  return copulaCollection_[copulaIndex].computeConditionalQuantile(q, conditioningVector);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions
   Some additional restriction occur for this copula: the indices must be of the form:
   [i_1^1,...,i_k1^1,i_1^2,...,i_k2^2,...,i_1^n,...,i_kn^n]
   where:
   i_1^1,...,i_k1^1 is a subset of {0,...,dim_1-1},
   i_1^2,...,i_k2^2 is a subset of {0,...,dim_2-1}+dim_1,
   i_1^n,...,i_kn^n is a subset of {0,...,dim_n-1}+dim_1+...+dim_(n-1),
   dim_1 = dimension(copula_1) etc.
*/
ComposedCopula::Implementation ComposedCopula::getMarginal(const Indices & indices) const
{
  const UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "Error: the indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  CopulaCollection marginalCopulas;
  const UnsignedLong indicesSize(indices.getSize());
  const UnsignedLong size(copulaCollection_.getSize());
  // For each copula, see if there is something to extract
  UnsignedLong currentPosition(0);
  UnsignedLong currentIndex(indices[currentPosition]);
  // Lower bound of indices related to the current copula
  UnsignedLong lowerIndex(0);
  // Upper bound of indices related to the current copula plus 1
  UnsignedLong upperIndex(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const Copula copula(copulaCollection_[i]);
      // Update index range for the current copula
      lowerIndex = upperIndex;
      upperIndex += copula.getDimension();
      Indices copulaIndices(0);
      // Find the indices related to the current copula
      while ((currentPosition < indicesSize) && (currentIndex >= lowerIndex) && (currentIndex < upperIndex))
        {
          copulaIndices.add(currentIndex - lowerIndex);
          // Go to next index
          ++currentPosition;
          if (currentPosition == indicesSize) break;
          currentIndex = indices[currentPosition];
        }
      // If there is something to extract
      if (copulaIndices.getSize() > 0) marginalCopulas.add(copulaCollection_[i].getMarginal(copulaIndices));
      // All the indices have been taken into account
      if (currentPosition == indicesSize) break;
      // Check if a bad case occurs: one index related to copula i is found after indices related to copula j, with j > i
      if (currentIndex < lowerIndex) throw InvalidArgumentException(HERE) << "Error: one index related to the ith copula has been found after indices related to the jth copula, with j > i";
    }
  return new ComposedCopula(marginalCopulas);
}

/* Parameters value and description accessor */
ComposedCopula::NumericalPointWithDescriptionCollection ComposedCopula::getParametersCollection() const
{
  NumericalPointWithDescriptionCollection parameters(1);
  // No marginal parameter
  // Put the dependence parameters
  NumericalPointWithDescription point(0);
  Description description(0);
  const UnsignedLong size(copulaCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // All distributions, including copulas, must output a collection of NumericalPoint of size at least 1,
      // even if the NumericalPoint are empty
      const NumericalPointWithDescription copulaParameters(copulaCollection_[i].getParametersCollection()[0]);
      const Description parametersDescription(copulaParameters.getDescription());
      const String copulaName(copulaCollection_[i].getName());
      const UnsignedLong parameterDimension(copulaParameters.getDimension());
      for (UnsignedLong j = 0; j < parameterDimension; ++j)
        {
          point.add(copulaParameters[j]);
          description.add(OSS() << "copula_" << i << "_" << parametersDescription[j]);
        }
    }
  point.setDescription(description);
  point.setName(getName());
  parameters[0] = point;
  return parameters;
} // getParametersCollection

void ComposedCopula::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  // Check if the given parameters are ok
  if (parametersCollection.getSize() != 1) throw InvalidArgumentException(HERE) << "Error: the given collection has a size=" << parametersCollection.getSize() << " but should be of size=1";
  // Dependence parameters
  const NumericalPoint parameters(parametersCollection[0]);
  const UnsignedLong parametersDimension(parameters.getDimension());
  // Index within the given parametersCollection
  UnsignedLong globalIndex(0);
  const UnsignedLong size(copulaCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // All distributions, including copulas, must output a collection of NumericalPoint of size at least 1,
      // even if the NumericalPoint are empty
      const UnsignedLong atomParametersDimension(copulaCollection_[i].getParametersCollection()[0].getDimension());
      // ith copula parameters
      NumericalPoint point(atomParametersDimension);
      for (UnsignedLong j = 0; j < atomParametersDimension; ++j)
        {
          if (globalIndex >= parametersDimension) throw InvalidArgumentException(HERE) << "Error: there are too few dependence parameters";
          point[j] = parameters[globalIndex];
          ++globalIndex;
        } // atom parameters
      copulaCollection_[i].setParametersCollection(NumericalPointCollection(1, point));
    } // atoms
  if (globalIndex != parametersDimension) throw InvalidArgumentException(HERE) << "Error: there are too many dependence parameters, expected " << globalIndex << " parameters and got " << parametersDimension;
}

/* Tell if the distribution has elliptical copula */
Bool ComposedCopula::hasEllipticalCopula() const
{
  return hasIndependentCopula();
}

/* Tell if the distribution has independent copula */
Bool ComposedCopula::hasIndependentCopula() const
{
  const UnsignedLong size(copulaCollection_.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // As soon as one of the elementary copula is not the independent one, the composed copula
      // is not the independent one
      if (!copulaCollection_[i].getImplementation()->hasIndependentCopula()) return false;
    }
  return true;
}

/* Method save() stores the object through the StorageManager */
void ComposedCopula::save(Advocate & adv) const
{
  CopulaImplementation::save(adv);
  adv.saveAttribute( "copulaCollection_", copulaCollection_ );
}

/* Method load() reloads the object from the StorageManager */
void ComposedCopula::load(Advocate & adv)
{
  CopulaImplementation::load(adv);
  adv.loadAttribute( "copulaCollection_", copulaCollection_ );
}

END_NAMESPACE_OPENTURNS
