//                                               -*- C++ -*-
/**
 *  @file  RandomWalkMetropolisHastings.cxx
 *  @brief RandomWalkMetropolisHastings updates the chain
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
 */

#include "RandomWalkMetropolisHastings.hxx"
#include "RandomGenerator.hxx"
#include "PersistentObjectFactory.hxx"
#include "ConditionalDistribution.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

TEMPLATE_CLASSNAMEINIT(PersistentCollection<CalibrationStrategy>);
static Factory<PersistentCollection<CalibrationStrategy> > RegisteredFactory2("PersistentCollection<CalibrationStrategy>");


CLASSNAMEINIT(RandomWalkMetropolisHastings);

static Factory<RandomWalkMetropolisHastings> RegisteredFactory("RandomWalkMetropolisHastings");

/* Default constructor */
RandomWalkMetropolisHastings::RandomWalkMetropolisHastings()
  : MCMC()
  , calibrationStrategy_(0)
  , samplesNumber_(0)
  , acceptedNumber_(0)
{
}


/* Parameters constructor */
RandomWalkMetropolisHastings::RandomWalkMetropolisHastings( const Distribution & prior,
                                                            const Distribution & conditional,
                                                            const NumericalSample & observations,
                                                            const NumericalPoint & initialState,
                                                            const DistributionCollection & proposal)
  : MCMC(prior, conditional, observations, initialState)
  , calibrationStrategy_(proposal_.getSize())
  , samplesNumber_(0)
  , acceptedNumber_(initialState.getDimension())
{
  setProposal(proposal);
}


/* Parameters constructor */
RandomWalkMetropolisHastings::RandomWalkMetropolisHastings( const Distribution & prior,
                                                            const Distribution & conditional,
                                                            const NumericalMathFunction & model,
                                                            const NumericalSample & observations,
                                                            const NumericalPoint & initialState,
                                                            const DistributionCollection & proposal)
  : MCMC(prior, conditional, model, observations, initialState)
  , calibrationStrategy_(proposal_.getSize())
  , samplesNumber_(0)
  , acceptedNumber_(initialState.getDimension())
{
  setProposal(proposal);
}


/* String converter */
String RandomWalkMetropolisHastings::__repr__() const
{
  return OSS() << "class=" << RandomWalkMetropolisHastings::GetClassName()
               << " name=" << getName()
               << " derived from " << MCMC::__repr__()
               << " proposal=" << proposal_
               << " calibrationStrategy=" << calibrationStrategy_;
}


RandomWalkMetropolisHastings* RandomWalkMetropolisHastings::clone() const
{
  return new RandomWalkMetropolisHastings(*this);
}

/* Here is the interface that all derived class must implement */

NumericalPoint RandomWalkMetropolisHastings::getRealization() const
{
  const UnsignedLong dimension = initialState_.getDimension();

  // update factor
  NumericalPoint delta(dimension, 1.);

  // number of samples accepted until calibration step
  Indices accepted(dimension);

  // perform burning if necessary
  const UnsignedLong size = getThinning() + (( samplesNumber_ < getBurnIn() ) ? getBurnIn() : 0);

  // compute the first likelihood
  NumericalScalar alphaLogSave = computeLogLikelihood(currentState_);


  // for each new sample
  UnsignedLong acceptedSteps = 0;
  for ( UnsignedLong i = 0; i < size; ++ i )
    {
      // copy ot current state to accept each component independently
      NumericalPoint newState(currentState_);

      // reuse the likelihood if one or None candidate has been accepted
      NumericalScalar alphaLogPrev = acceptedSteps <= 1 ? alphaLogSave : computeLogLikelihood(currentState_);
      alphaLogSave = alphaLogPrev;

      // update each chain component
      acceptedSteps = 0;
      for ( UnsignedLong j = 0; j < dimension; ++ j )
        {
          NumericalPoint nextState(currentState_);

          // new candidate per component
          nextState[j] += delta[j] * proposal_[j].getRealization()[0];

          const NumericalScalar alphaLogNext = computeLogLikelihood(nextState);

          // alphaLog = likehood(oldState)/likehood(newstate)
          const NumericalScalar alphaLog = alphaLogNext  - alphaLogPrev;

          // acceptation test
          const NumericalScalar uLog = log( RandomGenerator::Generate() );
          if ( uLog < alphaLog )
            {
              ++ acceptedSteps;
              alphaLogSave = alphaLogNext;
              ++ acceptedNumber_[j];
              ++ accepted[j];

              // accept the component
              newState[j] = nextState[j];
            }
        }

      // update state
      currentState_ = newState;

      // recalibrate for each component if necessary
      NumericalPoint factor(dimension);
      NumericalPoint partialRho(dimension);
      for ( UnsignedLong j = 0; j < dimension; ++ j )
        {
          const UnsignedLong calibrationStep = calibrationStrategy_[j].getCalibrationStep();
          if ( ( samplesNumber_ % calibrationStep ) == ( calibrationStep - 1 ) )
            {
              // compute the current acceptation rate
              NumericalScalar rho = static_cast<NumericalScalar>(accepted[j]) / calibrationStep;

              // compute factor
              NumericalScalar factor = calibrationStrategy_[j].computeUpdateFactor( rho );

              // update delta
              delta[j] *= factor;

              // reset the counter
              accepted[j] = 0;

              if ( getVerbose() )
                {
                  LOGINFO( OSS() << "rho=" << rho << " factor=" << factor << " delta=" << delta.getCollection() << " accept=" << getAcceptanceRate().getCollection() );
                }

            }
        }

      ++ samplesNumber_;

    } // for i

  return currentState_;
}


