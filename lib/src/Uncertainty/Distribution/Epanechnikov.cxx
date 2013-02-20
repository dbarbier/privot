//                                               -*- C++ -*-
/**
 *  @file  Epanechnikov.cxx
 *  @brief The Epanechnikov distribution
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
#include <cmath>
#include "Epanechnikov.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(Epanechnikov);

static Factory<Epanechnikov> RegisteredFactory("Epanechnikov");


/* Default constructor */
Epanechnikov::Epanechnikov()
  : NonEllipticalDistribution("Epanechnikov")
{
  setDimension(1);
  setRange(Interval(-1.0, 1.0));
}

/* Comparison operator */
Bool Epanechnikov::operator ==(const Epanechnikov & other) const
{
  return true;
}

/* String converter */
String Epanechnikov::__repr__() const
{
  OSS oss;
  oss << "class=" << Epanechnikov::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension();
  return oss;
}

String Epanechnikov::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName() << "()";
  return oss;
}

/* Virtual constructor */
Epanechnikov * Epanechnikov::clone() const
{
  return new Epanechnikov(*this);
}

/* Get the DDF of the distribution */
NumericalPoint Epanechnikov::computeDDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if ((x <= -1.0) || (x > 1.0)) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, -1.5 * x);
}


/* Get the PDF of the distribution */
NumericalScalar Epanechnikov::computePDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if ((x <= -1.0) || (x > 1.0)) return 0.0;
  return 0.75 * (1.0 + x) * (1.0 - x);
}


/* Get the CDF of the distribution */
NumericalScalar Epanechnikov::computeCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if (x <= -1.0) return 0.0;
  if (x > 1.0) return 1.0;
  return 0.5 + x * (0.75 - 0.25 * x * x);
}

NumericalScalar Epanechnikov::computeComplementaryCDF(const NumericalPoint & point) const
{
  NumericalScalar x(point[0]);
  if (x <= -1.0) return 1.0;
  if (x > 1.0) return 0.0;
  return 0.5 - x * (0.75 - 0.25 * x * x);
}

/** Get the PDFGradient of the distribution */
NumericalPoint Epanechnikov::computePDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0);
}

/** Get the CDFGradient of the distribution */
NumericalPoint Epanechnikov::computeCDFGradient(const NumericalPoint & point) const
{
  return NumericalPoint(0);
}

/* Get the quantile of the distribution */
NumericalScalar Epanechnikov::computeScalarQuantile(const NumericalScalar prob,
                                                    const Bool tail,
                                                    const NumericalScalar precision) const
{
  // 2.094395102393195492308429 = 2 * Pi / 3
  if (tail) return 2.0 * cos(0.3333333333333333333333333 * acos(2.0 * prob - 1.0) - 2.094395102393195492308429);
  return 2.0 * cos(0.3333333333333333333333333 * acos(1.0 - 2.0 * prob) - 2.094395102393195492308429);
}

/* Get the roughness, i.e. the L2-norm of the PDF */
NumericalScalar Epanechnikov::getRoughness() const
{
  return 0.6;
}

/* Compute the mean of the distribution */
void Epanechnikov::computeMean() const
{
  mean_ = NumericalPoint(1, 0.0);
  isAlreadyComputedMean_ = true;
}

/* Get the standard deviation of the distribution */
NumericalPoint Epanechnikov::getStandardDeviation() const
{
  // 0.4472135954999579392818348 = 1 / sqrt(5)
  return NumericalPoint(1, 0.4472135954999579392818348);
}

/* Get the skewness of the distribution */
NumericalPoint Epanechnikov::getSkewness() const
{
  return NumericalPoint(1, 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Epanechnikov::getKurtosis() const
{
  // 2.142857142857142857142857 = 15 / 7
  return NumericalPoint(1, 2.142857142857142857142857);
}

/* Get the moments of the standardized distribution */
NumericalPoint Epanechnikov::getStandardMoment(const UnsignedLong n) const
{
  if (n % 2 == 1) return NumericalPoint(1, 0.0);
  return NumericalPoint(1, 3.0 / (3.0 + n * (4.0 + n)));
}

/* Compute the covariance of the distribution */
void Epanechnikov::computeCovariance() const
{
  covariance_ = CovarianceMatrix(1);
  covariance_(0, 0) = 0.2;
  isAlreadyComputedCovariance_ = 0;
}

/* Parameters value and description accessor */
Epanechnikov::NumericalPointWithDescriptionCollection Epanechnikov::getParametersCollection() const
{
  // No parameter, return a collection with an empty NumericalPointWithDescription
  return NumericalPointWithDescriptionCollection(1, NumericalPointWithDescription(0));
}

/* Method save() stores the object through the StorageManager */
void Epanechnikov::save(Advocate & adv) const
{
  NonEllipticalDistribution::save(adv);
}

/* Method load() reloads the object from the StorageManager */
void Epanechnikov::load(Advocate & adv)
{
  NonEllipticalDistribution::load(adv);
}

END_NAMESPACE_OPENTURNS
