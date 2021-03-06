//                                               -*- C++ -*-
/**
 *  @file  StandardDistributionPolynomialFactory.cxx
 *  @brief OrthogonalUniVariatePolynomialStandardDistribution polynomial factory
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
 *  @date   2008-05-21 11:21:38 +0200 (Wed, 21 May 2008)
 */

#include "StandardDistributionPolynomialFactory.hxx"
#include "PersistentObjectFactory.hxx"
#include "Uniform.hxx"
#include "GramSchmidtAlgorithm.hxx"
#include "CharlierFactory.hxx"
#include "HermiteFactory.hxx"
#include "JacobiFactory.hxx"
#include "KrawtchoukFactory.hxx"
#include "LaguerreFactory.hxx"
#include "LegendreFactory.hxx"
#include "MeixnerFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

CLASSNAMEINIT(StandardDistributionPolynomialFactory);

static Factory<StandardDistributionPolynomialFactory> RegisteredFactory("StandardDistributionPolynomialFactory");


/* Default constructor */
StandardDistributionPolynomialFactory::StandardDistributionPolynomialFactory()
  : OrthogonalUniVariatePolynomialFactory(Uniform()),
    orthonormalizationAlgorithm_(GramSchmidtAlgorithm(Uniform())),
    specificFamily_(),
    hasSpecificFamily_(false)
{
  // Initialize the coefficient cache
  initializeCache();
}


/* Parameter constructor */
StandardDistributionPolynomialFactory::StandardDistributionPolynomialFactory(const Distribution & measure)
  : OrthogonalUniVariatePolynomialFactory(measure.getStandardRepresentative()),
    orthonormalizationAlgorithm_(GramSchmidtAlgorithm(measure.getStandardRepresentative())),
    specificFamily_(),
    hasSpecificFamily_(false)
{
  checkSpecificFamily();
  initializeCache();
}


/* Parameter constructor */
StandardDistributionPolynomialFactory::StandardDistributionPolynomialFactory(const OrthonormalizationAlgorithm & orthonormalizationAlgorithm)
  : OrthogonalUniVariatePolynomialFactory(orthonormalizationAlgorithm.getMeasure().getStandardRepresentative()),
    orthonormalizationAlgorithm_(orthonormalizationAlgorithm),
    specificFamily_(),
    hasSpecificFamily_(false)
{
  checkSpecificFamily();
  initializeCache();
}


/* Virtual constructor */
StandardDistributionPolynomialFactory * StandardDistributionPolynomialFactory::clone() const
{
  return new StandardDistributionPolynomialFactory(*this);
}


/* Calculate the coefficients of recurrence a0n, a1n, a2n such that
   Pn+1(x) = (a0n * x + a1n) * Pn(x) + a2n * Pn-1(x) */
StandardDistributionPolynomialFactory::Coefficients StandardDistributionPolynomialFactory::getRecurrenceCoefficients(const UnsignedLong n) const
{
  if (hasSpecificFamily_) return specificFamily_.getRecurrenceCoefficients(n);
  return orthonormalizationAlgorithm_.getRecurrenceCoefficients(n);
}

/* Check the existence of a specific family more efficient for the given measure */
void StandardDistributionPolynomialFactory::checkSpecificFamily()
{
  // Check for special cases. Need a more elegant conception and implementation.
  hasSpecificFamily_ = false;
  const String measureType(measure_.getImplementation()->getClassName());
  // Legendre factory
  if (measureType == "Uniform")
    {
      specificFamily_ = LegendreFactory();
      hasSpecificFamily_ = true;
      return;
    }
  // Hermite factory
  if (measureType == "Normal")
    {
      specificFamily_ = HermiteFactory();
      hasSpecificFamily_ = true;
      return;
    }
  // Jacobi factory
  if (measureType == "Beta")
    {
      const NumericalPoint parameters(measure_.getParametersCollection()[0]);
      specificFamily_ = JacobiFactory(parameters[1] - parameters[0] - 1.0, parameters[0] - 1.0);
      hasSpecificFamily_ = true;
      return;
    }
  // Laguerre factory
  if (measureType == "Gamma")
    {
      const NumericalPoint parameters(measure_.getParametersCollection()[0]);
      specificFamily_ = LaguerreFactory(parameters[0] - 1.0);
      hasSpecificFamily_ = true;
      return;
    }
  // Charlier factory
  if (measureType == "Poisson")
    {
      const NumericalPoint parameters(measure_.getParametersCollection()[0]);
      specificFamily_ = CharlierFactory(parameters[0]);
      hasSpecificFamily_ = true;
      return;
    }
  // Krawtchouk factory
  if (measureType == "Binomial")
    {
      const NumericalPoint parameters(measure_.getParametersCollection()[0]);
      specificFamily_ = KrawtchoukFactory(static_cast<UnsignedLong>(parameters[0]), parameters[1]);
      hasSpecificFamily_ = true;
      return;
    }
  // Meixner factory
  if (measureType == "NegativeBinomial")
    {
      const NumericalPoint parameters(measure_.getParametersCollection()[0]);
      specificFamily_ = MeixnerFactory(parameters[0], parameters[1]);
      hasSpecificFamily_ = true;
      return;
    }
}

/* String converter */
String StandardDistributionPolynomialFactory::__repr__() const
{
  return OSS() << "class=" << getClassName()
               << " orthonormalization algorithm=" << orthonormalizationAlgorithm_;
}

/* Method save() stores the object through the StorageManager */
void StandardDistributionPolynomialFactory::save(Advocate & adv) const
{
  OrthogonalUniVariatePolynomialFactory::save(adv);
  adv.saveAttribute( "orthonormalizationAlgorithm_", orthonormalizationAlgorithm_ );
  adv.saveAttribute( "specificFamily_", specificFamily_ );
  adv.saveAttribute( "hasSpecificFamily_", hasSpecificFamily_ );
}


/* Method load() reloads the object from the StorageManager */
void StandardDistributionPolynomialFactory::load(Advocate & adv)
{
  OrthogonalUniVariatePolynomialFactory::load(adv);
  adv.loadAttribute( "orthonormalizationAlgorithm_", orthonormalizationAlgorithm_ );
  adv.loadAttribute( "specificFamily_", specificFamily_ );
  adv.loadAttribute( "hasSpecificFamily_", hasSpecificFamily_ );
}


END_NAMESPACE_OPENTURNS
