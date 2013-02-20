//                                               -*- C++ -*-
/**
 *  @file  ContinuousDistribution.cxx
 *  @brief Abstract top-level class for Continuous distributions
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
#include <cstdlib>

#include "ContinuousDistribution.hxx"
#include "Collection.hxx"
#include "Distribution.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(ContinuousDistribution);


/* Default constructor */
ContinuousDistribution::ContinuousDistribution(const String & name)
  : UsualDistribution(name)
{
  // Nothing to do
}

/* Virtual constructor */
ContinuousDistribution * ContinuousDistribution::clone() const
{
  return new ContinuousDistribution(*this);
}

/* Comparison operator */
Bool ContinuousDistribution::operator ==(const ContinuousDistribution & other) const
{
  return (this == &other);
}

/* String converter */
String ContinuousDistribution::__repr__() const
{
  OSS oss;
  oss << "class=" << ContinuousDistribution::GetClassName();
  return oss;
}

/* Get the DDF of the distributionImplementation */
NumericalPoint ContinuousDistribution::computeDDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  NumericalPoint ddf(dimension);
  const NumericalScalar h(pow(pdfEpsilon_, 1.0 / 3.0));
  const NumericalScalar idenom(1.0 / (2.0 * h));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPoint left(point);
      left[i] += h;
      NumericalPoint right(point);
      right[i] -= h;
      ddf[i] = (computePDF(left) - computePDF(right)) * idenom;
    }
  return ddf;
}

/* Get the PDF of the distribution */
NumericalScalar ContinuousDistribution::computePDF(const NumericalPoint & point) const
{
  throw NotYetImplementedException(HERE);
}

/* Get the CDF of the distribution */
NumericalScalar ContinuousDistribution::computeCDF(const NumericalPoint & point) const
{
  const UnsignedLong dimension(getDimension());
  const NumericalPoint lowerBounds(getRange().getLowerBound());
  const NumericalPoint upperBounds(getRange().getUpperBound());
  Bool allOutside(true);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      if (point[i] <= lowerBounds[i]) return 0.0;
      allOutside &= (point[i] >= upperBounds[i]);
    }
  if (allOutside) return 1.0;
  const NumericalScalar cdf(ContinuousDistribution::computeProbability(Interval(getRange().getLowerBound(), point)));
  return cdf;
}

/* Get the probability content of an interval */
NumericalScalar ContinuousDistribution::computeProbability(const Interval & interval) const
{
  const Interval reducedInterval(interval.intersect(getRange()));
  if (reducedInterval.isNumericallyEmpty()) return 0.0;
  if (reducedInterval == getRange()) return 1.0;
  const UnsignedLong dimension(getDimension());
  const NumericalPoint lowerBounds(reducedInterval.getLowerBound());
  const NumericalPoint upperBounds(reducedInterval.getUpperBound());
  NumericalSample nodesAndWeights(getGaussNodesAndWeights());
  // Perform the integration
  const UnsignedLong marginalNodesNumber(getIntegrationNodesNumber());
  const UnsignedLong size(static_cast< UnsignedLong >(round(pow(marginalNodesNumber, dimension))));
  NumericalScalar probability(0.0);
  Indices indices(dimension, 0);
  for (UnsignedLong linearIndex = 0; linearIndex < size; ++linearIndex)
    {
      NumericalPoint node(dimension);
      NumericalScalar weight(1.0);
      for (UnsignedLong j = 0; j < dimension; ++j)
        {
          const UnsignedLong indiceJ(indices[j]);
          const NumericalScalar delta(0.5 * (upperBounds[j] - lowerBounds[j]));
          node[j] = lowerBounds[j] + delta * (1.0 + nodesAndWeights[0][indiceJ]);
          weight *= delta * nodesAndWeights[1][indiceJ];
        }
      probability += weight * computePDF(node);
      /* Update the indices */
      ++indices[0];
      /* Propagate the remainders */
      for (UnsignedLong j = 0; j < dimension - 1; ++j) indices[j + 1] += (indices[j] == marginalNodesNumber);
      /* Correction of the indices. The last index cannot overflow. */
      for (UnsignedLong j = 0; j < dimension - 1; ++j) indices[j] = indices[j] % marginalNodesNumber;
    } // Loop over the n-D nodes
  return probability;
}

