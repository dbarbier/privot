//                                               -*- C++ -*-
/**
 *  @file  Student.cxx
 *  @brief The Student distribution
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 */
#include <cstdlib>
#include <cmath>

#include "Student.hxx"
#include "Distribution.hxx"
#include "IdentityMatrix.hxx"
#include "RandomGenerator.hxx"
#include "SpecFunc.hxx"
#include "DistFunc.hxx"
#include "Log.hxx"
#include "OSS.hxx"
#include "Mvtdstpack.hxx"
#include "Tvpack.hxx"
#include "PersistentObjectFactory.hxx"
#include "NumericalPoint.hxx"
#include "Exception.hxx"

BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(Student);

static Factory<Student> RegisteredFactory("Student");

/* Default constructor */
Student::Student(const NumericalScalar nu,
                 const UnsignedLong dimension)
  : EllipticalDistribution(NumericalPoint(dimension, 0.0),
                           NumericalPoint(dimension, 1.0),
                           IdentityMatrix(dimension), -1.0, "Student")
  , nu_(0.0)
  , studentNormalizationFactor_(0.0)
{
  setDimension( dimension );
  // This call set also the range
  setNu(nu);
}

/* Parameters constructor */
Student::Student(const NumericalScalar nu,
                 const NumericalScalar mu,
                 const NumericalScalar sigma)
  : EllipticalDistribution(NumericalPoint(1, mu), NumericalPoint(1, sigma), IdentityMatrix(1), -1.0, "Student")
  , nu_(0.0)
  , studentNormalizationFactor_(0.0)
{
  setDimension(1);
  // Set nu with checks. This call set also the range.
  setNu(nu);
}

/* Parameters constructor */
Student::Student(const NumericalScalar nu,
                 const NumericalPoint & mu,
                 const NumericalPoint & sigma,
                 const CorrelationMatrix & R)
  : EllipticalDistribution(mu, sigma, R, -1.0, "Student")
  , nu_(0.0)
  , studentNormalizationFactor_(0.0)
{
  setDimension(mu.getDimension());
  // Set nu with checks. This call set also the range.
  setNu(nu);
}

/* Comparison operator */
Bool Student::operator ==(const Student & other) const
{
  if (this == &other) return true;
  return EllipticalDistribution::operator==(other) && (nu_ == other.nu_);
}

/* String converter */
String Student::__repr__() const
{
  OSS oss;
  oss << "class=" << Student::GetClassName()
      << " name=" << getName()
      << " dimension=" << getDimension()
      << " nu=" << nu_
      << " mean=" << mean_
      << " sigma=" << sigma_
      << " correlationMatrix=" << R_;
  return oss;
}

String Student::__str__(const String & offset) const
{
  OSS oss;
  oss << offset << getClassName();
  if (getDimension() == 1) oss << "(nu = " << nu_ << ", mu = " << getMean()[0] << ", sigma = " << getSigma()[0] << ")";
  else oss << "(nu = " << nu_ << ", mu = " << getMean().__str__() << ", sigma = " << getSigma().__str__() << ", R = " << getCorrelation().__str__(offset) << ")";
  return oss;
}

/* Compute the density generator of the elliptical generator, i.e.
 *  the function phi such that the density of the distribution can
 *  be written as p(x) = phi(t(x-mu)S^(-1)(x-mu))                      */
NumericalScalar Student::computeDensityGenerator(const NumericalScalar betaSquare) const
{
  return exp(studentNormalizationFactor_ - 0.5 * (nu_ + getDimension()) * log1p(betaSquare / nu_));
}

/* Compute the derivative of the density generator */
NumericalScalar Student::computeDensityGeneratorDerivative(const NumericalScalar betaSquare) const
{
  const NumericalScalar iNu(1.0 / nu_);
  const UnsignedLong dimension(getDimension());
  return -0.5 * exp(studentNormalizationFactor_ - (0.5 * (nu_ + dimension) + 1.0) * log1p(betaSquare * iNu)) * (1.0 + dimension * iNu);
}

/* Compute the second derivative of the density generator */
NumericalScalar Student::computeDensityGeneratorSecondDerivative(const NumericalScalar betaSquare) const
{
  const NumericalScalar iNu(1.0 / nu_);
  const UnsignedLong dimension(getDimension());
  return 0.25 * exp(studentNormalizationFactor_ - (0.5 * (nu_ + dimension) + 2.0) * log1p(betaSquare * iNu)) * (1.0 + dimension * iNu) * (1.0 + (dimension + 2.0) * iNu);
}


