//                                               -*- C++ -*-
/**
 *  @file  NonCentralStudent.hxx
 *  @brief The NonCentralStudent distribution
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
 *  Id      NonCentralStudent.hxx 2392 2012-02-17 18:35:43Z schueller
 */
#ifndef OPENTURNS_NONCENTRALSTUDENT_HXX
#define OPENTURNS_NONCENTRALSTUDENT_HXX

#include "NonEllipticalDistribution.hxx"
#include "DistFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class NonCentralStudent
 *
 * The NonCentralStudent distribution.
 */
class NonCentralStudent
  : public NonEllipticalDistribution
{
  CLASSNAME;
public:

  typedef Pointer<DistributionImplementation> Implementation;

  /** Default constructor */
  NonCentralStudent(const NumericalScalar nu = 5.0,
                    const NumericalScalar delta = 0.0,
                    const NumericalScalar gamma = 0.0);

  /** Comparison operator */
  Bool operator ==(const NonCentralStudent & other) const;

  /** String converter */
  String __repr__() const;
  String __str__(const String & offset = "") const;



  /* Interface inherited from Distribution */

  /** Virtual constructor */
  NonCentralStudent * clone() const;

  /** Get one realization of the distribution */
  NumericalPoint getRealization() const;

  /** Get the PDF of the distribution */
  using NonEllipticalDistribution::computePDF;
  NumericalScalar computePDF(const NumericalPoint & point) const;

  /** Get the CDF of the distribution */
  using NonEllipticalDistribution::computeCDF;
  NumericalScalar computeCDF(const NumericalPoint & point) const;

  /** Get the PDFGradient of the distribution */
  NumericalPoint computePDFGradient(const NumericalPoint & point) const;

  /** Get the CDFGradient of the distribution */
  NumericalPoint computeCDFGradient(const NumericalPoint & point) const;

  /** Get the standard deviation of the distribution */
  NumericalPoint getStandardDeviation() const;

  /** Get the skewness of the distribution */
  NumericalPoint getSkewness() const;

  /** Get the kurtosis of the distribution */
  NumericalPoint getKurtosis() const;

  /** Get the raw moments of the standardized distribution */
  NumericalPoint getStandardMoment(const UnsignedLong n) const;

  /** Get the standard representative in the parametric family, associated with the standard moments */
  Implementation getStandardRepresentative() const;

  /** Parameters value and description accessor */
  NumericalPointWithDescriptionCollection getParametersCollection() const;
  using NonEllipticalDistribution::setParametersCollection;
  void setParametersCollection(const NumericalPointCollection & parametersCollection);

  /** Nu accessor */
  void setNu(const NumericalScalar nu);
  NumericalScalar getNu() const;

  /** Delta accessor */
  void setDelta(const NumericalScalar delta);
  NumericalScalar getDelta() const;

  /** Gamma accessor */
  void setGamma(const NumericalScalar gamma);
  NumericalScalar getGamma() const;

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:


private:
#ifdef qtl
  struct CDFWrapper
  {
    CDFWrapper(const NumericalScalar & nu, const NumericalScalar delta, const NumericalScalar gamma, const Bool tail):
      nu_(nu),
      delta_(delta),
      gamma_(gamma),
      tail_(tail){};

    NumericalPoint computeCDF(const NumericalPoint & point) const
    {
      return NumericalPoint(1, DistFunc::pNonCentralStudent(nu_, delta_, point[0] - gamma_, tail_));
    };

    const NumericalScalar nu_;
    const NumericalScalar delta_;
    const NumericalScalar gamma_;
    const NumericalScalar tail_;
  }; // struct CDFWrapper

  /** Get the quantile of the distribution, i.e the value Xp such that P(X <= Xp) = prob */
  NumericalScalar computeScalarQuantile(const NumericalScalar prob,
                                        const Bool tail = false,
                                        const NumericalScalar precision = ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon")) const;
#endif
  /** Compute the mean of the distribution */
  void computeMean() const;

  /** Compute the covariance of the distribution */
  void computeCovariance() const;

  /** Number of degrees of freedom */
  NumericalScalar nu_;

  /** Non-centrality parameter */
  NumericalScalar delta_;

  /** Location parameter */
  NumericalScalar gamma_;

}; /* class NonCentralStudent */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_NONCENTRALSTUDENT_HXX */
