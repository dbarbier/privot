//                                               -*- C++ -*-
/**
 *  @file  CopulaImplementation.cxx
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
#include "CopulaImplementation.hxx"
#include "IndependentCopula.hxx"
#include "Distribution.hxx"
#include "Brent.hxx"
#include "Curve.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(CopulaImplementation);

/* Default constructor */
CopulaImplementation::CopulaImplementation(const String & name)
  : ContinuousDistribution(name)
{
  // Nothing to do
}

/* Virtual constructor */
CopulaImplementation * CopulaImplementation::clone() const
{
  return new CopulaImplementation(*this);
}

/* Comparison operator */
Bool CopulaImplementation::operator ==(const CopulaImplementation & other) const
{
  return (this != &other);
}

/* String converter */
String CopulaImplementation::__repr__() const
{
  OSS oss;
  oss << "class=" << CopulaImplementation::GetClassName();
  return oss;
}

/* Generic implementation of the quantile computation for copulas */
NumericalPoint CopulaImplementation::computeQuantile(const NumericalScalar prob,
                                                     const Bool tail) const
{
  NumericalScalar quantileEpsilon(ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
  if (prob < -quantileEpsilon || prob > 1.0 + quantileEpsilon) throw InvalidArgumentException(HERE) << "Error: cannot compute a quantile for a probability level outside of [0, 1]";
  const UnsignedLong dimension(getDimension());
  // Special case for bording values
  const NumericalScalar q(tail ? 1.0 - prob : prob);
  if (q <= 0.0) return NumericalPoint(dimension, 0.0);
  if (q >= 1.0) return NumericalPoint(dimension, 1.0);
  // Special case for dimension 1
  if (dimension == 1) return NumericalPoint(1, q);
  QuantileWrapper wrapper(this);
  const NumericalMathFunction f(bindMethod<QuantileWrapper, NumericalPoint, NumericalPoint>(wrapper, &QuantileWrapper::computeDiagonal, 1, 1));
  NumericalScalar leftTau(q);
  const NumericalPoint leftPoint(1, leftTau);
  const NumericalPoint leftValue(f(leftPoint));
  NumericalScalar leftCDF(leftValue[0]);
  // Upper bound of the bracketing interval
  NumericalScalar rightTau(1.0 - (1.0 - q) / dimension);
  NumericalPoint rightPoint(1, rightTau);
  const NumericalPoint rightValue(f(rightPoint));
  NumericalScalar rightCDF(rightValue[0]);
  // Use Brent's method to compute the quantile efficiently
  Brent solver(cdfEpsilon_, cdfEpsilon_, cdfEpsilon_, ResourceMap::GetAsUnsignedLong( "DistributionImplementation-DefaultQuantileIteration" ));
  return NumericalPoint(dimension, solver.solve(f, q, leftTau, rightTau, leftCDF, rightCDF));
}


/* Get the mean of the distribution */
NumericalPoint CopulaImplementation::getMean() const
{
  return NumericalPoint(getDimension(), 0.5);
}

/* Get the standard deviation of the distribution */
NumericalPoint CopulaImplementation::getStandardDeviation() const
{
  // 0.2886751345948128822545744 = 1 / sqrt(12)
  return NumericalPoint(getDimension(), 0.2886751345948128822545744);
}

/* Get the skewness of the distribution */
NumericalPoint CopulaImplementation::getSkewness() const
{
  return NumericalPoint(getDimension(), 0.0);
}

/* Get the kurtosis of the distribution */
NumericalPoint CopulaImplementation::getKurtosis() const
{
  // 1.8 = 9/5
  return NumericalPoint(getDimension(), 1.8);
}

/* Compute the covariance of the distribution */
void CopulaImplementation::computeCovariance() const
{
  const UnsignedLong dimension(getDimension());
  // We need this to initialize the covariance matrix in two cases:
  // + this is the first call to this routine (which could be checked by testing the dimension of the distribution and the dimension of the matrix
  // + the copula has changed from a non-independent one to the independent copula
  covariance_ = CovarianceMatrix(dimension);
  // First the diagonal terms, which are the marginal covariances
  // Uniform marginals, the diagonal is 1/12
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      // 0.08333333333333333333333333 = 1 / 12
      covariance_(i, i) = 0.08333333333333333333333333;
    }
  // Off-diagonal terms if the copula is not the independent copula
  if (!hasIndependentCopula())
    {
      // Compute the weights and nodes off the 1D gauss quadrature over [-1, 1]
      NumericalSample nodesAndWeights(getGaussNodesAndWeights());
      // Convert the nodes and weights for the interval [0, 1]
      for (UnsignedLong i = 0; i < integrationNodesNumber_; ++i)
        {
          nodesAndWeights[0][i] = 0.5 * (nodesAndWeights[0][i] + 1.0);
          nodesAndWeights[1][i] *= 0.5;
        }
      // Performs the integration for each covariance in the strictly lower triangle of the covariance matrix
      // We simply use a product gauss quadrature
      // We first loop over the coeeficients because the most expensive task is to get the 2D marginal copulas
      Indices indices(2);
      for(UnsignedLong rowIndex = 0; rowIndex < dimension; ++rowIndex)
        {
          indices[0] = rowIndex;
          for(UnsignedLong columnIndex = rowIndex + 1; columnIndex < dimension; ++columnIndex)
            {
              indices[1] = columnIndex;
              // For the usual case of a bidimensional copula, no need to extract marginal distributions
              Distribution marginalDistribution(*this);
              if (dimension > 2) marginalDistribution = getMarginal(indices);
              if (!marginalDistribution.getImplementation()->hasIndependentCopula())
                {
                  NumericalScalar covarianceIJ(0.0);
                  // Then we loop over the integration points
                  for(UnsignedLong rowNodeIndex = 0; rowNodeIndex < integrationNodesNumber_; ++rowNodeIndex)
                    {
                      const NumericalScalar nodeI(nodesAndWeights[0][rowNodeIndex]);
                      const NumericalScalar weightI(nodesAndWeights[1][rowNodeIndex]);
                      for(UnsignedLong columnNodeIndex = 0; columnNodeIndex < integrationNodesNumber_; ++columnNodeIndex)
                        {
                          const NumericalScalar nodeJ(nodesAndWeights[0][columnNodeIndex]);
                          const NumericalScalar weightJ(nodesAndWeights[1][columnNodeIndex]);
                          NumericalPoint in(2);
                          in[0] = nodeI;
                          in[1] = nodeJ;
                          covarianceIJ += weightI * weightJ * (marginalDistribution.computeCDF(in) - nodeI * nodeJ);
                        } // loop over J integration nodes
                    } // loop over I integration nodes
                  covariance_(rowIndex, columnIndex) = covarianceIJ;
                }
            } // loop over column indices
        } // loop over row indices
    } // if !hasIndependentCopula
  isAlreadyComputedCovariance_ = true;
} // computeCovariance