/* Virtual constructor */
Student * Student::clone() const
{
  return new Student(*this);
}

/* Get one realization of the distribution */
NumericalPoint Student::getRealization() const
{
  const UnsignedLong dimension(getDimension());
  if (dimension == 1) return NumericalPoint(1, mean_[0] + sigma_[0] * DistFunc::rStudent(nu_));
  NumericalPoint value(dimension);
  // First, a realization of independant standard normal coordinates
  for (UnsignedLong i = 0; i < dimension; ++i) value[i] = DistFunc::rNormal();
  return sqrt(0.5 * nu_ / DistFunc::rGamma(0.5 * nu_)) * (cholesky_ * value) + mean_;
}


/* Get the CDF of the distribution */
NumericalScalar Student::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point has a dimension incompatible with the distribution.";
  // Special case for dimension 1
  if (dimension == 1) return DistFunc::pStudent(nu_, (point[0] - mean_[0]) / sigma_[0]);
  // Normalize the point to use the standard form of the multivariate student distribution
  NumericalPoint u(normalize(point));
  // For the bidimensional case, use specialized high precision routine for integral degrees of freedom
  if ((dimension == 2) && (nu_ == round(nu_)))
    {
      int nu(static_cast<int>(round(nu_)));
      double r(R_(0, 1));
      return BVTL_F77(&nu, &(u[0]), &(u[1]), &r);
    }
  // For the tridimensional case, use specialized high precision routine for integral degrees of freedom
  if ((dimension == 3) && (nu_ == round(nu_)))
    {
      int nu(static_cast<int>(round(nu_)));
      double r[3];
      r[0] = R_(1, 0);
      r[1] = R_(2, 0);
      r[2] = R_(2, 1);
      double eps(ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ) * ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ));
      cdfEpsilon_ = eps;
      return TVTL_F77(&nu, &(u[0]), &r[0], &eps);
    }
  // For moderate dimension, use a Gauss-Legendre integration
  if (dimension <= ResourceMap::GetAsUnsignedLong("Student-SmallDimension"))
    {
      // Reduce the default integration point number for CDF computation in the range 3 < dimension <= Student-SmallDimension
      const UnsignedLong maximumNumber(static_cast< UnsignedLong > (round(pow(ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ), 1.0 / getDimension()))));
      const UnsignedLong candidateNumber(ResourceMap::GetAsUnsignedLong( "Student-MarginalIntegrationNodesNumber" ));
      if (candidateNumber > maximumNumber) LOGWARN(OSS() << "Warning! The requested number of marginal integration nodes=" << candidateNumber << " would lead to an excessive number of PDF evaluations. It has been reduced to " << maximumNumber << ". You should increase the ResourceMap key \"Student-MaximumNumberOfPoints\"");
      setIntegrationNodesNumber(std::min(maximumNumber, candidateNumber));
      return ContinuousDistribution::computeCDF(point);
    }
  // For larger dimension, use an adaptive integration
  if ((dimension <= 500) && (nu_ == round(nu_)))
    {
      int nu(static_cast<int>(round(nu_)));
      NumericalPoint lower(dimension, 0.0);
      std::vector<int> infin(dimension, 0);
      NumericalPoint correl(dimension * (dimension - 1) / 2, 0.0);
      /* Copy the correlation matrix in the proper format for mvndst */
      for (UnsignedLong i = 0; i < dimension; ++i)
        for (UnsignedLong j = 0; j < i; ++j) correl[j + i * (i - 1) / 2] = R_(j, i);
      // Non-centrality parameters
      NumericalPoint delta(dimension, 0.0);
      int maxpts(ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
      // Use only relative precision
      double abseps(0.0);
      // Reduce the precision according to the dimension. It ranges from ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ) for dimension=4 to ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) for dimension=500
      double releps(ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ) * pow(ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) / ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ), (4.0 + dimension) / 496.0));
      double error;
      double value;
      int inform;
      int dim(static_cast<UnsignedLong>( dimension ));
      do
        {
          MVTDST_F77(&dim, &nu, &lower[0], &u[0], &infin[0], &correl[0], &delta[0], &maxpts, &abseps, &releps, &error, &value, &inform);
          if (inform == 1)
            {
              LOGWARN(OSS() << "Warning, in Student::computeCDF(), the required precision has not been achieved with maxpts=" << NumericalScalar(maxpts) << ", we only got an absolute error of " << error << " and a relative error of " << error / value << ". We retry with maxpoint=" << 10 * maxpts);
              maxpts *= 10;
            }
          else if (inform != 0) throw InternalException(HERE) << "MVTDST: error code=" << inform;
        }
      while ((static_cast<UnsignedLong>(maxpts) <= ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" )) && (inform == 1));
      if (inform == 1) LOGWARN(OSS() << "Warning, in Student::computeCDF(), the required precision has not been achieved with maxpts=" << NumericalScalar(maxpts) << ", we only got an absolute error of " << error << " and a relative error of " << error / value << ". No more retry.");
      if ((value < 0.0) || (value > 1.0)) LOGWARN(OSS() << "Warning, in Student::computeCDF(), got a value outside of [0, 1], value=" << value << " your dependence structure might be too complex. The value will be truncated.");
      cdfEpsilon_ = error;
      return std::min(std::max(value, 0.0), 1.0);
    }
  // For very large dimension, use a MonteCarlo algorithm
  LOGWARN(OSS() << "Warning, in Student::computeCDF(), the dimension is very high. We will use a Monte Carlo method for the computation with a relative precision of 0.1% at 99% confidence level and a maximum of " << 10.0 * ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ) << " realizations. Expect a long running time and a poor accuracy for small values of the CDF...");
  NumericalScalar value(0.0);
  NumericalScalar variance(0.0);
  NumericalScalar a99(DistFunc::qNormal(0.995));
  UnsignedLong outerMax(10 * ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ) / ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
  NumericalScalar precision(0.0);
  for (UnsignedLong indexOuter = 0; indexOuter < outerMax; ++indexOuter)
    {
      NumericalScalar valueBlock(0.0);
      NumericalScalar varianceBlock(0.0);
      for (UnsignedLong indexSample = 0; indexSample < ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ); ++indexSample)
        {
          Bool inside(true);
          NumericalPoint realization(getRealization());
          // Check if the realization is in the integration domain
          for (UnsignedLong i = 0; i < dimension; ++i)
            {
              inside = realization[i] <= point[i];
              if (!inside) break;
            }
          // ind value is 1.0 if the realization is inside of the integration domain, 0.0 else.
          NumericalScalar ind(inside);
          NumericalScalar norm(1.0 / (indexSample + 1.0));
          varianceBlock = (varianceBlock * indexSample + (1.0 - norm) * (valueBlock - ind) * (valueBlock - ind)) * norm;
          valueBlock = (valueBlock * indexSample + ind) * norm;
        }
      NumericalScalar norm(1.0 / (indexOuter + 1.0));
      variance = (varianceBlock + indexOuter * variance + (1.0 - norm) * (value - valueBlock) * (value - valueBlock)) * norm;
      value = (value * indexOuter + valueBlock) * norm;
      // Quick return for value = 1
      const NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
      if ((value >= 1.0 - quantileEpsilon) && (variance == 0.0)) return 1.0;
      precision = a99 * sqrt(variance / (indexOuter + 1.0) / ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
      if (precision < ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) * value) return value;
      // 0.1 * ((1000 * indexOuter) / outerMax) is to print percents with one figure after the decimal point
      LOGINFO(OSS() << 0.1 * ((1000 * indexOuter) / outerMax) << "% value=" << value << " absolute precision(99%)=" << precision << " relative precision(99%)=" << ((value > 0.0) ? precision / value : -1.0));
    }
  cdfEpsilon_ = precision;
  return value;
} // computeCDF

