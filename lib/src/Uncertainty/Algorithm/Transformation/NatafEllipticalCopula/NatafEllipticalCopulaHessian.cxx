//                                               -*- C++ -*-
/**
 *  @file  NatafEllipticalCopulaHessian.cxx
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
#include "NatafEllipticalCopulaHessian.hxx"
#include "PersistentObjectFactory.hxx"

BEGIN_NAMESPACE_OPENTURNS

/*
 * @class NatafEllipticalCopulaHessian
 *
 * This class offers an interface for the Nataf function for elliptical copula
 */

CLASSNAMEINIT(NatafEllipticalCopulaHessian);

static Factory<NatafEllipticalCopulaHessian> RegisteredFactory("NatafEllipticalCopulaHessian");

/* Default constructor */
NatafEllipticalCopulaHessian::NatafEllipticalCopulaHessian():
  NumericalMathHessianImplementation(),
  standardDistribution_(),
  inverseCholesky_()
{
  // Nothing to do
}

/* Parameter constructor */
NatafEllipticalCopulaHessian::NatafEllipticalCopulaHessian(const Distribution & standardDistribution,
                                                           const SquareMatrix & inverseCholesky):
  NumericalMathHessianImplementation(),
  standardDistribution_(standardDistribution),
  inverseCholesky_(inverseCholesky)
{
  // Nothing to do
}

/* Virtual constructor */
NatafEllipticalCopulaHessian * NatafEllipticalCopulaHessian::clone() const
{
  return new NatafEllipticalCopulaHessian(*this);
}

/* String converter */
String NatafEllipticalCopulaHessian::__repr__() const
{
  OSS oss;
  oss << "class=" << NatafEllipticalCopulaHessian::GetClassName()
      << " standardDistribution=" << standardDistribution_
      << " inverseCholesky=" << inverseCholesky_;

  return oss;
}

/*
 * Evaluation
 * The elliptical copula has a correlation matrix R. The Nataf transform T reads:
 * Yi(x) = Q(xi), where Q = F^{-1} and F is the CDF of the standard elliptical distribution
 * T(x) = G.Y(x), where G = L^{-1} and L is the Cholesky factor of R: L.L^t = R, L is lower triangular
 * Its Jacobian J is:
 * Jij = dTi/dxj = (G.dY/dx)ij
 *               = (G.diag(Q'(xk), k=1...n))ij
 *               = (G:,1Q'(x1)|...|G:,nQ'(xn))ij
 *               = GijQ'(xj)
 * thus, (DT)ij = Jji = GjiQ'(xi)
 * One step further, we have:
 * Hijk = d2Ti/dxjdxk = (G.d2Y/dxdx')ijk
 *                    = (G.diag(Q''(xl), l=1..n))ijk
 *                    = GijQ''(xj) if k = j
 *                    = 0 else
 * Thus, (D2T)ijk = Hkji = GjiQ''(xi) if k = j, else 0: each sheet of D2T is a diagonal matrix diag(GjiQ''(xi), i=1..n)
 */
SymmetricTensor NatafEllipticalCopulaHessian::hessian(const NumericalPoint & inP) const
{
  UnsignedLong dimension(getInputDimension());
  NumericalPoint quantileSecondDerivative(dimension);
  const Distribution standardMarginal(standardDistribution_.getMarginal(0));
  SymmetricTensor result(dimension, dimension);
  for (UnsignedLong i = 0; i < dimension; ++i)
    {
      NumericalPoint q(standardMarginal.computeQuantile(inP[i]));
      NumericalScalar factor(1.0 / standardMarginal.computePDF(q));
      NumericalScalar quantileSecondDerivative(-standardMarginal.computeDDF(q)[0] * factor * factor * factor);
      // inverseCholesky_ is lower triangular
      for (UnsignedLong j = i; j < dimension; ++j)
        {
          result(i, i, j) = inverseCholesky_(j, i) * quantileSecondDerivative;
        }
    }
  return result;
}

/* Accessor for input point dimension */
UnsignedLong NatafEllipticalCopulaHessian::getInputDimension() const
{
  return inverseCholesky_.getNbColumns();
}

/* Accessor for output point dimension */
UnsignedLong NatafEllipticalCopulaHessian::getOutputDimension() const
{
  return inverseCholesky_.getNbRows();
}

/* Method save() stores the object through the StorageManager */
void NatafEllipticalCopulaHessian::save(Advocate & adv) const
{
  NumericalMathHessianImplementation::save(adv);
  adv.saveAttribute( "standardDistribution_", standardDistribution_ );
  adv.saveAttribute( "inverseCholesky_", inverseCholesky_ );
}

/* Method load() reloads the object from the StorageManager */
void NatafEllipticalCopulaHessian::load(Advocate & adv)
{
  NumericalMathHessianImplementation::load(adv);
  adv.loadAttribute( "standardDistribution_", standardDistribution_ );
  adv.loadAttribute( "inverseCholesky_", inverseCholesky_ );
}

END_NAMESPACE_OPENTURNS
