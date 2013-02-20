//                                               -*- C++ -*-
/**
 *  @file  FrankCopulaFactory.cxx
 *  @brief Factory for FrankCopula distribution
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu, 10 mai 2007)
 */
#include <cstdlib>
#include <cmath>

#include "FrankCopulaFactory.hxx"
#include "SpecFunc.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(FrankCopulaFactory);

/* Default constructor */
FrankCopulaFactory::FrankCopulaFactory():
  DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
FrankCopulaFactory * FrankCopulaFactory::clone() const
{
  return new FrankCopulaFactory(*this);
}

/* Here is the interface that all derived class must implement */

FrankCopulaFactory::Implementation FrankCopulaFactory::build(const NumericalSample & sample) const
{
  return buildAsFrankCopula(sample).clone();
}

FrankCopulaFactory::Implementation FrankCopulaFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsFrankCopula(parameters).clone();
}

FrankCopulaFactory::Implementation FrankCopulaFactory::build() const
{
  return buildAsFrankCopula().clone();
}

FrankCopula FrankCopulaFactory::buildAsFrankCopula(const NumericalSample & sample) const
{
  if (sample.getSize() == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a FrankCopula distribution from an empty sample";
  if (sample.getDimension() != 2) throw InvalidArgumentException(HERE) << "Error: cannot build a FrankCopula distribution from a sample of dimension not equal to 2";
  NumericalScalar tau(sample.computeKendallTau().operator()(0, 1));
  if (tau == 1) throw InvalidArgumentException(HERE) << "Error: cannot build a FrankCopula distribution from a sample with Kendall tau equals to 1";
  // Search the value of the Frank copula parameter by numerically inverting the relation:
  // between Kendall's tau and Frank copula's parameter
  NumericalScalar theta(1.0);
  NumericalScalar step(0.5);
  NumericalScalar tauTheta(KendallTauFromParameter(theta));
  // Find a lower bound
  while (tauTheta > tau)
    {
      theta -= step;
      tauTheta = KendallTauFromParameter(theta);
      step *= 0.5;
    }
  // Here, tauTheta <= tau, hence theta is a lower bound of the parameter
  NumericalScalar minTheta(theta);
  // Now, look for an upper bound
  // If we started from a value of theta such that tauTheta > tau, theta + 2 * step is an upper bound
  NumericalScalar maxTheta(theta + 2.0 * step);
  // but if step = 0.5, it is because tauTheta was < tau for the initial choice of theta
  if (step < 0.5)
    {
      while (tauTheta <= tau)
        {
          theta += step;
          tauTheta = KendallTauFromParameter(theta);
          step *= 2.0;
        }
      // Here, we know that [theta - 0.5 * step, theta] is a bracketing interval
      minTheta = theta - 0.5 * step;
      maxTheta = theta;
    }
  // Bisection
  const NumericalScalar thetaEpsilon(ResourceMap::GetAsNumericalScalar( "FrankCopulaFactory-ThetaEpsilon" ));
  while (maxTheta - minTheta > thetaEpsilon)
    {
      theta = 0.5 * (maxTheta + minTheta);
      tauTheta = KendallTauFromParameter(theta);
      if (tauTheta <= tau) minTheta = theta;
      else maxTheta = theta;
    }
  return FrankCopula(theta);
}

FrankCopula FrankCopulaFactory::buildAsFrankCopula(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsFrankCopula(RemoveDescriptionFromCollection(parameters));
}

FrankCopula FrankCopulaFactory::buildAsFrankCopula(const NumericalPointCollection & parameters) const
{
  try
    {
      FrankCopula copula;
      copula.setParametersCollection(parameters);
      return copula;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a FrankCopula from the given parameters";
    }
}

FrankCopula FrankCopulaFactory::buildAsFrankCopula() const
{
  return FrankCopula();
}

// Compute Kendall's tau from Frank copula's parameter. It is an increasing function
NumericalScalar FrankCopulaFactory::KendallTauFromParameter(const NumericalScalar theta) const
{
  // Third order expansion for small theta, to avoid roundoff error for theta=0
  if (fabs(theta) < 8.75677785080733459909609107099e-4) return theta * (1.0 - 0.01 * theta * theta) / 9.0;
  return 1.0 - 4.0 * (1.0 - SpecFunc::Debye(theta, 1)) / theta;
}

END_NAMESPACE_OPENTURNS