/* Get the CDF of the distribution */
NumericalScalar Student::computeComplementaryCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  // Special case for dimension 1
  if (dimension == 1) return DistFunc::pStudent(nu_, (point[0] - mean_[0]) / sigma_[0], true);
  // Other cases do not depend on tail flag
  return computeCDF(point);
}

/* Compute the probability content of an interval */
NumericalScalar Student::computeProbability(const Interval & interval) const
{
  const UnsignedLong dimension(getDimension());
  if (interval.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given interval must have dimension=" << dimension << ", here dimension=" << interval.getDimension();

  if (interval.isNumericallyEmpty()) return 0.0;
  // The generic implementation provided by the DistributionImplementation upper class is more accurate than the generic implementation provided by the ContinuousDistribution upper class for dimension = 1
  if (dimension == 1) return DistributionImplementation::computeProbability(interval);
  // Decompose and normalize the interval
  NumericalPoint lower(normalize(interval.getLowerBound()));
  NumericalPoint upper(normalize(interval.getUpperBound()));
  const Interval::BoolCollection finiteLower(interval.getFiniteLowerBound());
  const Interval::BoolCollection finiteUpper(interval.getFiniteUpperBound());
  /* General case */
  // For moderate dimension, use a Gauss-Legendre integration
  if (dimension <= ResourceMap::GetAsUnsignedLong("Student-SmallDimension"))
    {
      // Reduce the default integration point number for CDF computation in the range 3 < dimension <= Student-SmallDimension
      const UnsignedLong maximumNumber(static_cast< UnsignedLong > (round(pow(ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ), 1.0 / getDimension()))));
      const UnsignedLong candidateNumber(ResourceMap::GetAsUnsignedLong( "Student-MarginalIntegrationNodesNumber" ));
      if (candidateNumber > maximumNumber) LOGWARN(OSS() << "Warning! The requested number of marginal integration nodes=" << candidateNumber << " would lead to an excessive number of PDF evaluations. It has been reduced to " << maximumNumber << ". You should increase the ResourceMap key \"Student-MaximumNumberOfPoints\"");
      setIntegrationNodesNumber(std::min(maximumNumber, candidateNumber));
      return ContinuousDistribution::computeProbability(interval);
    }
  // For large dimension, use an adaptive integration
  if (dimension <= 500)
    {
      int nu(static_cast<UnsignedLong>(round(nu_)));
      // Build finite/infinite flags according to MVNDST documentation
      std::vector<int> infin(dimension, -1);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          // Infin[i] should be:
          //  2 if finiteLower[i] && finiteUpper[i]
          //  1 if finiteLower[i] && !finiteUpper[i]
          //  0 if !finiteLower[i] && finiteUpper[i]
          // -1 if !finiteLower[i] && !finiteUpper[i]
          infin[i] += 2 * int(finiteLower[i]) + int(finiteUpper[i]);
        }
      NumericalPoint correl(dimension * (dimension - 1) / 2, 0.0);
      /* Copy the correlation matrix in the proper format for mvndst */
      for (UnsignedLong i = 0; i < UnsignedLong(dimension); ++i)
        for (UnsignedLong j = 0; j < i; ++j) correl[j + i * (i - 1) / 2] = R_(j, i);
      // Non-centrality parameter
      NumericalPoint delta(dimension, 0.0);
      int maxpts(ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
      // Use only relative precision
      double abseps(0.0);
      // Reduce the precision according to the dimension. It ranges from ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ) for dimension=4 to ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) for dimension=500
      double releps(ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ) * pow(ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) / ResourceMap::GetAsNumericalScalar( "Student-MaximumCDFEpsilon" ), (4.0 + dimension) / 496.0));
      double error;
      double value;
      int inform;
      int dim = static_cast<UnsignedLong>( dimension );
      do
        {
          MVTDST_F77(&dim, &nu, &lower[0], &upper[0], &infin[0], &correl[0], &delta[0], &maxpts, &abseps, &releps, &error, &value, &inform);
          if (inform == 1)
            {
              LOGWARN(OSS() << "Warning, in Student::computeProbability(), the required precision has not been achieved with maxpts=" << NumericalScalar(maxpts) << ", we only got an absolute error of " << error << " and a relative error of " << error / value << ". We retry with maxpoint=" << 10 * maxpts);
              maxpts *= 10;
            }
          else if (inform != 0) throw InternalException(HERE) << "MVTDST: error code=" << inform;
        }
      while ((static_cast<UnsignedLong>(maxpts) <= ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" )) && (inform == 1));
      if (inform == 1) LOGWARN(OSS() << "Warning, in Student::computeProbability(), the required precision has not been achieved with maxpts=" << NumericalScalar(maxpts) << ", we only got an absolute error of " << error << " and a relative error of " << error / value << ". No more retry.");
      if ((value < 0.0) || (value > 1.0)) LOGWARN(OSS() << "Warning, in Student::computeProbability(), got a value outside of [0, 1], value=" << value << " your dependence structure might be too complex. The value will be truncated.");
      return std::min(std::max(value, 0.0), 1.0);
    }
  // For very large dimension, use a MonteCarlo algorithm
  LOGWARN(OSS() << "Warning, in Student::computeProbability(), the dimension is very high. We will use a Monte Carlo method for the computation with a relative precision of 0.1% at 99% confidence level and a maximum of " << 10.0 * ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ) << " realizations. Expect a long running time and a poor accuracy for low values of the CDF...");
  RandomGeneratorState initialState(RandomGenerator::GetState());
  RandomGenerator::SetSeed(ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
  NumericalScalar value(0.0);
  NumericalScalar variance(0.0);
  const NumericalScalar a99(DistFunc::qNormal(0.995));
  UnsignedLong outerMax(10 * ResourceMap::GetAsUnsignedLong( "Student-MaximumNumberOfPoints" ) / ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
  NumericalScalar precision(0.0);
  for (UnsignedLong indexOuter = 0; indexOuter < outerMax; ++indexOuter)
    {
      NumericalScalar valueBlock(0.0);
      NumericalScalar varianceBlock(0.0);
      for (UnsignedLong indexSample = 0; indexSample < ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ); ++indexSample)
        {
          // ind value is 1.0 if the realization is inside of the integration domain, 0.0 else.
          NumericalScalar ind(interval.numericallyContains(getRealization()));
          NumericalScalar norm(1.0 / (indexSample + 1.0));
          varianceBlock = (varianceBlock * indexSample + (1.0 - norm) * (valueBlock - ind) * (valueBlock - ind)) * norm;
          valueBlock = (valueBlock * indexSample + ind) * norm;
        }
      NumericalScalar norm(1.0 / (indexOuter + 1.0));
      variance = (varianceBlock + indexOuter * variance + (1.0 - norm) * (value - valueBlock) * (value - valueBlock)) * norm;
      value = (value * indexOuter + valueBlock) * norm;
      // Quick return for value = 1
      const NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
      if ((value >= 1.0 - quantileEpsilon) && (variance == 0.0)) return 1.0;
      precision = a99 * sqrt(variance / (indexOuter + 1.0) / ResourceMap::GetAsUnsignedLong( "Student-MinimumNumberOfPoints" ));
      if (precision < ResourceMap::GetAsNumericalScalar( "Student-MinimumCDFEpsilon" ) * value) return value;
      // 0.1 * ((1000 * indexOuter) / outerMax) is to print percents with one figure after the decimal point
      LOGINFO(OSS() << 0.1 * ((1000 * indexOuter) / outerMax) << "% value=" << value << " absolute precision(99%)=" << precision << " relative precision(99%)=" << ((value > 0.0) ? precision / value : -1.0));
    }
  RandomGenerator::SetState(initialState);
  return value;
}

