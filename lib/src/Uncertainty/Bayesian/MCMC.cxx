//                                               -*- C++ -*-
/**
 *  @file  MCMC.cxx
 *  @brief MCMC updates the chain
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

#include "MCMC.hxx"
#include "ConditionalDistribution.hxx"
#include "Normal.hxx"
#include "SpecFunc.hxx"
#include "PersistentObjectFactory.hxx"
#include "IdentityMatrix.hxx"
#include "LinearNumericalMathFunction.hxx"


BEGIN_NAMESPACE_OPENTURNS



CLASSNAMEINIT(MCMC);

static Factory<MCMC> RegisteredFactory("MCMC");

/* Default constructor */
MCMC::MCMC()
  : SamplerImplementation()
  , burnIn_(0)
  , thinning_(0)
{
}


/* Parameters constructor */
MCMC::MCMC( const Distribution & prior,
            const Distribution & conditional,
            const NumericalSample & observations,
            const NumericalPoint & initialState)
  : SamplerImplementation()
  , initialState_(initialState)
  , currentState_(initialState)
  , conditional_(conditional)

    // when not provided, set the model to the identity
  , model_(NumericalMathFunction::NumericalMathFunctionCollection(observations.getSize(), LinearNumericalMathFunction(NumericalPoint(initialState.getDimension()), NumericalPoint(initialState.getDimension()), IdentityMatrix(initialState.getDimension()))))

  , burnIn_(ResourceMap::GetAsUnsignedLong("MCMC-DefaultBurnIn"))
  , thinning_(ResourceMap::GetAsUnsignedLong("MCMC-DefaultThinning"))
{
  setPrior(prior);
  if (model_.getInputDimension() != prior.getDimension()) throw InvalidDimensionException(HERE) << "The model input dimension (" << model_.getInputDimension() << ") does not match the dimension of the prior (" << prior.getDimension() << ").";
  setObservations(observations);
  if (conditional.getParametersNumber() * observations.getSize() != model_.getOutputDimension()) throw InvalidDimensionException(HERE) << "The number of observations per parameter (" << conditional.getParametersNumber() << "x" << observations.getSize() << ") does not match the output dimension of the model (" << model_.getOutputDimension() << ").";
  if (initialState.getDimension() != prior.getDimension()) throw InvalidDimensionException(HERE) << "The initialState state dimension (" << initialState.getDimension() << ") does not match the prior dimension (" << prior.getDimension() << ").";

}


/* Parameters constructor */
MCMC::MCMC( const Distribution & prior,
            const Distribution & conditional,
            const NumericalMathFunction & model,
            const NumericalSample & observations,
            const NumericalPoint & initialState)
  : SamplerImplementation()
  , initialState_(initialState)
  , currentState_(initialState)
  , conditional_(conditional)
  , model_(model)
  , burnIn_(ResourceMap::GetAsUnsignedLong("MCMC-DefaultBurnIn"))
  , thinning_(ResourceMap::GetAsUnsignedLong("MCMC-DefaultThinning"))
{
  setPrior(prior);
  if (model.getInputDimension() != prior.getDimension()) throw InvalidDimensionException(HERE) << "The model input dimension (" << model.getInputDimension() << ") does not match the dimension of the prior (" << prior.getDimension() << ").";
  setObservations(observations);
  if (conditional.getParametersNumber() * observations.getSize() != model.getOutputDimension()) throw InvalidDimensionException(HERE) << "The number of observations per parameter (" << conditional.getParametersNumber() << "x" << observations.getSize() << ") does not match the output dimension of the model (" << model.getOutputDimension() << ").";
  if (initialState.getDimension() != prior.getDimension()) throw InvalidDimensionException(HERE) << "The initialState state dimension (" << initialState.getDimension() << ") does not match the prior dimension (" << prior.getDimension() << ").";

}


/* String converter */
String MCMC::__repr__() const
{
  return OSS() << "class=" << MCMC::GetClassName()
               << "prior=" << prior_
               << "conditional=" << conditional_
               << "model=" << model_
               << "observations=" << observations_
               << "burnIn=" << burnIn_
               << "thinning=" << thinning_;
}


MCMC* MCMC::clone() const
{
  return new MCMC(*this);
}


UnsignedLong MCMC::getDimension() const
{
  return prior_.getDimension();
}


/* Compute the likelihood w.r.t. observartions */
NumericalScalar MCMC::computeLogLikelihood(const NumericalPoint & xi) const
{
  NumericalScalar value = prior_.computeLogPDF(xi);
  if ( value == -SpecFunc::MaxNumericalScalar ) return -SpecFunc::MaxNumericalScalar;

  // retrieve model data if available
  const NumericalPoint z( model_(xi) );

  const UnsignedLong size = observations_.getSize();
  const UnsignedLong p = conditional_.getParametersNumber();
  for ( UnsignedLong i = 0; i < size; ++ i )
    {
      NumericalPoint zi(p);
      for ( UnsignedLong j = 0; j < p; ++ j )
        {
          zi[j] = z[i * p + j];
        }
      Distribution pI( conditional_ );
      pI.setParametersCollection( zi );
      NumericalScalar logPdf = pI.computeLogPDF( observations_[i] );
      if ( logPdf == -SpecFunc::MaxNumericalScalar ) return -SpecFunc::MaxNumericalScalar;
      value += logPdf;
    }
  return value;
}

void MCMC::setPrior(const Distribution& prior)
{
  if (!prior.isContinuous()) throw InvalidArgumentException(HERE) << "The prior should be continuous.";
  prior_ = prior;
}


Distribution MCMC::getPrior() const
{
  return prior_;
}


Distribution MCMC::getConditional() const
{
  return conditional_;
}


NumericalMathFunction MCMC::getModel() const
{
  return model_;
}


void MCMC::setObservations(const NumericalSample& observations)
{
  if (!(observations.getSize() > 0)) throw InvalidArgumentException(HERE) << "No observations provided.";
  observations_ = observations;
}


NumericalSample MCMC::getObservations() const
{
  return observations_;
}


void MCMC::setBurnIn(UnsignedLong burnIn)
{
  burnIn_ = burnIn;
}


UnsignedLong MCMC::getBurnIn() const
{
  return burnIn_;
}


void MCMC::setThinning(UnsignedLong thinning)
{
  thinning_ = thinning;
}


UnsignedLong MCMC::getThinning() const
{
  return thinning_;
}


/* Method save() stores the object through the StorageManager */
void MCMC::save(Advocate & adv) const
{
  SamplerImplementation::save(adv);
  adv.saveAttribute("initialState_", initialState_);
  adv.saveAttribute("currentState_", currentState_);
  adv.saveAttribute("prior_", prior_);
  adv.saveAttribute("conditional_", conditional_);
  adv.saveAttribute("model_", model_);
  adv.saveAttribute("observations_", observations_);
  adv.saveAttribute("burnIn_", burnIn_);
  adv.saveAttribute("thinning_", thinning_);
}

/* Method load() reloads the object from the StorageManager */
void MCMC::load(Advocate & adv)
{
  SamplerImplementation::load(adv);
  adv.loadAttribute("initialState_", initialState_);
  adv.loadAttribute("currentState_", currentState_);
  adv.loadAttribute("prior_", prior_);
  adv.loadAttribute("conditional_", conditional_);
  adv.loadAttribute("model_", model_);
  adv.loadAttribute("observations_", observations_);
  adv.loadAttribute("burnIn_", burnIn_);
  adv.loadAttribute("thinning_", thinning_);
}


END_NAMESPACE_OPENTURNS
