//                                               -*- C++ -*-
/**
 * @file  PythonDistributionImplementation.hxx
 * @brief This class binds a Python object to an Open TURNS' Distribution
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
 * @author schueller
 * @date   2012-07-16 15:59:45 +0200 (Mon, 16 Jul 2012)
 */

#ifndef OPENTURNS_PYTHONDISTRIBUTIONIMPLEMENTATION_HXX
#define OPENTURNS_PYTHONDISTRIBUTIONIMPLEMENTATION_HXX

#include "Python.h"
#include "DistributionImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS


/**
 * @class PythonDistributionImplementation
 *
 * This class binds a Python object to an Open TURNS' RandomVector
 */
class PythonDistributionImplementation
  : public DistributionImplementation
{
  CLASSNAME;
public:


  /** Constructor from Python object*/
  PythonDistributionImplementation(PyObject * pyObject);

  /** Copy constructor */
  PythonDistributionImplementation(const PythonDistributionImplementation & other);

  /** Virtual constructor */
  virtual PythonDistributionImplementation * clone() const;

  /** Destructor */
  virtual ~PythonDistributionImplementation();

  /** Comparison operator */
  Bool operator ==(const PythonDistributionImplementation & other) const;

  /** String converter */
  virtual String __repr__() const;
  virtual String __str__(const String & offset) const;


  /* Here is the interface that all derived class must implement */

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get a numerical sample whose elements follow the distributionImplementation */
  NumericalSample getSample(const UnsignedLong size) const;

  /** Get the DDF of the distribution */
  NumericalPoint computeDDF(const NumericalPoint & point) const;

  /** Get the PDF of the distribution */
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the complementary CDF of the distribution */
  NumericalScalar computeComplementaryCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the quantile of the distribution */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;

  /** Get the roughness, i.e. the L2-norm of the PDF */
  NumericalScalar getRoughness() const;

  /** Get the mean of the distribution */
  NumericalPoint getMean() const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the distribution */
  NumericalPoint getMoment(const UnsignedLong n) const;

  /** Get the centered moments of the distribution */
  NumericalPoint getCenteredMoment(const UnsignedLong n) const;

  /** Parameters value and description accessor */
  //   NumericalPointWithDescriptionCollection getParametersCollection() const;
  //   void setParametersCollection(const NumericalPointCollection & parametersCollection);
  //

  /** Check if the distribution is elliptical */
  Bool isElliptical() const;

  /** Check if the distribution is continuous */
  Bool isContinuous() const;

  /** Check if the distribution is integral */
  Bool isIntegral() const;

  /** Tell if the distribution has elliptical copula */
  Bool hasEllipticalCopula() const;

  /** Tell if the distribution has independent copula */
  Bool hasIndependentCopula() const;

  /** Method save() stores the object through the StorageManager */
  virtual void save(Advocate & adv) const;

  /** Method save() reloads the object from the StorageManager */
  virtual void load(Advocate & adv);

protected:


private:

  friend class Factory<PythonDistributionImplementation>;

  /** Default constructor */
  PythonDistributionImplementation();

  /** The underlying Python object */
  PyObject * pyObj_;

}; /* class PythonDistributionImplementation */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_PYTHONDISTRIBUTIONIMPLEMENTATION_HXX */
