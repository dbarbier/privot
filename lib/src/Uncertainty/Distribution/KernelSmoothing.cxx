//                                               -*- C++ -*-
/**
 *  @file  KernelSmoothing.cxx
 *  @brief This class acts like a KernelMixture factory, implementing a
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
#include "KernelSmoothing.hxx"
#include "Normal.hxx"
#include "KernelMixture.hxx"
#include "TruncatedDistribution.hxx"
#include "PersistentObjectFactory.hxx"
#include "Brent.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "NumericalMathFunction.hxx"
#include "HermiteFactory.hxx"
#include "UniVariatePolynomial.hxx"
#include "SpecFunc.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

/**
 * @class KernelSmoothing
 *
 * The class describes the probabilistic concept of KernelSmoothing.
 */

CLASSNAMEINIT(KernelSmoothing);

static Factory<KernelSmoothing> RegisteredFactory("KernelSmoothing");

/* Default constructor */
KernelSmoothing::KernelSmoothing(const String & name)
  : PersistentObject(name)
  , bandwidth_(NumericalPoint(0))
  , kernel_(Normal())
{
  // Nothing to do
}

/* Default constructor */
KernelSmoothing::KernelSmoothing(const Distribution & kernel,
                                 const String & name)
  : PersistentObject(name)
  , bandwidth_(NumericalPoint(0))
  , kernel_(kernel)
{
  // Only 1D kernel allowed here
  if (kernel.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: only 1D kernel allowed for product kernel smoothing";
}

/* Virtual constructor */
KernelSmoothing * KernelSmoothing::clone() const
{
  return new KernelSmoothing(*this);
}

/* Compute the bandwidth according to Silverman's rule */
NumericalPoint KernelSmoothing::computeSilvermanBandwidth(const NumericalSample & sample)
{
  UnsignedLong dimension(sample.getDimension());
  UnsignedLong size(sample.getSize());
  NumericalPoint standardDeviations(sample.computeStandardDeviationPerComponent());
  // Silverman's Normal rule
  NumericalScalar factor(pow(size, -1.0 / (4.0 + dimension)) / kernel_.getStandardDeviation()[0]);
  // Scott's Normal rule
  return factor * standardDeviations;
}

struct PluginConstraint
{
  /** Constructor from a sample and a derivative factor estimate */
  PluginConstraint(const NumericalSample & sample,
                   const NumericalScalar K,
                   const UnsignedLong order):
    sample_(sample),
    N_(sample.getSize()),
    K_(K),
    order_(order),
    hermitePolynomial_(HermiteFactory().build(order))
  {
    // Normalize the polynomial
    hermitePolynomial_ = hermitePolynomial_ * (1.0 / hermitePolynomial_.getCoefficients()[order]);
  };

  /** Compute the derivative estimate based on the given bandwidth */
  NumericalScalar computePhi(const NumericalScalar h) const
  {
    // Quick return for odd order
    if (order_ % 2 == 1) return 0.0;
    NumericalScalar phi(N_ * hermitePolynomial_(0.0));
    const NumericalScalar cutOffPlugin(ResourceMap::GetAsNumericalScalar( "KernelSmoothing-CutOffPlugin" ));
    for (UnsignedLong i = 1; i < N_; ++i)
      {
        for (UnsignedLong j = 0; j < i; ++j)
          {
            const NumericalScalar dx(sample_[i][0] - sample_[j][0]);
            const NumericalScalar x(dx / h);
            // Clipping: if x is large enough, the exponential factor is 0.0
            if (fabs(x) < cutOffPlugin) phi += 2.0 * hermitePolynomial_(x) * exp(-0.5 * x * x);
          }
      }
    const NumericalScalar res(phi / ((N_ * (N_ - 1.0)) * pow(h, order_ + 1) * sqrt(2.0 * M_PI)));
    return res;
  }

  /** Compute the constraint for the plugin bandwidth */
  NumericalPoint computeBandwidthConstraint(const NumericalPoint & x) const
  {
    const NumericalScalar h(x[0]);
    const NumericalScalar gammaH(K_ * pow(h, 5.0 / 7.0));
    const NumericalScalar phiGammaH(computePhi(gammaH));
    const NumericalScalar res(h - pow(2.0 * sqrt(M_PI) * phiGammaH * N_, -1.0 / 5.0));
    return NumericalPoint(1, res);
  }

  NumericalSample sample_;
  UnsignedLong N_;
  NumericalScalar K_;
  UnsignedLong order_;
  UniVariatePolynomial hermitePolynomial_;
};

/* Compute the bandwidth according to the plugin rule
   See Vikas Chandrakant Raykar, Ramani Duraiswami, "Very Fast optimal bandwidth selection for univariate kernel density estimation" CS-TR-4774
   We implement only the basic estimator, not the fast version of it.
*/
NumericalPoint KernelSmoothing::computePluginBandwidth(const NumericalSample & sample)
{
  const UnsignedLong dimension(sample.getDimension());
  if (dimension != 1) throw InvalidArgumentException(HERE) << "Error: plugin bandwidth is available only for 1D sample";
  const UnsignedLong size(sample.getSize());
  // Approximate the derivatives by smoothing under the Normal assumption
  const NumericalScalar sd(sample.computeStandardDeviationPerComponent()[0]);
  const NumericalScalar phi6Normal(-15.0 / (16.0 * sqrt(M_PI)) * pow(sd, -7.0));
  const NumericalScalar phi8Normal(105.0 / (32.0 * sqrt(M_PI)) * pow(sd, -9.0));
  const NumericalScalar g1(pow(-6.0 / (sqrt(2.0 * M_PI) * phi6Normal * size), 1.0 / 7.0));
  const NumericalScalar g2(pow(30.0 / (sqrt(2.0 * M_PI) * phi8Normal * size), 1.0 / 9.0));
  const NumericalScalar phi4(PluginConstraint(sample, 1.0, 4).computePhi(g1));
  const NumericalScalar phi6(PluginConstraint(sample, 1.0, 6).computePhi(g2));
  const NumericalScalar K(pow(-6.0 * sqrt(2.0) * phi4 / phi6, 1.0 / 7.0));
  PluginConstraint constraint(sample, K, 4);
  const NumericalMathFunction f(bindMethod<PluginConstraint, NumericalPoint, NumericalPoint>(constraint, &PluginConstraint::computeBandwidthConstraint, 1, 1));
  // Find a bracketing interval
  NumericalScalar a(g1);
  NumericalScalar b(g2);
  NumericalScalar fA(f(NumericalPoint(1, a))[0]);
  NumericalScalar fB(f(NumericalPoint(1, b))[0]);
  // While f has the same sign at the two bounds, update the interval
  while ((fA * fB > 0.0))
    {
      a = 0.5 * a;
      fA = f(NumericalPoint(1, a))[0];
      if (fA * fB <= 0.0) break;
      b = 2.0 * b;
      fB = f(NumericalPoint(1, b))[0];
    }
  // Solve loosely the constraint equation
  Brent solver(ResourceMap::GetAsNumericalScalar( "KernelSmoothing-AbsolutePrecision" ), ResourceMap::GetAsNumericalScalar( "KernelSmoothing-RelativePrecision" ), ResourceMap::GetAsNumericalScalar( "KernelSmoothing-ResidualPrecision" ), ResourceMap::GetAsUnsignedLong( "KernelSmoothing-MaximumIteration" ));
  return NumericalPoint(1, solver.solve(f, 0.0, a, b) / kernel_.getStandardDeviation()[0]);
}

/* Compute the bandwidth according to a mixed rule:
 * simply use the plugin rule for small sample, and
 * estimate the ratio between the plugin rule and
 * the Silverman rule on a small sample, then
 * scale the Silverman bandwidth computed on the full
 * sample with this ratio
 */
NumericalPoint KernelSmoothing::computeMixedBandwidth(const NumericalSample & sample)
{
  const UnsignedLong dimension(sample.getDimension());
  if (dimension != 1) throw InvalidArgumentException(HERE) << "Error: mixed bandwidth is available only for 1D sample";
  const UnsignedLong size(sample.getSize());
  // Small sample, just return the plugin bandwidth
  if (size <= ResourceMap::GetAsUnsignedLong( "KernelSmoothing-SmallSize" )) return computePluginBandwidth(sample);
  NumericalSample smallSample(ResourceMap::GetAsUnsignedLong( "KernelSmoothing-SmallSize" ), 1);
  for (UnsignedLong i = 0; i < ResourceMap::GetAsUnsignedLong( "KernelSmoothing-SmallSize" ); ++i) smallSample[i][0] = sample[i][0];
  const NumericalScalar h1(computePluginBandwidth(smallSample)[0]);
  const NumericalScalar h2(computeSilvermanBandwidth(smallSample)[0]);
  return computeSilvermanBandwidth(sample) * (h1 / h2);
}

/* Build a Normal kernel mixture based on the given sample. If no bandwith has already been set, Silverman's rule is used */
Distribution KernelSmoothing::build(const NumericalSample & sample,
                                    const Bool boundaryCorrection)
{
  // For 1D sample, use the rule that give the best tradeoff between speed and precision
  if (sample.getDimension() == 1) return build(sample, computeMixedBandwidth(sample), boundaryCorrection);
  // For nD sample, use the only available rule
  return build(sample, computeSilvermanBandwidth(sample), boundaryCorrection);
}

/* Build a Normal kernel mixture based on the given sample and bandwidth */
Distribution KernelSmoothing::build(const NumericalSample & sample,
                                    const NumericalPoint & bandwidth,
                                    const Bool boundaryCorrection)
{
  const UnsignedLong dimension(sample.getDimension());
  if (bandwidth.getDimension() != dimension) throw InvalidDimensionException(HERE) << "Error: the given bandwidth must have the same dimension as the given sample, here bandwidth dimension=" << bandwidth.getDimension() << " and sample dimension=" << dimension;
  setBandwidth(bandwidth);
  // Make cheap boundary correction by extending the sample. Only valid for 1D sample.
  if (boundaryCorrection && (dimension == 1))
    {
      NumericalScalar min(sample.getMin()[0]);
      NumericalScalar max(sample.getMax()[0]);
      NumericalScalar h(bandwidth[0]);
      // Reflect and add points close to the boundaries to the sample
      NumericalSample newSample(sample);
      const UnsignedLong size(sample.getSize());
      for (UnsignedLong i = 0; i < size; i++)
        {
          NumericalScalar realization(sample[i][0]);
          if (realization <= min + h) newSample.add(NumericalPoint(1, 2.0 * min - realization));
          if (realization >= max - h) newSample.add(NumericalPoint(1, 2.0 * max - realization));
        }
      TruncatedDistribution kernelMixture(KernelMixture(kernel_, bandwidth, newSample), min, max);
      return kernelMixture;
    }
  KernelMixture kernelMixture(kernel_, bandwidth, sample);
  kernelMixture.setName("Kernel smoothing from sample " + sample.getName());
  return kernelMixture;
}

/* Bandwidth accessor */
void KernelSmoothing::setBandwidth(const NumericalPoint & bandwidth)
{
  // Check the given bandwidth
  for (UnsignedLong i = 0; i < bandwidth.getDimension(); i++)
    {
      if (bandwidth[i] <= 0.0) throw InvalidArgumentException(HERE) << "Error: the bandwidth must be > 0, here bandwith=" << bandwidth;
    }
  bandwidth_ = bandwidth;
}

NumericalPoint KernelSmoothing::getBandwidth() const
{
  return bandwidth_;
}

Distribution KernelSmoothing::getKernel() const
{
  return kernel_;
}

/* Method save() stores the object through the StorageManager */
void KernelSmoothing::save(Advocate & adv) const
{
  PersistentObject::save(adv);
  adv.saveAttribute( "bandwidth_", bandwidth_ );
}

/* Method load() reloads the object from the StorageManager */
void KernelSmoothing::load(Advocate & adv)
{
  PersistentObject::load(adv);
  adv.loadAttribute( "bandwidth_", bandwidth_ );
}

END_NAMESPACE_OPENTURNS
