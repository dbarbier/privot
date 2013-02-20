//                                               -*- C++ -*-
/**
 *  @file  InverseNatafEllipticalCopulaHessian.cxx
 *  @brief Class for the InverseNataf transformation evaluation for elliptical
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
#include "InverseNatafEllipticalCopulaHessian.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class InverseNatafEllipticalCopulaHessian
 *
 * This class offers an interface for the InverseNataf function for elliptical copula
 */

CLASSNAMEINIT(InverseNatafEllipticalCopulaHessian);

static Factory<InverseNatafEllipticalCopulaHessian> RegisteredFactory("InverseNatafEllipticalCopulaHessian");

/* Default constructor */
InverseNatafEllipticalCopulaHessian::InverseNatafEllipticalCopulaHessian():
  NumericalMathHessianImplementation(),
  standardDistribution_(),
  cholesky_()
{
  // Nothing to do
}

/* Parameter constructor */
InverseNatafEllipticalCopulaHessian::InverseNatafEllipticalCopulaHessian(const Distribution & standardDistribution,
                                                                         const SquareMatrix & cholesky):
  NumericalMathHessianImplementation(),
  standardDistribution_(standardDistribution),
  cholesky_(cholesky)
{
  // Nothing to do
}

/* Virtual constructor */
InverseNatafEllipticalCopulaHessian * InverseNatafEllipticalCopulaHessian::clone() const
{
  return new InverseNatafEllipticalCopulaHessian(*this);
}

/* String converter */
String InverseNatafEllipticalCopulaHessian::__repr__() const
{
  OSS oss;
  oss << "class=" << InverseNatafEllipticalCopulaHessian::GetClassName()
      << " standardDistribution=" << standardDistribution_
      << " cholesky=" << cholesky_;

  return oss;
}

/*
 * Evaluation
 * The elliptical copula has a correlation matrix R. The inverse Nataf transform S reads:
 * Z(u) = L.u, where L is the Cholesky factor of R: L.L^t = R, L is lower triangular
 * Si(u) = F(Zi), where F is the CDF of the standard elliptical distribution
 * Its Jacobian J is:
 * Jij = dSi/duj = (dF/du)ij
 *               = (F'(Zi)dZ/du)ij
 *               = F'(Zi)Lij
 * thus, (DS)ij = Jji = LjiF'(Zj)
 * One step further, we have:
 * Hijk = d2Si/dxjdxk = (d2F/dudu')ijk
 *                    = (d(F'(Zi)dZ/du)/du')ijk
 *                    = (F''(Zi)(dZ/du)(dZ/du'))ijk
 *                    = (F''(Zi)LijLik
 * Thus, (D2T)ijk = Hkji = LkjLkiF''(Zk)
 */
SymmetricTensor InverseNatafEllipticalCopulaHessian::hessian(const NumericalPoint & inP) const
{
  UnsignedLong dimension(getInputDimension());
  // First, correlate the components
  NumericalPoint point(cholesky_ * inP);
  SymmetricTensor result(dimension, dimension);
  const Distribution standardMarginal(standardDistribution_.getMarginal(0));
  // Then, apply standard marginal transformation
  for (UnsignedLong k = 0; k < dimension; ++k)
    {
      NumericalScalar ddf(standardMarginal.computeDDF(NumericalPoint(1, point[k]))[0]);
      for (UnsignedLong i = 0; i <= k; ++i)
        {
          NumericalScalar factor(ddf * cholesky_(k, i));
          for (UnsignedLong j = 0; j <= k; ++j)
            {
              result(i, j, k) = cholesky_(k, j) * factor;
            }
        }
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong InverseNatafEllipticalCopulaHessian::getInputDimension() const
{
  return cholesky_.getNbColumns();
}

/* Accessor for output point dimension */
UnsignedLong InverseNatafEllipticalCopulaHessian::getOutputDimension() const
{
  return cholesky_.getNbRows();
}

/* Method save() stores the object through the StorageManager */
void InverseNatafEllipticalCopulaHessian::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "standardDistribution_", standardDistribution_ );
  adv.saveAttribute( "cholesky_", cholesky_ );
}

/* Method load() reloads the object from the StorageManager */
void InverseNatafEllipticalCopulaHessian::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "standardDistribution_", standardDistribution_ );
  adv.loadAttribute( "cholesky_", cholesky_ );
}

END_NAMESPACE_OPENTURNS
