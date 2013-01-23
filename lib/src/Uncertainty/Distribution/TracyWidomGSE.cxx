//                                               -*- C++ -*-
/**
 *  @file  TracyWidomGSE.cxx
 *  @brief The TracyWidomGSE distribution
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
#include "TracyWidomGSE.hxx"
#include "RandomGenerator.hxx"
#include "Exception.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(TracyWidomGSE);

static Factory<TracyWidomGSE> RegisteredFactory("TracyWidomGSE");


/* Default constructor */
TracyWidomGSE::TracyWidomGSE()
  : NonEllipticalDistribution("TracyWidomGSE")
{
  throw NotYetImplementedException(HERE);
  setDimension(1);
  setRange(Interval(-1.0, 1.0));
}

/* Comparison operator */
Bool TracyWidomGSE::operator ==(const TracyWidomGSE & other) const
{
  return true;
}

/* String converter */
String TracyWidomGSE::__repr__() const
{
  OSS oss;
  oss << "class=" << TracyWidomGSE::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  return oss;
}

/* Virtual constructor */
TracyWidomGSE * TracyWidomGSE::clone() const
{
  return new TracyWidomGSE(*this);
}

/* Get the DDF of the distribution */
NumericalPoint TracyWidomGSE::computeDDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}


/* Get the PDF of the distribution */
NumericalScalar TracyWidomGSE::computePDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}


/* Get the CDF of the distribution */
NumericalScalar TracyWidomGSE::computeCDF(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  throw NotYetImplementedException(HERE);
}

/** Get the PDFGradient of the distribution */
NumericalPoint TracyWidomGSE::computePDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  return NumericalPoint(0);
}

/** Get the CDFGradient of the distribution */
NumericalPoint TracyWidomGSE::computeCDFGradient(const NumericalPoint & point) const
{
  if (point.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=1, here dimension=" << point.getDimension();

  return NumericalPoint(0);
}

/* Get the quantile of the distribution */
NumericalScalar TracyWidomGSE::computeScalarQuantile(const NumericalScalar prob,
                                                     const Bool tail,
                                                     const NumericalScalar precision) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar TracyWidomGSE::getRoughness() const
{
  throw NotYetImplementedException(HERE);
}

/* Compute the mean of the distribution */
void TracyWidomGSE::computeMean() const
{
  // RMT matlab toolbox computation
  mean_ = NumericalPoint(1, -2.306884893241);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint TracyWidomGSE::getStandardDeviation() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.5177237207726);
}

/* Get the skewness of the distribution */
NumericalPoint TracyWidomGSE::getSkewness() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.16550949435);
}

/* Get the kurtosis of the distribution */
NumericalPoint TracyWidomGSE::getKurtosis() const
{
  // RMT matlab toolbox computation
  return NumericalPoint(1, 0.0491951565);
}

/* Compute the covariance of the distribution */
void TracyWidomGSE::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  const NumericalScalar sigma(getStandardDeviation()[0]);
  covariance_(0, 0) = sigma * sigma;
  isAlreadyComputedCovariance_ = true;
}

/* Parameters value and description accessor */
TracyWidomGSE::NumericalPointWithDescriptionCollection TracyWidomGSE::getParametersCollection() const
{
  // No parameter, return a collection with an empty NumericalPointWithDescription
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

/* Method save() stores the object through the StorageManager */
void TracyWidomGSE::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void TracyWidomGSE::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
}

END_NAMESPACE_OPENTURNS