/* Compute the CDF of Xi | X1, ..., Xi-1. x = Xi, y = (X1,...,Xi-1) */
NumericalScalar ContinuousDistribution::computeConditionalCDF(const NumericalScalar x,
                                                              const NumericalPoint & y) const
{
  const UnsignedLong conditioningDimension(y.getDimension());
  if (conditioningDimension >= getDimension()) throw InvalidArgumentException(HERE) << "Error: cannot compute a conditional CDF with a conditioning point of dimension greater or equal to the distribution dimension.";
  // Special case for no conditioning or independent copula
  if ((conditioningDimension == 0) || (hasIndependentCopula())) return Implementation(getMarginal(conditioningDimension))->computeCDF(x);
  // General case
  Indices conditioning(conditioningDimension);
  for (UnsignedLong i = 0; i < conditioningDimension; ++i)
    {
      conditioning[i] = i;
    }
  Indices conditioned(conditioning);
  conditioned.add(conditioningDimension);
  Implementation conditioningDistribution(getMarginal(conditioning));
  const NumericalScalar pdfConditioning(conditioningDistribution->computePDF(y));
  if (pdfConditioning <= 0.0) return 0.0;
  // Compute the conditional CDF by numerical integration of the conditional PDF. We use a fixed point Gauss integration.
  const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
  const UnsignedLong numberOfNodes(nodesAndWeights.getDimension());
  NumericalScalar cdfConditioned(0.0);
  const Implementation marginal(getMarginal(conditioningDimension));
  const NumericalScalar dispersion(marginal->getDispersionIndicator());
  const NumericalScalar position(marginal->getPositionIndicator());
  const NumericalScalar lowerBound(std::max(std::min(position, x) - 8.0 * dispersion, marginal->getRange().getLowerBound()[0]));
  const NumericalScalar halfLength(0.5 * (std::min(x, marginal->getRange().getUpperBound()[0]) - lowerBound));
  const Implementation conditionedDistribution(getMarginal(conditioned));
  cdfEpsilon_ = 0.0;
  // Map [x-8*s,x] into [-1,1]
  for (UnsignedLong i = 0; i < numberOfNodes; ++i)
    {
      const NumericalScalar w(nodesAndWeights[1][i]);
      const NumericalScalar xi(nodesAndWeights[0][i]);
      NumericalPoint z(y);
      z.add(lowerBound + (1.0 + xi) * halfLength);
      const NumericalScalar pdfConditioned(conditionedDistribution->computePDF(z));
      // Accumulate CDF value
      cdfConditioned += w * pdfConditioned;
      // Compute an approximation of the rounding error
      cdfEpsilon_ += w * conditionedDistribution->getPDFEpsilon();
    }
  cdfConditioned *= halfLength;
  cdfEpsilon_ = cdfEpsilon_ * halfLength + conditioningDistribution->getPDFEpsilon();
  return std::max(std::min(cdfConditioned / pdfConditioning, 1.0 - 1e-7), 1e-7);
}