/* Get the PDFGradient of the distribution */
NumericalPoint Student::computePDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());

  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  NumericalPoint pdfGradient(2 * dimension + 1, 0.0);
  if (dimension == 1)
    {
      const NumericalPoint ellipticalPDFGradient(EllipticalDistribution::computePDFGradient(point));
      const NumericalScalar epsNu(1e-3);
      pdfGradient[0] = (Student(nu_ + epsNu, mean_, sigma_, R_).computePDF(point) - Student(nu_ - epsNu, mean_, sigma_, R_).computePDF(point)) / (2.0 * epsNu);
      for (UnsignedLong i = 0; i < 2 * dimension; ++i) pdfGradient[i + 1] = ellipticalPDFGradient[i];
      return pdfGradient;
    }
  else throw NotYetImplementedException(HERE);
}

/* Get the CDFGradient of the distribution */
NumericalPoint Student::computeCDFGradient(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());

  if (point.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the given point must have dimension=" << dimension << ", here dimension=" << point.getDimension();

  if (dimension == 1)
    {
      NumericalPoint cdfGradient(3, 0.0);
      const NumericalScalar x(point[0] - mean_[0]);
      const NumericalScalar eps(pow(ResourceMap::GetAsNumericalScalar("DistFunc-Precision"), 1.0 / 3.0));
      const NumericalScalar i2Eps(0.5 / eps);
      cdfGradient[0] = (DistFunc::pStudent(nu_ + eps, x / sigma_[0]) - DistFunc::pStudent(nu_ - eps, x / sigma_[0])) * i2Eps;
      // Opposite sign for eps because x - eps = point[0] - (mu + eps)
      cdfGradient[1] = (DistFunc::pStudent(nu_, (x - eps) / sigma_[0]) - DistFunc::pStudent(nu_, (x + eps) / sigma_[0])) * i2Eps;
      cdfGradient[2] = (DistFunc::pStudent(nu_, x / (sigma_[0] + eps)) - DistFunc::pStudent(nu_, x / (sigma_[0] - eps))) * i2Eps;
      return cdfGradient;
    }
  else throw NotYetImplementedException(HERE);
}

