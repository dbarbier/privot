//                                               -*- C++ -*-
/**
 *  @file  MarginalTransformationEvaluation.cxx
 *  @brief Class for the Nataf transformation evaluation for elliptical
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
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include "MarginalTransformationEvaluation.hxx"
#include "PersistentObjectFactory.hxx"
#include "Uniform.hxx"
#include "Log.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(MarginalTransformationEvaluation);

static Factory<MarginalTransformationEvaluation> RegisteredFactory("MarginalTransformationEvaluation");


/* Default constructor */
MarginalTransformationEvaluation::MarginalTransformationEvaluation():
  NumericalMathEvaluationImplementation(),
  inputDistributionCollection_(),
  outputDistributionCollection_(),
  direction_()
{
  // Nothing to do
}

/* Parameter constructor */
MarginalTransformationEvaluation::MarginalTransformationEvaluation(const DistributionCollection & inputDistributionCollection,
                                                                   const DistributionCollection & outputDistributionCollection,
                                                                   const Bool simplify):
  NumericalMathEvaluationImplementation(),
  inputDistributionCollection_(inputDistributionCollection),
  outputDistributionCollection_(outputDistributionCollection),
  direction_(FROMTO),
  simplifications_(inputDistributionCollection.getSize(), 0),
  expressions_(inputDistributionCollection.getSize())
{
  Description description;
  const UnsignedLong size(inputDistributionCollection.getSize());
  // Check that the collections of input and output distributions have the same size
  if (outputDistributionCollection_.getSize() != size) throw InvalidArgumentException(HERE) << "Error: a MarginalTransformationEvaluation cannot be built using collections of input and output distributions of different size";
  // First, check that the distributions are all 1D
  for (UnsignedLong i = 0; i < size; ++i)
    {
      if (inputDistributionCollection_[i].getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: a MarginalTransformationEvaluation cannot be built using distributions with dimension > 1.";
      if (outputDistributionCollection_[i].getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: a MarginalTransformationEvaluation cannot be built using distributions with dimension > 1.";
    }
  // Second, build the description of the transformation
  for (UnsignedLong i = 0; i < size; ++i)
    {
      OSS oss;
      oss << "x" << i;
      description.add(oss);
    }
  for (UnsignedLong i = 0; i < size; ++i)
    {
      OSS oss;
      oss << "y" << i;
      description.add(oss);
    }
  setDescription(description);
  if (simplify)
    {
      // Third, look for possible simplifications
      for (UnsignedLong i = 0; i < size; ++i)
        {
          const String xName(getInputDescription()[i]);
          const String yName(getOutputDescription()[i]);
          const Distribution inputDistribution(inputDistributionCollection[i]);
          const Distribution outputDistribution(outputDistributionCollection[i]);
          const String inputClass(inputDistribution.getImplementation()->getClassName());
          const String outputClass(outputDistribution.getImplementation()->getClassName());
          const NumericalPoint inputParameters(inputDistribution.getParametersCollection()[0]);
          const NumericalPoint outputParameters(outputDistribution.getParametersCollection()[0]);
          // Normal/Normal simplification
          if ((inputClass == "Normal") && (outputClass == "Normal"))
            {
              const NumericalScalar mu1(inputParameters[0]);
              const NumericalScalar sigma1(inputParameters[1]);
              const NumericalScalar mu2(outputParameters[0]);
              const NumericalScalar sigma2(outputParameters[1]);
              OSS oss;
              oss.setPrecision(20);
              if (mu2 != 0.0) oss << mu2 << " + ";
              oss << sigma2 / sigma1;
              if (mu1 == 0.0) oss << " * " << xName;
              else
                {
                  oss << " * (" << xName;
                  if (mu1 > 0.0) oss << " - " << mu1 << ")";
                  else oss << " + " << -mu1 << ")";
                }
              const String formula(oss);
              expressions_[i] = NumericalMathFunction(xName, formula, yName);
              simplifications_[i] = 1;
            } // Normal vs Normal
          // Uniform/Uniform simplification
          if ((inputClass == "Uniform") && (outputClass == "Uniform"))
            {
              const NumericalScalar a1(inputParameters[0]);
              const NumericalScalar b1(inputParameters[1]);
              const NumericalScalar a2(outputParameters[0]);
              const NumericalScalar b2(outputParameters[1]);
              OSS oss;
              oss.setPrecision(20);
              if (a2 != 0.0) oss << a2 << " + ";
              oss << b2 - a2 << " * ("; // " * max(0.0, min(1.0, ";
              if (a1 == 0.0) oss << xName << " / " << b1 - a1 << ")"; // "))";
              else
                {
                  oss << "(" << xName;
                  if (a1 > 0.0) oss << " - " << a1 << ") / " << b1 - a1 << ")"; // "))";
                  else oss << " + " << -a1 << ") / " << b1 - a1 << ")"; // "))";
                }
              const String formula(oss);
              expressions_[i] = NumericalMathFunction(xName, formula, yName);
              simplifications_[i] = 1;
            } // Uniform vs Uniform
          if (((inputClass == "ChiSquare") || (inputClass == "Exponential") || (inputClass == "Gamma")) &&
              ((outputClass == "ChiSquare") || (outputClass == "Exponential") || (outputClass == "Gamma")))
            {
              NumericalScalar k1;
              NumericalScalar lambda1;
              NumericalScalar gamma1;
              if (inputClass == "ChiSquare")
                {
                  k1 = 0.5 * inputParameters[0];
                  lambda1 = 0.5 * inputParameters[0];
                  gamma1 = 0.0;
                }
              else if (inputClass == "Exponential")
                {
                  k1 = 1.0;
                  lambda1 = inputParameters[0];
                  gamma1 = inputParameters[1];
                }
              else
                {
                  k1 = inputParameters[0];
                  lambda1 = inputParameters[1];
                  gamma1 = inputParameters[2];
                }
              NumericalScalar k2;
              NumericalScalar lambda2;
              NumericalScalar gamma2;
              if (outputClass == "ChiSquare")
                {
                  k2 = 0.5 * outputParameters[0];
                  lambda2 = 0.5 * outputParameters[0];
                  gamma2 = 0.0;
                }
              else if (outputClass == "Exponential")
                {
                  k2 = 1.0;
                  lambda2 = outputParameters[0];
                  gamma2 = outputParameters[1];
                }
              else
                {
                  k2 = outputParameters[0];
                  lambda2 = outputParameters[1];
                  gamma2 = outputParameters[2];
                }
              // There is a simplification only if k1 == k2
              if (k1 == k2)
                {
                  OSS oss;
                  oss.setPrecision(20);
                  oss << "max(0.0, " << gamma2 + log(lambda2) << " + " << lambda1 << " * ";
                  if (gamma1 == 0.0) oss << xName << ")";
                  else
                    {
                      oss << "(" << xName;
                      if (gamma1 > 0.0) oss << " - " << gamma1 << "))";
                      else oss << " + " << -gamma1 << "))";
                    }
                  const String formula(oss);
                  expressions_[i] = NumericalMathFunction(xName, formula, yName);
                  simplifications_[i] = 1;
                } // k1 == k2
            } // ChiSquare || Exponential || Gamma vs ChiSquare || Exponential || Gamma
        } // Loop over the components
    } // Simplifications
}

/* Parameter constructor */
MarginalTransformationEvaluation::MarginalTransformationEvaluation(const DistributionCollection & distributionCollection,
                                                                   const UnsignedLong direction):
  NumericalMathEvaluationImplementation(),
  inputDistributionCollection_(0),
  outputDistributionCollection_(0),
  direction_(direction),
  simplifications_(distributionCollection.getSize(), 0),
  expressions_(distributionCollection.getSize())
{
  switch (direction)
    {
    case FROM:
      // From the given marginals to the uniform ones
      inputDistributionCollection_ = distributionCollection;
      outputDistributionCollection_ = DistributionCollection(distributionCollection.getSize(), Uniform(0.0, 1.0));
      break;
    case TO:
      // From uniform marginals to the given ones
      inputDistributionCollection_ = DistributionCollection(distributionCollection.getSize(), Uniform(0.0, 1.0));
      outputDistributionCollection_ = distributionCollection;
      break;
    default:
      throw InvalidArgumentException(HERE) << "Error: wrong value given for direction";
    }
  *this = MarginalTransformationEvaluation(inputDistributionCollection_, outputDistributionCollection_);
  // We must overwrite the value of direction_ by the given one, as the call of the general constructor has set the value to FROMTO
  direction_ = direction;
  // Get all the parameters
  // The notion of parameters is used only for transformation from or to a standard space, so we have to extract the parameters of either the input distributions or the output distribution depending on the direction
  const UnsignedLong size(distributionCollection.getSize());
  NumericalPointWithDescription parameters(0);
  Description parametersDescription(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      // The marginal distribution is 1D, so the collection of parameters is of size 1
      NumericalPointWithDescription marginalParameters(distributionCollection[i].getParametersCollection()[0]);
      const UnsignedLong marginalParametersSize(marginalParameters.getSize());
      for (UnsignedLong j = 0; j < marginalParametersSize; ++j)
        {
          parameters.add(marginalParameters[j]);
          parametersDescription.add(marginalParameters.getDescription()[j]);
        }
      parameters.setDescription(parametersDescription);
    } // get all the parameters
  setParameters(parameters);
}

/* Virtual constructor */
MarginalTransformationEvaluation * MarginalTransformationEvaluation::clone() const
{
  return new MarginalTransformationEvaluation(*this);
}

/* Evaluation */
NumericalPoint MarginalTransformationEvaluation::operator () (const NumericalPoint & inP) const
{
  const UnsignedLong dimension(getOutputDimension());
  NumericalPoint result(dimension);
  // The marginal transformation apply G^{-1} o F to each component of the input, where F is the ith input CDF and G the ith output CDf
  const NumericalScalar tailThreshold(ResourceMap::GetAsNumericalScalar( "MarginalTransformationEvaluation-DefaultTailThreshold" ));
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      if (simplifications_[i]) result[i] = expressions_[i](NumericalPoint(1, inP[i]))[0];
      else
        {
          NumericalScalar inputCDF(inputDistributionCollection_[i].computeCDF(inP[i]));
          // For accuracy reason, check if we are in the upper tail of the distribution
          const Bool upperTail(inputCDF > tailThreshold);
          if (upperTail) inputCDF = inputDistributionCollection_[i].computeComplementaryCDF(inP[i]);
          // The upper tail CDF is defined by CDF(x, upper) = P(X>x)
          // The upper tail quantile is defined by Quantile(CDF(x, upper), upper) = x
          const NumericalScalar q(outputDistributionCollection_[i].computeQuantile(inputCDF, upperTail)[0]);
          result[i] = q;
        }
    }
  ++callsNumber_;
  if (isHistoryEnabled_)
    {
      inputStrategy_.store(inP);
      outputStrategy_.store(result);
    }
  return result;
}

/* Gradient according to the marginal parameters.
 *
 * F is the CDF of the ith marginal input distribution
 * Q is the quantile function of the ith output distribution
 *
 * F : RxRs -> [0,1]
 *    (x, pf) -> F(x, pf)
 *
 * Q : [0,1]xRt -> R
 *    (y, pq) -> G(y, pq)
 *
 * Let p = [pg, pf] be the parameter vector of H, with:
 *
 * H : RmxRt+s -> Rn
 *     (x, p)  -> Q(F(x, pq), pf)
 *
 * We have:
 *
 * (dH/dp)(x, p) = [(dQ/dy)(F(x, pf), pq) . (dF/dpf)(x, pf), 0] + [0, (dQ/dpq)(F(x, pf), pq)]
 *
 * The computation of (dQ/dy) leads to:
 *
 * (dQ/dy)(y, pq) = 1 / (pdf(Q(y, pq)))
 *
 * where pdf is the PDF of the distribution associated with the quantile function Q.
 *
 * The computation of (dQ/dpq) leads to:
 *
 * (dQ/dpq)(y, pq) = -(dcdf/dpq)(Q(y, pq), pq) / pdf(Q(y, pq), pq)
 *
 * where dcdf/dpq is the gradient according to its parameters of the CDF of the distribution
 * associated with the quantile function Q.
 *
 * the needed gradient is [(dH/dp)(x,p)]^t
 *
 */
Matrix MarginalTransformationEvaluation::parametersGradient(const NumericalPoint & inP) const
{
  const NumericalPoint parameters(getParameters());
  const UnsignedLong parametersDimension(parameters.getDimension());
  const UnsignedLong inputDimension(getInputDimension());
  Matrix result(parametersDimension, inputDimension);
  UnsignedLong rowIndex(0);
  switch (direction_)
    {
    case FROM:
      /*
       * Here, we suppose that pq is empty, so dQ/dpq = 0
       * For each row, store (dF/dpf)(x, pf) / pdf(Q(F(x, pf)))
       */
      for (UnsignedLong j = 0; j < inputDimension; ++j)
        {
          const NumericalPoint x(1, inP[j]);
          const Distribution inputMarginal(inputDistributionCollection_[j]);
          const Distribution outputMarginal(outputDistributionCollection_[j]);
          const NumericalScalar denominator(outputMarginal.computePDF(outputMarginal.computeQuantile(inputMarginal.computeCDF(x))));
          if (denominator > 0.0)
            {
              try
                {
                  const NumericalPoint normalizedCDFGradient(inputMarginal.computeCDFGradient(x) * (1.0 / denominator));
                  const UnsignedLong marginalParametersDimension(normalizedCDFGradient.getDimension());
                  for (UnsignedLong i = 0; i < marginalParametersDimension; ++i)
                    {
                      result(rowIndex, j) = normalizedCDFGradient[i];
                      ++rowIndex;
                    }
                }
              catch (NotYetImplementedException & ex)
                {
                  LOGWARN(OSS() << "Cannot compute the gradient according to the parameters of the " << j << "th marginal distribution");
                }
            }
        } // FROM
      return result;
    case TO:
      /*
       * Here, we suppose that pf is empty, so dF/dpf = 0
       * For each row, store -(dcdf/dpq)(Q(F(x), pq), pq) / pdf(Q(F(x), pq), pq)
       */
      for (UnsignedLong j = 0; j < inputDimension; ++j)
        {
          const NumericalPoint x(1, inP[j]);
          const Distribution inputMarginal(inputDistributionCollection_[j]);
          const Distribution outputMarginal(outputDistributionCollection_[j]);
          const NumericalPoint q(outputMarginal.computeQuantile(inputMarginal.computeCDF(x)));
          const NumericalScalar denominator(outputMarginal.computePDF(q));
          if (denominator > 0.0)
            {
              try
                {
                  const NumericalPoint normalizedCDFGradient(outputMarginal.computeCDFGradient(q) * (-1.0 / denominator));
                  const UnsignedLong marginalParametersDimension(normalizedCDFGradient.getDimension());
                  for (UnsignedLong i = 0; i < marginalParametersDimension; ++i)
                    {
                      result(rowIndex, j) = normalizedCDFGradient[i];
                      ++rowIndex;
                    }
                }
              catch (NotYetImplementedException & ex)
                {
                  LOGWARN(OSS() << "Cannot compute the gradient according to the parameters of the " << j << "th marginal distribution");
                }
            }
        } // TO
      return result;
    default:
      // Should never go there
      throw NotYetImplementedException(HERE);
    }
}

/* Accessor for input point dimension */
UnsignedLong MarginalTransformationEvaluation::getInputDimension() const
{
  return inputDistributionCollection_.getSize();
}

/* Accessor for output point dimension */
UnsignedLong MarginalTransformationEvaluation::getOutputDimension() const
{
  return inputDistributionCollection_.getSize();
}

/* Direction accessor */
void MarginalTransformationEvaluation::setDirection(const TranformationDirection direction)
{
  direction_ = direction;
}

UnsignedLong MarginalTransformationEvaluation::getDirection() const
{
  return direction_;
}

/* Input distribution collection accessor */
void MarginalTransformationEvaluation::setInputDistributionCollection(const DistributionCollection & inputDistributionCollection)
{
  inputDistributionCollection_ = inputDistributionCollection;
}

MarginalTransformationEvaluation::DistributionCollection MarginalTransformationEvaluation::getInputDistributionCollection() const
{
  return inputDistributionCollection_;
}

/* Output distribution collection accessor */
void MarginalTransformationEvaluation::setOutputDistributionCollection(const DistributionCollection & outputDistributionCollection)
{
  outputDistributionCollection_ = outputDistributionCollection;
}

MarginalTransformationEvaluation::DistributionCollection MarginalTransformationEvaluation::getOutputDistributionCollection() const
{
  return outputDistributionCollection_;
}

/* Simplifications accessor */
Collection<UnsignedLong> MarginalTransformationEvaluation::getSimplifications() const
{
  return simplifications_;
}

/* expressions accessor */
Collection<NumericalMathFunction> MarginalTransformationEvaluation::getExpressions() const
{
  return expressions_;
}

/* String converter */
String MarginalTransformationEvaluation::__repr__() const
{
  OSS oss;
  oss << "class=" << MarginalTransformationEvaluation::GetClassName()
      << " description=" << getDescription()
      << " input marginals=" << inputDistributionCollection_
      << " output marginals=" << outputDistributionCollection_
      << " simplifications=" << simplifications_
      << " expressions=" << expressions_.__repr__();
  return oss;
}

String MarginalTransformationEvaluation::__str__(const String & offset) const
{
  OSS oss;
  const String name(getName());
  if (hasVisibleName()) oss << offset << "Marginal transformation " << getName() << " :\n";
  const Description inputDescription(getInputDescription());
  const Description outputDescription(getOutputDescription());
  UnsignedLong length(0);
  for (UnsignedLong i = 0; i < inputDistributionCollection_.getSize(); ++i)
    {
      const UnsignedLong l(outputDescription[i].length());
      if (l > length) length = l;
    }
  for (UnsignedLong i = 0; i < inputDistributionCollection_.getSize(); ++i)
    {
      oss << offset;
      if (inputDistributionCollection_.getSize() > 1) oss << "| " << std::setw(length) << outputDescription[i] << " = ";
      if (simplifications_[i]) oss << expressions_[i].getEvaluationImplementation()->__str__() << "\n";
      else oss << inputDistributionCollection_[i].__str__() << " -> " << outputDescription[i] << " : " << outputDistributionCollection_[i].__str__() << "\n";
    }
  return oss;
}

/* Method save() stores the object through the StorageManager */
void MarginalTransformationEvaluation::save(Advocate & adv) const
{
  NumericalMathEvaluationImplementation::save(adv);
  adv.saveAttribute( "inputDistributionCollection_", inputDistributionCollection_ );
  adv.saveAttribute( "outputDistributionCollection_", outputDistributionCollection_ );
  adv.saveAttribute( "direction_", direction_ );
}

/* Method load() reloads the object from the StorageManager */
void MarginalTransformationEvaluation::load(Advocate & adv)
{
  NumericalMathEvaluationImplementation::load(adv);
  adv.loadAttribute( "inputDistributionCollection_", inputDistributionCollection_ );
  adv.loadAttribute( "outputDistributionCollection_", outputDistributionCollection_ );
  adv.loadAttribute( "direction_", direction_ );
  *this = MarginalTransformationEvaluation(inputDistributionCollection_, outputDistributionCollection_, direction_);
}

END_NAMESPACE_OPENTURNS