/* Get the i-th marginal distribution */
CopulaImplementation::Implementation CopulaImplementation::getMarginal(const UnsignedLong i) const
{
  if (i >= getDimension()) throw InvalidArgumentException(HERE) << "The index of a marginal distribution must be in the range [0, dim-1]";
  return new IndependentCopula(1);
}

/* Get the copula of a distribution */
CopulaImplementation::Implementation CopulaImplementation::getCopula() const
{
  return clone();
}

/* Compute the mathematical and numerical range of the distribution.
   Its mathematical range is the smallest closed interval outside
   of which the PDF is zero, and the numerical range is the interval
   outside of which the PDF is rounded to zero in double precision */
void CopulaImplementation::computeRange()
{
  setRange(Interval(getDimension()));
}

/* Draw the PDF of the copula when its dimension is 2 */
Graph CopulaImplementation::drawPDF(const Indices & pointNumber) const
{
  Graph graph(drawPDF(NumericalPoint(2, 0.0), NumericalPoint(2, 1.0), pointNumber));
  Drawable drawable(graph.getDrawable(0));
  NumericalSample data(5, 2);
  data[1][0] = 1.0;
  data[2]    = NumericalPoint(2, 1.0);
  data[3][1] = 1.0;
  Curve square(data);
  square.setColor("blue");
  graph.setDrawable(square, 0);
  graph.add(drawable);
  return graph;
}

/* Draw the CDF of the copula when its dimension is 2 */
Graph CopulaImplementation::drawCDF(const Indices & pointNumber) const
{
  return drawCDF(NumericalPoint(2, 0.0), NumericalPoint(2, 1.0), pointNumber);
}

END_NAMESPACE_OPENTURNS