/* Compute the PDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1)
   For Student distributions, the conditional distribution is no more Student. See:
   Samuel Kotz, Saralees Nadarajah, "Multivariate t Distributions and Their Applications", Cambridge University Press, 2001.
*/
NumericalScalar Student::computeConditionalPDF(const NumericalScalar x,
                                               const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional PDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning
  if (conditioningDimension == 0) return Distribution(getMarginal(conditioningDimension)).computePDF(x);
  // General case
  throw NotYetImplementedException(HERE);
  NumericalScalar meanRos(0.0);
  const NumericalScalar sigmaRos(1.0 / inverseCholesky_(conditioningDimension, conditioningDimension));
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) meanRos += inverseCholesky_(conditioningDimension, i) / sqrt(sigma_[i]) * (y[i] - mean_[i]);
  meanRos = mean_[conditioningDimension] - sigmaRos * sqrt(sigma_[conditioningDimension]) * meanRos;
  return exp(-0.5 * pow(x - meanRos, 2.0) / (sigmaRos * sigmaRos)) / (sigmaRos * sqrt(2.0 * M_PI));
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Student::computeConditionalCDF(const NumericalScalar x,
                                               const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning
  if (conditioningDimension == 0) return Distribution(getMarginal(conditioningDimension)).computeCDF(x);
  // General case
  throw NotYetImplementedException(HERE);
  NumericalScalar meanRos(0.0);
  const NumericalScalar sigmaRos(1.0 / inverseCholesky_(conditioningDimension, conditioningDimension));
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) meanRos += inverseCholesky_(conditioningDimension, i) / sqrt(sigma_[i]) * (y[i] - mean_[i]);
  meanRos = mean_[conditioningDimension] - sigmaRos * sqrt(sigma_[conditioningDimension]) * meanRos;
  return DistFunc::pNormal((x - meanRos) / sigmaRos);
}

