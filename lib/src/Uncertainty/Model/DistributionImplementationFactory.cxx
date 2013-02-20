//                                               -*- C++ -*-
/**
 *  @file  DistributionImplementationFactory.cxx
 *  @brief Top-level class for all distribution factories
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
 *  @author schueller
 *  @date   2012-04-18 17:56:46 +0200 (Wed, 18 Apr 2012)
 */
#include <cstdlib>
#include <iomanip>
#include <fstream>
#include "DistributionImplementationFactory.hxx"
#include "Description.hxx"
#include "Path.hxx"
#include "Exception.hxx"
#include "OTconfig.hxx"
#include "Log.hxx"
#include "Os.hxx"
#include "BootstrapExperiment.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(DistributionImplementationFactory);

/* Default constructor */
DistributionImplementationFactory::DistributionImplementationFactory(const UnsignedLong bootstrapSize,
                                                                     const String & name)
  : PersistentObject(name)
  , bootstrapSize_(bootstrapSize)
{
  // Nothing to do
}

/* Virtual constructor */
DistributionImplementationFactory * DistributionImplementationFactory::clone() const
{
  return new DistributionImplementationFactory(*this);
}

/* String converter */
String DistributionImplementationFactory::__repr__() const
{
  OSS oss;
  oss << "class=" << DistributionImplementationFactory::GetClassName();
  return oss;
}

/* String converter */
String DistributionImplementationFactory::__str__(const String & offset) const
{
  return this->getClassName();
}


/* Here is the interface that all derived class must implement */

/* Build a distribution based on a sample */
DistributionImplementationFactory::Implementation DistributionImplementationFactory::build(const NumericalSample & sample) const
{
  throw NotYetImplementedException(HERE);
}

/* Build a distribution based on a sample and gives the covariance matrix of the estimate */
DistributionImplementationFactory::Implementation DistributionImplementationFactory::build(const NumericalSample & sample,
                                                                                           CovarianceMatrix & covariance) const
{
  /* The bootstrap sampler */
  BootstrapExperiment bootstrap(sample);
  /* Build the distribution based on the given sample */
  DistributionImplementationFactory::Implementation distribution(build(sample));
  const UnsignedLong bootstrapSize(ResourceMap::GetAsUnsignedLong("DistributionImplementationFactory-DefaultBootstrapSize"));
  NumericalSample parametersSample(bootstrapSize, ParametersAsNumericalPoint(distribution->getParametersCollection()));
  for (UnsignedLong i = 1; i < bootstrapSize; ++i)
    {
      /* Draw a bootstrap sample */
      const NumericalSample bootstrapSample(bootstrap.generate());
      /* Build the associated distribution */
      const DistributionImplementation::Implementation newDistribution(build(bootstrapSample));
      /* Add the parameters to the parameters sample */
      parametersSample.add(ParametersAsNumericalPoint(newDistribution->getParametersCollection()));
    }
  /* Compute the bootstrap covariance */
  covariance = parametersSample.computeCovariance();
  return distribution;
}


/* Build a distribution based on a set of parameters */
DistributionImplementationFactory::Implementation DistributionImplementationFactory::build(const NumericalPointCollection & parameters) const
{
  throw NotYetImplementedException(HERE);
}

DistributionImplementationFactory::Implementation DistributionImplementationFactory::build(const NumericalPointWithDescriptionCollection & parameters) const
{
  return build(RemoveDescriptionFromCollection(parameters));
}

/* Build a distribution using its default constructor */
DistributionImplementationFactory::Implementation DistributionImplementationFactory::build() const
{
  throw NotYetImplementedException(HERE);
}

