//                                               -*- C++ -*-
/**
 *  @file  TracyWidomGUE.cxx
 *  @brief The TracyWidomGUE distribution
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
 *  @author dutka
 *  @date   2010-02-04 16:44:49 +0100 (jeu. 04 févr. 2010)
 */
#include <cmath>
#include "TracyWidomGUE.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(TracyWidomGUE);

static Factory<TracyWidomGUE> RegisteredFactory("TracyWidomGUE");


/* Default constructor */
TracyWidomGUE::TracyWidomGUE()
  : NonEllipticalDistribution("TracyWidomGUE")
{
  throw NotYetImplementedException(HERE);
  setDimension(1);
  setRange(Interval(-1.0, 1.0));
}

/* Comparison operator */
Bool TracyWidomGUE::operator ==(const TracyWidomGUE & other) const
{
  return true;
}

/* String converter */
String TracyWidomGUE::__repr__() const
{
  OSS oss;
  oss << "class=" << TracyWidomGUE::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  return oss;
}

/* Virtual constructor */
TracyWidomGUE * TracyWidomGUE::clone() const
{
  return new TracyWidomGUE(*this);
}

/* Get the DDF of the distribution */
NumericalPoint TracyWidomGUE::computeDDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}


/* Get the PDF of the distribution */
NumericalScalar TracyWidomGUE::computePDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}


/* Get the CDF of the distribution */
NumericalScalar TracyWidomGUE::computeCDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/** Get the PDFGradient of the distribution */
NumericalPoint TracyWidomGUE::computePDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0);
}

/** Get the CDFGradient of the distribution */
NumericalPoint TracyWidomGUE::computeCDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0);
}

/* Get the quantile of the distribution */
NumericalScalar TracyWidomGUE::computeScalarQuantile(const NumericalScalar prob,
                                                     const Bool tail,
                                                     const NumericalScalar precision) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar TracyWidomGUE::getRoughness() const
{
  throw NotYetImplementedException(HERE);
}

/* Compute the mean of the distribution */
void TracyWidomGUE::computeMean() const
{
  // RMT matlab toolbox computation
  mean_ = NumericalPoint(1, -1.771086807411);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint TracyWidomGUE::getStandardDeviation() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.813194792832);
}

/* Get the skewness of the distribution */
NumericalPoint TracyWidomGUE::getSkewness() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.224084203610);
}

/* Get the kurtosis of the distribution */
NumericalPoint TracyWidomGUE::getKurtosis() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.0934480876);
}

/* Compute the covariance of the distribution */
void TracyWidomGUE::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar sigma(getStandardDeviation()[0]);
  covariance_(0, 0) = sigma * sigma;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
TracyWidomGUE::NumericalPointWithDescriptionCollection TracyWidomGUE::getParametersCollection() const
{
  // No parameter, return a collection with an empty NumericalPointWithDescription
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

/* Method save() stores the object through the StorageManager */
void TracyWidomGUE::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void TracyWidomGUE::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
}

END_NAMESPACE_OPENTURNS