/* Compute the quantile of Xi | X1, ..., Xi-1, i.e. x such that CDF(x|y) = q with x = Xi, y = (X1,...,Xi-1) */
NumericalScalar Student::computeConditionalQuantile(const NumericalScalar q,
                                                    const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile with a conditioning point of dimension greater or equal to the distribution dimension.";
  if ((q < 0.0) || (q > 1.0)) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional quantile for a probability level outside of [0, 1]";
  // Special case when no contitioning
  if (conditioningDimension == 0) return mean_[conditioningDimension] + sigma_[conditioningDimension] * DistFunc::qStudent(nu_, q);
  // General case
  throw NotYetImplementedException(HERE);
  NumericalScalar meanRos(0.0);
  const NumericalScalar sigmaRos(1.0 / inverseCholesky_(conditioningDimension, conditioningDimension));
  for (UnsignedLong i = 0; i < conditioningDimension; ++i) meanRos += inverseCholesky_(conditioningDimension, i) / sqrt(sigma_[i]) * (y[i] - mean_[i]);
  meanRos = mean_[conditioningDimension] - sigmaRos * sqrt(sigma_[conditioningDimension]) * meanRos;
  if (q == 0.0) return meanRos - 0.5 * sigmaRos * sqrt(4.0 * (log(SpecFunc::ISQRT2PI / sigmaRos) - SpecFunc::LogMinNumericalScalar));
  if (q == 1.0) return meanRos + 0.5 * sigmaRos * sqrt(4.0 * (log(SpecFunc::ISQRT2PI / sigmaRos) - SpecFunc::LogMinNumericalScalar));
  return meanRos + sigmaRos * DistFunc::qNormal(q);
}