NumericalPoint RandomWalkMetropolisHastings::getAcceptanceRate() const
{
  const UnsignedLong dimension = initialState_.getDimension();
  NumericalPoint acceptanceRate(dimension);
  for ( UnsignedLong j = 0; j < dimension; ++ j )
    {
      acceptanceRate[j] = static_cast<NumericalScalar>(acceptedNumber_[j]) / samplesNumber_;
    }
  return acceptanceRate;
}


void RandomWalkMetropolisHastings::setCalibrationStrategy(const CalibrationStrategy& calibrationStrategy)
{
  for ( UnsignedLong i = 0; i < calibrationStrategy_.getSize(); ++ i )
    {
      calibrationStrategy_[i] = calibrationStrategy;
    }
}

RandomWalkMetropolisHastings::CalibrationStrategyCollection RandomWalkMetropolisHastings::getCalibrationStrategyPerComponent() const
{
  return calibrationStrategy_;
}

void RandomWalkMetropolisHastings::setCalibrationStrategyPerComponent(const CalibrationStrategyCollection& calibrationStrategy)
{
  const UnsignedLong dimension = proposal_.getSize();
  if(dimension != calibrationStrategy.getSize()) throw InvalidDimensionException(HERE) << "The proposal dimension (" << dimension << ") does not match the size of calibration strategy (" << calibrationStrategy.getSize() << ").";
  calibrationStrategy_ = calibrationStrategy;
}


void RandomWalkMetropolisHastings::setProposal(const RandomWalkMetropolisHastings::DistributionCollection& proposal)
{
  const UnsignedLong dimension = getPrior().getDimension();
  if (proposal.getSize() != dimension) throw InvalidDimensionException(HERE) << "The proposal dimension (" << proposal.getSize() << ") does not match the prior dimension (" << dimension << ").";

  for ( UnsignedLong i = 0; i < dimension; ++ i )
    {
      Bool symmetric = proposal[i].isElliptical();
      symmetric = symmetric && (fabs(proposal[i].getMean()[0]) < ResourceMap::GetAsNumericalScalar("DistributionImplementation-DefaultQuantileEpsilon"));
      if (!symmetric) throw InvalidArgumentException(HERE) << "The proposal density is not symmetric.";
    }
  proposal_ = proposal;
}


RandomWalkMetropolisHastings::DistributionCollection RandomWalkMetropolisHastings::getProposal() const
{
  return proposal_;
}


/* Method save() stores the object through the StorageManager */
void RandomWalkMetropolisHastings::save(Advocate & adv) const
{
  MCMC::save(adv);
  adv.saveAttribute("proposal_", proposal_);
  adv.saveAttribute("calibrationStrategy_", calibrationStrategy_);
  adv.saveAttribute("samplesNumber_", samplesNumber_);
  adv.saveAttribute("acceptedNumber_", acceptedNumber_);
}

/* Method load() reloads the object from the StorageManager */
void RandomWalkMetropolisHastings::load(Advocate & adv)
{
  MCMC::load(adv);
  adv.loadAttribute("proposal_", proposal_);
  adv.loadAttribute("calibrationStrategy_", calibrationStrategy_);
  adv.loadAttribute("samplesNumber_", samplesNumber_);
  adv.loadAttribute("acceptedNumber_", acceptedNumber_);
}


END_NAMESPACE_OPENTURNS
