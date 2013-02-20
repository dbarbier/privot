//                                               -*- C++ -*-
/**
 *  @file  LeastSquaresStrategy.hxx
 *  @brief An implementation of projection strategy as a leastSquares
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
 *  @date   2008-05-21 17:44:02 +0200 (Wed, 21 May 2008)
 *  Id      Object.hxx 818 2008-05-21 15:44:02Z dutka
 */
#ifndef OPENTURNS_LEASTSQUARESSTRATEGY_HXX
#define OPENTURNS_LEASTSQUARESSTRATEGY_HXX

#include "ProjectionStrategyImplementation.hxx"
#include "PenalizedLeastSquaresAlgorithmFactory.hxx"
#include "ApproximationAlgorithmImplementationFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS



/**
 * @class LeastSquaresStrategy
 *
 * An implementation of projection strategy as a leastSquares
 */

class LeastSquaresStrategy
  : public ProjectionStrategyImplementation
{
  CLASSNAME;
public:

  typedef Pointer<ApproximationAlgorithmImplementationFactory>  ApproximationAlgorithmImplementationFactoryImplementation;

  // friend class Factory<LeastSquaresStrategy>;

  /** Default constructor */
  LeastSquaresStrategy(const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  /** Parameter constructor */
  LeastSquaresStrategy(const Distribution & measure,
                       const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  /** Parameter constructor */
  LeastSquaresStrategy(const WeightedExperiment & weightedExperiment,
                       const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  /** Parameter constructor */
  LeastSquaresStrategy(const Distribution & measure,
                       const WeightedExperiment & weightedExperiment,
                       const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  /** Parameter constructor */
  LeastSquaresStrategy(const NumericalSample & inputSample,
                       const NumericalPoint & weights,
                       const NumericalSample & outputSample,
                       const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  LeastSquaresStrategy(const NumericalSample & inputSample,
                       const NumericalSample & outputSample,
                       const ApproximationAlgorithmImplementationFactory & factory = PenalizedLeastSquaresAlgorithmFactory());

  /** Virtual constructor */
  virtual LeastSquaresStrategy * clone() const;

  /** String converter */
  String __repr__() const;

  /** Compute the components alpha_k_p_ by projecting the model on the partial L2 basis */
  void computeCoefficients(const NumericalMathFunction & function,
                           const NumericalMathFunctionCollection & partialBasis,
                           const Indices & indices,
                           const Indices & addedRanks,
                           const Indices & conservedRanks,
                           const Indices & removedRanks,
                           const UnsignedLong marginalIndex = 0);

  /** Method save() stores the object through the StorageManager */
  void save(Advocate & adv) const;

  /** Method load() reloads the object from the StorageManager */
  void load(Advocate & adv);


protected:

private:
  /** Factory to build an ApproximationAlgorithmImplementation */
  ApproximationAlgorithmImplementationFactoryImplementation p_approximationAlgorithmImplementationFactory_;


} ; /* class LeastSquaresStrategy */


END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_LEASTSQUARESSTRATEGY_HXX */
