//                                               -*- C++ -*-
/**
 *  @file  RiceFactory.cxx
 *  @brief Factory for Rice distribution
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
 *  @date   2007-05-10 16:43:31 +0200 (jeu. 10 mai 2007)
 */
#include "RiceFactory.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "SpecFunc.hxx"
#include "Brent.hxx"
#include "NumericalPoint.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(RiceFactory);

/* Default constructor */
RiceFactory::RiceFactory()
  : DistributionImplementationFactory()
{
  // Nothing to do
}

/* Virtual constructor */
RiceFactory * RiceFactory::clone() const
{
  return new RiceFactory(*this);
}

struct RiceFactoryParameterConstraint
{
  /** Constructor from a sample and a derivative factor estimate */
  RiceFactoryParameterConstraint(const NumericalScalar r):
    r2_(r * r)
  {
    // Nothing to do
  };

  NumericalPoint computeConstraint(const NumericalPoint & parameter) const
  {
    const NumericalScalar u(parameter[0]);
    const NumericalScalar relation(u - ((1.0 + r2_) * computeXi(u) - 2.0));
    return NumericalPoint(1, relation);
  }

  NumericalScalar computeXi(const NumericalScalar u) const
  {
    const NumericalScalar logI0(SpecFunc::LogBesselI0(0.25 * u));
    const NumericalScalar logI1(SpecFunc::LogBesselI1(0.25 * fabs(u)));

    return 2.0 + u - 0.125 * M_PI * exp(-0.5 * u + 2.0 * logI0) * pow(2.0 + u + fabs(u) * exp(logI1 - logI0), 2);
  }

  NumericalScalar r2_;
};

/* Here is the interface that all derived class must implement */

RiceFactory::Implementation RiceFactory::build(const NumericalSample & sample) const
{
  return buildAsRice(sample).clone();
}

RiceFactory::Implementation RiceFactory::build(const NumericalPointCollection & parameters) const
{
  return buildAsRice(parameters).clone();
}

RiceFactory::Implementation RiceFactory::build() const
{
  return buildAsRice().clone();
}

Rice RiceFactory::buildAsRice(const NumericalSample & sample) const
{
  const UnsignedLong size(sample.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot build a Rice distribution from an empty sample";
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a Rice distribution only from a sample of dimension 1, here dimension=" << sample.getDimension();
  const NumericalScalar mu(sample.computeMean()[0]);
  const NumericalScalar std(sample.computeStandardDeviationPerComponent()[0]);
  // Koay inversion method
  RiceFactoryParameterConstraint constraint(mu / std);
  const NumericalMathFunction f(bindMethod<RiceFactoryParameterConstraint, NumericalPoint, NumericalPoint>(constraint, &RiceFactoryParameterConstraint::computeConstraint, 1, 1));
  // Find a bracketing interval
  NumericalScalar a(1.0);
  NumericalScalar b(2.0);
  NumericalScalar fA(f(NumericalPoint(1, a))[0]);
  NumericalScalar fB(f(NumericalPoint(1, b))[0]);
  const NumericalScalar largeValue(sqrt(SpecFunc::MaxNumericalScalar));
  const UnsignedLong maximumIteration(ResourceMap::GetAsUnsignedLong( "RiceFactory-MaximumIteration" ));
  UnsignedLong iteration(0);
  // While f has the same sign at the two bounds, update the interval
  while ((fA * fB > 0.0) && (fabs(fA) < largeValue) && (fabs(fB) < largeValue) && (b < largeValue) && (iteration < maximumIteration))
    {
      a = 0.5 * a;
      fA = f(NumericalPoint(1, a))[0];
      if (fA * fB <= 0.0) break;
      b = 2.0 * b;
      fB = f(NumericalPoint(1, b))[0];
      LOGINFO(OSS() << "a=" << a << ", fa=" << fA << ", b=" << b << ", fb=" << fB);
      ++iteration;
    }
  if ((fabs(fA) > largeValue) || (fabs(fB) > largeValue) || (fabs(b) > largeValue) || (iteration == maximumIteration)) throw InvalidArgumentException(HERE) << "Error: cannot estimate parameters of a Rice distribution from the given sample";
  // Solve the constraint equation
  Brent solver(ResourceMap::GetAsNumericalScalar( "RiceFactory-AbsolutePrecision" ), ResourceMap::GetAsNumericalScalar( "RiceFactory-RelativePrecision" ), ResourceMap::GetAsNumericalScalar( "RiceFactory-ResidualPrecision" ), maximumIteration);
  // u estimate
  const NumericalScalar u(solver.solve(f, 0.0, a, b, fA, fB));
  const NumericalScalar xiU(constraint.computeXi(u));
  // Corresponding sigma estimate
  const NumericalScalar sigma(std / sqrt(xiU));
  // Corresponding nu estimate
  const NumericalScalar nu(sqrt(mu * mu + sigma * sigma * (xiU - 2.0)));
  try
    {
      return Rice(sigma, nu);
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot estimate parameters of a Rice distribution from the given sample";
    }
}

Rice RiceFactory::buildAsRice(const NumericalPointWithDescriptionCollection & parameters) const
{
  return buildAsRice(RemoveDescriptionFromCollection(parameters));
}

Rice RiceFactory::buildAsRice(const NumericalPointCollection & parameters) const
{
  try
    {
      Rice distribution;
      distribution.setParametersCollection(parameters);
      return distribution;
    }
  catch (InvalidArgumentException & ex)
    {
      throw InvalidArgumentException(HERE) << "Error: cannot build a Rice distribution from the given parameters";
    }
}

Rice RiceFactory::buildAsRice() const
{
  return Rice();
}

END_NAMESPACE_OPENTURNS