/* Get the i-th marginal distribution */
Student::Implementation Student::getMarginal(const UnsignedLong i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  // Special case for dimension 1
  if (getDimension() == 1) return clone();
  // General case
  const CorrelationMatrix R(1);
  const NumericalPoint sigma(1, sigma_[i]);
  const NumericalPoint mean(1, mean_[i]);
  return new Student(nu_, mean, sigma, R);
}

/* Get the distribution of the marginal distribution corresponding to indices dimensions */
Student::Implementation Student::getMarginal(const Indices & indices) const
{
  UnsignedLong dimension(getDimension());
  if (!indices.check(dimension - 1)) throw InvalidArgumentException(HERE) << "The indices of a marginal distribution must be in the range [0, dim-1] and  must be different";
  // Special case for dimension 1
  if (dimension == 1) return clone();
  // General case
  const UnsignedLong outputDimension(indices.getSize());
  CorrelationMatrix R(outputDimension);
  NumericalPoint sigma(outputDimension);
  NumericalPoint mean(outputDimension);
  // Extract the correlation matrix, the marginal standard deviations and means
  for (UnsignedLong i = 0; i < outputDimension; ++i)
    {
      const UnsignedLong index_i(indices[i]);
      sigma[i] = sigma_[index_i];
      mean[i] = mean_[index_i];
      for (UnsignedLong j = 0; j <= i; ++j) R(i, j) = R_(index_i, indices[j]);
    }
  return new Student(nu_, mean, sigma, R);
} // getMarginal(Indices)

/* Compute the radial distribution CDF */
NumericalScalar Student::computeRadialDistributionCDF(const NumericalScalar radius,
                                                      const Bool tail) const
{
  throw NotYetImplementedException(HERE);
}

/* Mu accessor */
void Student::setMu(const NumericalScalar mu)
{
  if (getDimension() == 1) mean_ = NumericalPoint(1, mu);
  computeRange();
}

NumericalScalar Student::getMu() const
{
  if (getDimension() == 1) return mean_[0];
  throw InvalidArgumentException(HERE) << "Error: cannot call this method if dimension > 1.";
}

/* Get the mean of the distribution */
NumericalPoint Student::getMean() const
{
  if (nu_ <= 1.0) throw NotDefinedException(HERE) << "Student mean is defined only for nu > 1, here nu=" << nu_;
  return EllipticalDistribution::getMean();
}

/* Get the standard deviation of the distribution */
NumericalPoint Student::getStandardDeviation() const
{
  if (nu_ <= 2.0) throw NotDefinedException(HERE) << "Student standard deviation is defined only for nu > 2, here nu=" << nu_;
  return EllipticalDistribution::getStandardDeviation();
}

