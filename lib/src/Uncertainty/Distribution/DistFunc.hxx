//                                               -*- C++ -*-
/**
 *  @file  DistFunc.hxx
 *  @brief OpenTURNS wrapper to a library of special functions
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
 *  @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)
 *  Id      DistFunc.hxx 2604 2012-07-16 10:24:33Z schueller
 */
#ifndef OPENTURNS_DISTFUNC_HXX
#define OPENTURNS_DISTFUNC_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS



class DistFunc
{
public:
  static const UnsignedLong NumberOfBandNormalZigurrat;
  static const NumericalScalar NormalZigguratTail;
  // The array NormalZigguratAbscissa stores the abscissas of the bands:
  // the ith band is [NormalZigguratAbscissa[0], NormalZigguratAbscissa[i+1]] with
  // NormalZigguratAbscissa[0] = 0.0 and
  // NormalZigguratAbscissa[NumberOfBandNormalZigurrat] = NormalZigguratTail
  static const NumericalScalar NormalZigguratAbscissa[];
  // The array NormalZigguratRatio stores the ratio between a band and the next one,
  // starting from the smallest one:
  // NormalZigguratRatio[i] = NormalZigguratAbscissa[i] / NormalZigguratAbscissa[i+1]
  static const NumericalScalar NormalZigguratRatio[];

  // For Beta distribution. WARNING: here we adopt the classical definition
  // If tail=false, compute P(X<=x), else P(X>x)
  static NumericalScalar pBeta(const NumericalScalar p1,
                               const NumericalScalar p2,
                               const NumericalScalar x,
                               const Bool tail = false);
  // If tail=false, compute x such that P(X<=x)=q, else x such that P(X>x)=q
  static NumericalScalar qBeta(const NumericalScalar p1,
                               const NumericalScalar p2,
                               const NumericalScalar q,
                               const Bool tail = false);
  static NumericalScalar rBeta(const NumericalScalar p1,
                               const NumericalScalar p2);
  // For the Binomial distribution
private:
  static NumericalScalar fcBinomial(const UnsignedLong k);

public:
  static UnsignedLong rBinomial(const UnsignedLong n,
                                const NumericalScalar p);
  // For Gamma distribution
  static NumericalScalar pGamma(const NumericalScalar k,
                                const NumericalScalar x,
                                const Bool tail = false);
  static NumericalScalar qGamma(const NumericalScalar k,
                                const NumericalScalar q,
                                const Bool tail = false);
  static NumericalScalar rGamma(const NumericalScalar k);
  // For Kolmogorov distribution
  static NumericalScalar pKolmogorov(const UnsignedLong n,
                                     const NumericalScalar x,
                                     const Bool tail = false);
  static NumericalScalar pKolmogorovAsymptotic(const NumericalScalar x,
                                               const Bool tail = false);
  // For NonCentralChiSquare distribution
  static NumericalScalar dNonCentralChiSquare(const NumericalScalar nu,
                                              const NumericalScalar lambda,
                                              const NumericalScalar x);
  static NumericalScalar pNonCentralChiSquare(const NumericalScalar nu,
                                              const NumericalScalar lambda,
                                              const NumericalScalar x,
                                              const Bool tail = false);
  static NumericalScalar rNonCentralChiSquare(const NumericalScalar nu,
                                              const NumericalScalar lambda);
  // For NonCentralStudent distribution
  static NumericalScalar dNonCentralStudent(const NumericalScalar nu,
                                            const NumericalScalar delta,
                                            const NumericalScalar x);
  static NumericalScalar pNonCentralStudent(const NumericalScalar nu,
                                            const NumericalScalar delta,
                                            const NumericalScalar x,
                                            const Bool tail = false);
  static NumericalScalar rNonCentralStudent(const NumericalScalar nu,
                                            const NumericalScalar delta);
  // For Normal distribution
  static NumericalScalar pNormal(const NumericalScalar x,
                                 const Bool tail = false);
  static NumericalScalar pNormal2D(const NumericalScalar x1,
                                   const NumericalScalar x2,
                                   const NumericalScalar rho,
                                   const Bool tail = false,
                                   const Bool newAlgo = true);
  static NumericalScalar pNormal3D(const NumericalScalar x1,
                                   const NumericalScalar x2,
                                   const NumericalScalar x3,
                                   const NumericalScalar rho12,
                                   const NumericalScalar rho13,
                                   const NumericalScalar rho23,
                                   const Bool tail = false,
                                   const Bool newAlgo = true);
  static NumericalScalar qNormal(const NumericalScalar q,
                                 const Bool tail = false);
  static NumericalScalar rNormal();
  // For Poisson distribution
  static NumericalScalar rPoisson(const NumericalScalar lambda);
  // For Student distribution
  static NumericalScalar pStudent(const NumericalScalar nu,
                                  const NumericalScalar x,
                                  const Bool tail = false);
  static NumericalScalar qStudent(const NumericalScalar nu,
                                  const NumericalScalar q,
                                  const Bool tail = false);
  static NumericalScalar rStudent(const NumericalScalar nu);
  // For TruncatedNormal distribution
  //      static NumericalScalar rTruncatedNormal(const NumericalScalar a, const NumericalScalar b);
  // Compute the expectation of the min of n independent standard normal random variables
  static NumericalScalar eZ1(const UnsignedLong n);

  // Asymptotic distribution of the DickeyFuller distribution
  static NumericalScalar pDickeyFullerTrend(const NumericalScalar x,
                                            Bool tail = false);

  static NumericalScalar pDickeyFullerConstant(const NumericalScalar x,
                                               Bool tail = false);

  static NumericalScalar pDickeyFullerNoConstant(const NumericalScalar x,
                                                 Bool tail = false);

  static NumericalScalar qDickeyFullerTrend(const NumericalScalar q,
                                            Bool tail = false);

  static NumericalScalar qDickeyFullerConstant(const NumericalScalar q,
                                               Bool tail = false);

  static NumericalScalar qDickeyFullerNoConstant(const NumericalScalar q,
                                                 Bool tail = false);

}; /* class DistFunc */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_DISTFUNC_HXX */