/* Execute a R script */
NumericalPoint DistributionImplementationFactory::runRFactory(const NumericalSample & sample,
                                                              const DistributionImplementation & distribution) const
{
  const String distributionName(distribution.getClassName());
  if (sample.getDimension() != 1) throw InvalidArgumentException(HERE) << "Error: can build a distribution of type " << distributionName << " using R methods";
  const String dataFileName(sample.storeToTemporaryFile());
  const String resultFileName(Path::BuildTemporaryFileName("RResult.txt.XXXXXX"));
  const String commandFileName(Path::BuildTemporaryFileName("RCmd.R.XXXXXX"));
  std::ofstream cmdFile(commandFileName.c_str(), std::ios::out);
  // Fill-in the command file
  cmdFile << "library(rot)" << std::endl;
  cmdFile << "options(digits=17)" << std::endl;
  cmdFile << "options(warn=-1)" << std::endl;
  cmdFile << "sample <- data.matrix(read.table(\"" << dataFileName << "\"))" << std::endl;
  cmdFile << "res <- estimate" << distributionName << "Parameters(sample)" << std::endl;
  cmdFile << "f <- file(\"" << resultFileName << "\",\"wt\")" << std::endl;
  cmdFile << "cat(";
  const Description parametersDescription(distribution.getParametersCollection()[0].getDescription());
  const UnsignedLong parametersNumber(parametersDescription.getSize());
  for (UnsignedLong i = 0; i < parametersNumber; ++i) cmdFile << "res$" << parametersDescription[i] << ", ";
  cmdFile << "sep=\"\\n\", file=f)" << std::endl;
  cmdFile << "close(f)" << std::endl;
  cmdFile.close();
  const String RExecutable(ResourceMap::Get("R-executable-command"));
  OSS systemCommand;
  if (RExecutable != "") systemCommand << RExecutable << " --no-save --silent < \"" << commandFileName << "\"" << Os::GetDeleteCommandOutput();
  else throw NotYetImplementedException(HERE) << "DistributionImplementationFactory::runRFactory() needs R. Please install it and set the absolute path of the R executable in ResourceMap.";
  int returnCode(Os::ExecuteCommand(systemCommand));
  if (returnCode != 0) throw InternalException(HERE) << "Error: unable to execute the system command " << String(systemCommand) << " returned code is " << returnCode;
  // Parse result file
  std::ifstream resultFile(resultFileName.c_str(), std::ios::in);
  NumericalPoint parametersValue(parametersNumber);
  for (UnsignedLong i = 0; i < parametersNumber; ++i) resultFile >> parametersValue[i];
  if (remove(dataFileName.c_str()) == -1) LOGWARN(OSS() << "Warning: cannot delete file " << dataFileName);
  if (remove(resultFileName.c_str()) == -1) LOGWARN(OSS() << "Warning: cannot delete file " << resultFileName);
  if (remove(commandFileName.c_str()) == -1) LOGWARN(OSS() << "Warning: cannot delete file " << commandFileName);
  return parametersValue;
}

/* Convert a NumericalPointWithDescriptionCollection into a NumericalPointCollection */
DistributionImplementationFactory::NumericalPointCollection DistributionImplementationFactory::RemoveDescriptionFromCollection(const NumericalPointWithDescriptionCollection & coll)
{
  const UnsignedLong size(coll.getSize());
  NumericalPointCollection newColl(size);
  for (UnsignedLong i = 0; i < size; ++i) newColl[i] = coll[i];
  return newColl;
}

/* Convert a NumericalPointCollection into a NumericalPointWithDescriptionCollection */
DistributionImplementationFactory::NumericalPointWithDescriptionCollection DistributionImplementationFactory::AddDescriptionToCollection(const NumericalPointCollection & coll)
{
  const UnsignedLong size(coll.getSize());
  NumericalPointWithDescriptionCollection newColl(size);
  for (UnsignedLong i = 0; i < size; ++i) newColl[i] = coll[i];
  return newColl;
}

/* Convert a parameters collection into a NumericalPoint */
NumericalPoint DistributionImplementationFactory::ParametersAsNumericalPoint(const NumericalPointWithDescriptionCollection & parameters)
{
  const UnsignedLong size(parameters.getSize());
  if (size == 0) throw InvalidArgumentException(HERE) << "Error: cannot convert an empty collection of parameters.";
  // Early exit if dimension 1
  if (size == 1) return parameters[0];
  // Concatenate the parameters
  NumericalPoint allParameters(0);
  for (UnsignedLong i = 0; i < size; ++i)
    {
      const NumericalPoint marginalParameters(parameters[i]);
      const UnsignedLong marginalSize(marginalParameters.getSize());
      for (UnsignedLong j = 0; j < marginalSize; ++j) allParameters.add(marginalParameters[j]);
    }
  return allParameters;
}

/* Bootstrap size accessor */
UnsignedLong DistributionImplementationFactory::getBootstrapSize() const
{
  return bootstrapSize_;
}

void DistributionImplementationFactory::setBootstrapSize(const UnsignedLong bootstrapSize)
{
  if (bootstrapSize == 0) throw InvalidArgumentException(HERE) << "Error: the bootstrap size must be > 0.";
  bootstrapSize_ = bootstrapSize;
}

END_NAMESPACE_OPENTURNS