/* Get the skewness of the distribution */
NumericalPoint Student::getSkewness() const
{
  if (nu_ <= 3.0) throw NotDefinedException(HERE) << "Student skewness is defined only for nu > 3, here nu=" << nu_;
  return NumericalPoint(getDimension(), 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint Student::getKurtosis() const
{
  if (nu_ <= 4.0) throw NotDefinedException(HERE) << "Student kurtosis is defined only for nu > 4, here nu=" << nu_;
  return NumericalPoint(getDimension(), 3.0 + 6.0 / (nu_ - 4.0));
}

/* Get the covariance of the distribution */
CovarianceMatrix Student::getCovariance() const
{
  if (nu_ <= 2.0) throw NotDefinedException(HERE) << "Student covariance is defined only for nu > 2, here nu=" << nu_;
  return EllipticalDistribution::getCovariance();
}

/* Get the moments of the standardized distribution */
NumericalPoint Student::getStandardMoment(const UnsignedLong n) const
{
  if (n >= nu_) throw NotDefinedException(HERE) << "Error: cannot compute a standard moment of order greater or equal to the number of degrees of freedom";
  if (n % 2 == 1) return NumericalPoint(1, 0.0);
  NumericalScalar moment(1.0);
  for (UnsignedLong i = 0; i < n / 2; ++i) moment *= (nu_ * (2 * i + 1)) / (nu_ - 2 * (i + 1));
  return NumericalPoint(1, moment);
}

/* Get the standard representative in the parametric family, associated with the standard moments */
Student::Implementation Student::getStandardRepresentative() const
{
  return Student(nu_).clone();
}

/* Parameters value and description accessor */
Student::NumericalPointWithDescriptionCollection Student::getParametersCollection() const
{
  // First, get the parameters of the underlying elliptical distribution, it means mu, sigma and R
  NumericalPointWithDescriptionCollection parameters(EllipticalDistribution::getParametersCollection());
  // We get a collection of NumericalPointWithDescription, we append the value of nu at the beginning of each NumericalPointWithDescription
  const UnsignedLong size(parameters.getSize());
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPointWithDescription ellipticalParameterI(parameters[i]);
      const Description ellipticalDescriptionI(ellipticalParameterI.getDescription());
      const UnsignedLong ellipticalParameterIDimension(ellipticalParameterI.getDimension());
      NumericalPointWithDescription parameterI(ellipticalParameterIDimension + 1);
      Description descriptionI(ellipticalParameterIDimension + 1);
      parameterI[0] = nu_;
      descriptionI[0] = "nu";
      for (UnsignedLong j = 0; j < ellipticalParameterIDimension; ++j)
        {
          parameterI[j + 1] = ellipticalParameterI[j];
          descriptionI[j + 1] = ellipticalDescriptionI[j];
        }
      parameterI.setDescription(descriptionI);
      parameters[i] = parameterI;
    }
  return parameters;
}

void Student::setParametersCollection(const NumericalPointCollection & parametersCollection)
{
  const UnsignedLong size(parametersCollection.getSize());
  const UnsignedLong dimension(size > 1 ? size - 1 : size);
  if (dimension == 1) *this = Student(parametersCollection[0][0], parametersCollection[0][1], parametersCollection[0][2]);
  else
    {
      const NumericalScalar nu(parametersCollection[0][0]);
      NumericalPoint mean(dimension);
      NumericalPoint sigma(dimension);
      CorrelationMatrix R(dimension);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          mean[i] = parametersCollection[i][1];
          sigma[i] = parametersCollection[i][2];
        }
      UnsignedLong parameterIndex(1);
      for (UnsignedLong i = 0; i < dimension; ++i)
        {
          for (UnsignedLong j = 0; j < i; ++j)
            {
              R(i, j) = parametersCollection[size - 1][parameterIndex];
              ++parameterIndex;
            }
        }
      *this = Student(nu, mean, sigma, R);
    }
}


/* Nu accessor */
void Student::setNu(const NumericalScalar nu)
{
  if (nu <= 2.0) LOGWARN(OSS() << "Warning! As nu <= 2, the covariance of the distribution will not be defined");
  const UnsignedLong dimension(getDimension());
  nu_ = nu;
  // Only set the covarianceScalingFactor if nu > 0, else its value is -1.0
  if (nu > 2.0) covarianceScalingFactor_ = nu_ / (nu_ - 2.0);
  studentNormalizationFactor_ = SpecFunc::LnGamma(0.5 * (nu + dimension)) - SpecFunc::LnGamma(0.5 * nu) - 0.5 * dimension * log(nu * M_PI);
  computeRange();
}

/* Tell if the distribution has independent copula */
Bool Student::hasIndependentCopula() const
{
  // A multivariate Student distribution never has an independent copula
  return getDimension() == 1;
}

/* Nu accessor */
NumericalScalar Student::getNu() const
{
  return nu_;
}

/* Quantile computation for dimension=1 */
NumericalScalar Student::computeScalarQuantile(const NumericalScalar prob,
                                               const Bool tail,
                                               const NumericalScalar precision) const
{
  return mean_[0] + sigma_[0] * DistFunc::qStudent(nu_, prob, tail);
}

/* Method save() stores the object through the StorageManager */
void Student::save(Advocate & adv) const
{
  EllipticalDistribution::save(adv);
  adv.saveAttribute( "nu_", nu_ );
  adv.saveAttribute( "studentNormalizationFactor_", studentNormalizationFactor_ );
}

/* Method load() reloads the object from the StorageManager */
void Student::load(Advocate & adv)
{
  EllipticalDistribution::load(adv);
  adv.loadAttribute( "nu_", nu_ );
  adv.loadAttribute( "studentNormalizationFactor_", studentNormalizationFactor_ );
  computeRange();
}




END_NAMESPACE_OPENTURNS