/* Compute the covariance of the distribution */
void ContinuousDistribution::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension);
  // First the diagonal terms, which are the marginal covariances
  // Marginal covariances
  const NumericalPoint variance(getCenteredMoment(2));
  for(UnsignedLong component = 0; component < dimension; ++component) covariance_(component, component) = variance[component];
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
    {
      // To ensure that the mean is up to date
      mean_ = getMean();
      // Performs the integration for each covariance in the strictly lower triangle of the covariance matrix
      // We simply use a product gauss quadrature
      // We first loop over the coeeficients because the most expensive task is to get the 2D marginal distributions

      // Compute the conditional CDF by numerical integration of the conditional PDF. We use a fixed point Gauss integration.
      const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
      Indices indices(2);
      for(UnsignedLong rowIndex = 0; rowIndex < dimension; ++rowIndex)
        {
          indices[0] = rowIndex;
          for(UnsignedLong columnIndex = rowIndex + 1; columnIndex < dimension; ++columnIndex)
            {
              indices[1] = columnIndex;
              const Implementation marginalDistribution(getMarginal(indices));
              if (!marginalDistribution->hasIndependentCopula())
                {
                  const NumericalScalar aI(marginalDistribution->getRange().getLowerBound()[0]);
                  const NumericalScalar bI(marginalDistribution->getRange().getUpperBound()[0]);
                  const NumericalScalar aJ(marginalDistribution->getRange().getLowerBound()[1]);
                  const NumericalScalar bJ(marginalDistribution->getRange().getUpperBound()[1]);
                  const NumericalScalar halfLengthI(0.5 * (bI - aI));
                  const NumericalScalar halfLengthJ(0.5 * (bJ - aJ));
                  NumericalScalar covarianceIJ(0.0);
                  // Then we loop over the integration points
                  for(UnsignedLong rowNodeIndex = 0; rowNodeIndex < integrationNodesNumber_; ++rowNodeIndex)
                    {
                      const NumericalScalar nodeI(aI + (1.0 + nodesAndWeights[0][rowNodeIndex]) * halfLengthI);
                      const NumericalScalar xI(nodeI - mean_[rowNodeIndex]);
                      const NumericalScalar weightI(nodesAndWeights[1][rowNodeIndex]);
                      for(UnsignedLong columnNodeIndex = 0; columnNodeIndex < integrationNodesNumber_; ++columnNodeIndex)
                        {
                          const NumericalScalar nodeJ(aJ + (1.0 + nodesAndWeights[0][columnIndex]) * halfLengthJ);
                          const NumericalScalar xJ(nodeJ - mean_[columnNodeIndex]);
                          const NumericalScalar weightJ(nodesAndWeights[1][columnNodeIndex]);
                          NumericalPoint in(2);
                          in[0] = nodeI;
                          in[1] = nodeJ;
                          covarianceIJ += weightI * weightJ * xI * xJ * marginalDistribution->computePDF(in);
                        } // loop over J integration nodes
                    } // loop over I integration nodes
                  covariance_(rowIndex, columnIndex) = covarianceIJ * halfLengthI * halfLengthJ;
                }
            } // loop over column indices
        } // loop over row indices
    } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
} // computeCovariance

/* Compute the shifted moments of the distribution */
NumericalPoint ContinuousDistribution::computeShiftedMoment(const UnsignedLong n,
                                                            const NumericalPoint & shift) const
{
  const UnsignedLong dimension(getDimension());
  if (n == 0) throw InvalidArgumentException(HERE) << "Error: the centered moments of order 0 are undefined.";
  if (shift.getDimension() != dimension) throw InvalidArgumentException(HERE) << "Error: the shift dimension must match the distribution dimension.";
  setIntegrationNodesNumber(std::max(ResourceMap::GetAsUnsignedLong( "ContinuousDistribution-DefaultIntegrationNodesNumber" ), 20 * n + 1));
  const NumericalSample nodesAndWeights(getGaussNodesAndWeights());
  const UnsignedLong numberOfNodes(nodesAndWeights.getDimension());
  NumericalPoint moment(dimension, 0.0);
  // For each component
  for(UnsignedLong component = 0; component < dimension; ++component)
    {
      const Implementation marginalDistribution(getMarginal(component));
      const NumericalScalar a(marginalDistribution->getRange().getLowerBound()[0]);
      const NumericalScalar b(marginalDistribution->getRange().getUpperBound()[0]);
      const NumericalScalar halfLength(0.5 * (b - a));
      const NumericalScalar shiftComponent(shift[component]);
      NumericalScalar value(0.0);
      for (UnsignedLong i = 0; i < numberOfNodes; ++i)
        {
          const NumericalScalar w(nodesAndWeights[1][i]);
          const NumericalScalar xi(nodesAndWeights[0][i]);
          const NumericalScalar z(a + (1.0 + xi) * halfLength);
          value += w * pow(z - shiftComponent, n) * marginalDistribution->computePDF(z);
        } // Integration nodes
      moment[component] = value * halfLength;
    } // End of each component
  return moment;
}

/* Tell if the distribution is continuous */
Bool ContinuousDistribution::isContinuous() const
{
  return true;
}

END_NAMESPACE_OPENTURNS
