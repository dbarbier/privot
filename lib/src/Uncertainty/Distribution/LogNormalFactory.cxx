//                                               -*- C++ -*-
/**
 *  @file  LogNormalFactory.cxx
 *  @brief Factory for LogNormal distribution
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 */
#include <cmath>
#include "LogNormalFactory.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "Brent.hxx"
#include "DistFunc.hxx"
#include "SpecFunc.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(LogNormalFactory);

/* Default constructor */
LogNormalFactory::LogNormalFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
LogNormalFactory * LogNormalFactory::clone() const
{
  return new LogNormalFactory(*this);
}


/* Here is the interface that all derived class must implement */

/* Algorithm associated with the method of moments */
LogNormal LogNormalFactory::buildMethodOfMoments(const NumericalSample & sample) const
{
  // ME
  const NumericalScalar std(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar skew(sample.computeSkewnessPerComponent()[0]);
  const NumericalScalar a3(skew);
  // V parameter, see "LogNormal Distributions, Theory and Applications", ISBN 0-8247-7803-0, p116
  const NumericalScalar v(cbrt(0.5 * (a3 + sqrt(4.0 + a3 * a3))) + cbrt(0.5 * (a3 - sqrt(4.0 + a3 * a3))));
  const NumericalScalar omega(1.0 + v * v);
  const NumericalScalar sigma(sqrt(log(omega)));
  const NumericalScalar beta(std / sqrt(omega * (omega - 1.0)));
  const NumericalScalar mu(log(beta));
  const NumericalScalar mean(sample.computeMean()[0]);
  const NumericalScalar gamma(mean - beta * sqrt(omega));
  return LogNormal(mu, sigma, gamma);
}

struct LogNormalFactoryLMLEParameterConstraint
{
  /** Constructor from a sample and a derivative factor estimate */
  LogNormalFactoryLMLEParameterConstraint(const NumericalSample & sample):
    sample_(sample),
    size_(sample.getSize())
  {
    // Nothing to do
  };

  NumericalPoint computeConstraint(const NumericalPoint & parameter) const
  {
    const NumericalPoint sums(computeMaximumLikelihoodSums(parameter[0]));
    return NumericalPoint(1, sums[0] * (sums[2] - sums[1] * (1.0 + sums[1] / size_)) + size_ * sums[3]);
  }

  /*
    S_0 = \sum_i (X_i - \gamma)^{-1}
    S_1 = \sum_i \log(X_i - \gamma)
    S_2 = \sum_i \log^2(X_i - \gamma)
    S_3 = \sum_i \log(X_i - \gamma) / (X_i - \gamma)
  */
  NumericalPoint computeMaximumLikelihoodSums(const NumericalScalar gamma) const
  {
    NumericalPoint sums(4, 0.0);
    for (UnsignedLong i = 0; i < size_; ++i)
      {
        const NumericalScalar delta(sample_[i][0] - gamma);
        const NumericalScalar logDelta(log(delta));
        const NumericalScalar inverseDelta(1.0 / delta);
        sums[0] += inverseDelta;
        sums[1] += logDelta;
        sums[2] += logDelta * logDelta;
        sums[3] += logDelta * inverseDelta;
      }
    return sums;
  }

  // The data
  NumericalSample sample_;
  UnsignedLong size_;
};

/* Algoritm associated with the method of local likelihood maximization */
LogNormal LogNormalFactory::buildMethodOfLocalLikelihoodMaximization(const NumericalSample & sample) const
{
  const NumericalScalar std(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  NumericalScalar step(std * sqrt(quantileEpsilon));
  const NumericalScalar xMin(sample.getMin()[0]);
  NumericalScalar right(xMin - quantileEpsilon);
  const LogNormalFactoryLMLEParameterConstraint constraint(sample);
  const NumericalMathFunction f(bindMethod<LogNormalFactoryLMLEParameterConstraint, NumericalPoint, NumericalPoint>(constraint, &LogNormalFactoryLMLEParameterConstraint::computeConstraint, 1, 1));
  NumericalScalar constraintRight(f(NumericalPoint(1, right))[0]);
  NumericalScalar left(right - step);
  NumericalScalar constraintLeft(f(NumericalPoint(1, right))[0]);
  // First, the bracketing interval. We should find a change of sign within [Xmin-sigma, Xmin], else use another estimator
  while ((constraintLeft < 0.0) == (constraintRight < 0.0) && (step < sqrt(SpecFunc::MaxNumericalScalar)))
    {
      right = left;
      constraintRight = constraintLeft;
      left -= step;
      constraintLeft = f(NumericalPoint(1, left))[0];
      step *= 2.0;
    }
  // If we are unable to bracket the gamma parameter
  if ((constraintLeft < 0.0) == (constraintRight < 0.0)) throw InternalException(HERE) << "Error: unable to bracket the gamma parameter. The local maximum likelihood estimator is not defined";
  // Second, the bisection
  // Solve the constraint equation
  const Brent solver(ResourceMap::GetAsNumericalScalar("LogNormalFactory-AbsolutePrecision"), ResourceMap::GetAsNumericalScalar("LogNormalFactory-RelativePrecision"), ResourceMap::GetAsNumericalScalar("LogNormalFactory-ResidualPrecision"), ResourceMap::GetAsUnsignedLong("LogNormalFactory-MaximumIteration"));
  // Gamma estimate
  const NumericalScalar gamma(solver.solve(f, 0.0, left, right, constraintLeft, constraintRight));
  // Third, the final estimates
  const UnsignedLong size(sample.getSize());
  const NumericalPoint sums(constraint.computeMaximumLikelihoodSums(gamma));
  const NumericalScalar mu(sums[1] / size);
  const NumericalScalar sigma2(sums[2] / size - mu * mu);
  if (sigma2 <= 0.0) throw InternalException(HERE) << "Error: the variance local maximum likelihood estimator should be positive, here sigma2=" << sigma2;
  return LogNormal(mu, sqrt(sigma2), gamma);
}

struct LogNormalFactoryMMEParameterConstraint
{
  /** Constructor from a sample and a derivative factor estimate */
  LogNormalFactoryMMEParameterConstraint(const UnsignedLong n,
                                         const NumericalScalar xMin,
                                         const NumericalScalar mean,
                                         const NumericalScalar std):
    eZ1_(DistFunc::eZ1(n)),
    alpha_(pow(std / (mean - xMin), 2))
  {
    // Nothing to do
  };

  NumericalPoint computeConstraint(const NumericalPoint & parameter) const
  {
    const NumericalScalar omega(parameter[0]);
    return NumericalPoint(1, alpha_ * pow(sqrt(omega) - exp(eZ1_ * sqrt(log(omega))), 2) - omega * (omega - 1.0));
  }

  NumericalScalar getEZ1() const
  {
    return eZ1_;
  }

  // Some intermediate coefficients
  NumericalScalar eZ1_;
  NumericalScalar alpha_;
};

/* Algorithm associated with the method of modified moments */
LogNormal LogNormalFactory::buildMethodOfModifiedMoments(const NumericalSample & sample) const
{
  const NumericalScalar mean(sample.computeMean()[0]);
  const NumericalScalar std(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar xMin(sample.getMin()[0]);
  const LogNormalFactoryMMEParameterConstraint constraint(sample.getSize(), xMin, mean, std);
  const NumericalMathFunction f(bindMethod<LogNormalFactoryMMEParameterConstraint, NumericalPoint, NumericalPoint>(constraint, &LogNormalFactoryMMEParameterConstraint::computeConstraint, 1, 1));
  // First, the bracketing interval
  NumericalScalar ea(1.0);
  NumericalScalar eb(2.0);
  NumericalScalar a(1.0 + ea);
  NumericalScalar b(1.0 + eb);
  NumericalScalar fA(f(NumericalPoint(1, a))[0]);
  NumericalScalar fB(f(NumericalPoint(1, b))[0]);
  // While f has the same sign at the two bounds, update the interval
  const NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  while ((fA * fB > 0.0) && (ea > quantileEpsilon))
    {
      ea = 0.5 * ea;
      a = 1.0 + ea;
      fA = f(NumericalPoint(1, a))[0];
      if (fA * fB <= 0.0) break;
      eb = 2.0 * eb;
      b = 1.0 + eb;
      fB = f(NumericalPoint(1, b))[0];
    }
  const NumericalScalar absolutePrecision(ResourceMap::GetAsNumericalScalar("LogNormalFactory-AbsolutePrecision"));
  if (a <= quantileEpsilon) throw InternalException(HERE) << "Error: the modified moment estimator is not defined";
  NumericalScalar omega(0.0);
  if (fabs(fA) < absolutePrecision) omega = a;
  else if (fabs(fB) < absolutePrecision) omega = b;
  else
    {
      const Brent solver(absolutePrecision, ResourceMap::GetAsNumericalScalar("LogNormalFactory-RelativePrecision"), ResourceMap::GetAsNumericalScalar("LogNormalFactory-ResidualPrecision"), ResourceMap::GetAsUnsignedLong("LogNormalFactory-MaximumIteration"));
      // Omega estimate
      omega = solver.solve(f, 0.0, a, b, fA, fB);
    }
  const NumericalScalar sigma(sqrt(log(omega)));
  const NumericalScalar eZ1(constraint.getEZ1());
  const NumericalScalar sqrtOmega(sqrt(omega));
  const NumericalScalar beta((mean - xMin) / (sqrtOmega - exp(eZ1 * sigma)));
  const NumericalScalar gamma(mean - beta * sqrtOmega);
  const NumericalScalar mu(log(beta));
  return LogNormal(mu, sigma, gamma);
}

LogNormalFactory::Implementation LogNormalFactory::build(const NumericalSample & sample,
                                                         const UnsignedLong method) const
{
  return buildAsLogNormal(sample, method).clone();
}

LogNormalFactory::Implementation LogNormalFactory::build() const
{
  return buildAsLogNormal().clone();
}

LogNormalFactory::Implementation LogNormalFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsLogNormal(parameters).clone();
}

LogNormal LogNormalFactory::buildAsLogNormal(const NumericalSample & sample,
                                             const UnsignedLong method) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a LogNormal distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a LogNormal distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  switch (method)
    {
    case 0:
      try
        {
          return buildMethodOfLocalLikelihoodMaximization(sample);
        }
      catch (InternalException & ex)
        {
          // We switch to the moment estimate
          LOGWARN(OSS() << "Warning! Unable to bracket the location parameter gamma. Using the modified moment estimator.");
          return buildAsLogNormal(sample, 1);
        }
      break;
    case 1:
      try
        {
          return buildMethodOfModifiedMoments(sample);
        }
      catch (InternalException & ex)
        {
          // We switch to the moment estimate
          LOGWARN(OSS() << "Warning! Unable to bracket the shape parameter sigma. Using the classical moment estimator.");
          return buildAsLogNormal(sample, 2);
        }
      break;
    case 2:
      return buildMethodOfMoments(sample);
      break;
    default:
      throw InternalException(HERE) << "Error: invalid value=" << method << " for the key 'LogNormalFactory-EstimationMethod' in ResourceMap";
    }
}

LogNormal LogNormalFactory::buildAsLogNormal(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsLogNormal(RemoveDescriptionFromCollection(parameters));
}

LogNormal LogNormalFactory::buildAsLogNormal(const NumericalPointCollection & parameters) const
{
  try
    {
      LogNormal distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a LogNormal distribution from the given parameters";
    }
}

LogNormal LogNormalFactory::buildAsLogNormal() const
{
  return LogNormal();
}

END_NAMESPACE_OPENTURNS
