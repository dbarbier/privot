//                                               -*- C++ -*-
/**
 *  @file  SpecFunc.hxx
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
 *  @date   2012-07-16 10:12:54 +0200 (Mon, 16 Jul 2012)
 *  Id      SpecFunc.hxx 2600 2012-07-16 08:12:54Z schueller
 */
#ifndef OPENTURNS_SPECFUNC_HXX
#define OPENTURNS_SPECFUNC_HXX

#include "OTprivate.hxx"

BEGIN_NAMESPACE_OPENTURNS



class SpecFunc
{

public:

  // 0.39894228040143267 = 1 / sqrt(2.pi)
  static const NumericalScalar ISQRT2PI;
  // 0.57721566490153286 = Euler constant gamma
  static const NumericalScalar EulerConstant;
  // 1.64493406684822643 = pi^2 / 6
  static const NumericalScalar PI2_6;
  // 1.28254983016118640 = pi / sqrt(6)
  static const NumericalScalar PI_SQRT6;
  // 0.45005320754569466 = gamma * sqrt(6) / pi
  static const NumericalScalar EULERSQRT6_PI;
  // 3.28986813369645287 = pi^2 / 3
  static const NumericalScalar PI2_3;
  // 0.55132889542179204 = sqrt(3) / pi
  static const NumericalScalar SQRT3_PI;
  // 1.81379936423421785 = pi / sqrt(3)
  static const NumericalScalar PI_SQRT3;
  // 1.20205690315959429 = Zeta(3)
  static const NumericalScalar ZETA3;
  // Minimum positive real number
  static const NumericalScalar MinNumericalScalar;
  static const NumericalScalar LogMinNumericalScalar;
  // Maximum positive real number
  static const NumericalScalar MaxNumericalScalar;
  static const NumericalScalar LogMaxNumericalScalar;
  // Real number accuracy
  static const NumericalScalar NumericalScalarEpsilon;

  // Modified first kind Bessel function of order 0: BesselI0(x) = \sum_{m=0}\infty\frac{1}{m!^2}\left(\frac{x}{2}\right)^{2m}
private:
  static NumericalScalar SmallCaseBesselI0(const NumericalScalar x);
  static NumericalScalar LargeCaseLogBesselI0(const NumericalScalar x);
public:
  static NumericalScalar BesselI0(const NumericalScalar x);
  static NumericalScalar LogBesselI0(const NumericalScalar x);
  // Modified first kind Bessel function of order 1: BesselI1(x) = \sum_{m=0}\infty\frac{1}{m!(m+1)!}\left(\frac{x}{2}\right)^{2m+1}
private:
  static NumericalScalar SmallCaseBesselI1(const NumericalScalar x);
  static NumericalScalar LargeCaseLogBesselI1(const NumericalScalar x);
public:
  static NumericalScalar BesselI1(const NumericalScalar x);
  static NumericalScalar LogBesselI1(const NumericalScalar x);
  // Beta function: beta(a, b) = \int_0^1 t^{a-1}.(1-t)^{b-1} dt
  static NumericalScalar Beta(const NumericalScalar a,
                              const NumericalScalar b);
  // Incomplete beta function: betaInc(a, b, x) = \int_0^x t^{a-1}.(1-t)^{b-1} dt
  static NumericalScalar BetaInc(const NumericalScalar a,
                                 const NumericalScalar b,
                                 const NumericalScalar x);
  // Incomplete beta function inverse with respect to x
  static NumericalScalar BetaIncInv(const NumericalScalar a,
                                    const NumericalScalar b,
                                    const NumericalScalar x);
  // Incomplete beta ratio function: betaRatioInc(a, b, x) = \int_0^x t^{a-1}.(1-t)^{b-1} dt / beta(a, b)
  static NumericalScalar BetaRatioInc(const NumericalScalar a,
                                      const NumericalScalar b,
                                      const NumericalScalar x);
  // Incomplete beta ratio function inverse with respect to x
  static NumericalScalar BetaRatioIncInv(const NumericalScalar a,
                                         const NumericalScalar b,
                                         const NumericalScalar x);
  // Natural logarithm of the beta function
  static NumericalScalar LnBeta(const NumericalScalar a,
                                const NumericalScalar b);
  static NumericalScalar LogBeta(const NumericalScalar a,
                                 const NumericalScalar b);
  // Dawson function: Dawson(x) = \exp(-x^2) * \int_0^x \exp(t^2) dt
  static NumericalScalar Dawson(const NumericalScalar x);
  // Debye function of order n: DebyeN(x, n) = n / x^n \int_0^x t^n/(\exp(t)-1) dt
  static NumericalScalar Debye(const NumericalScalar x,
                               const UnsignedLong n);
  // Real Faddeeva function: faddeeva(z) = exp(-z^2).erfc(-I*z)
  //      static NumericalComplex Faddeeva(const NumericalScalar x);
  // Complex Faddeeva function: faddeeva(z) = exp(-z^2)\erfc(-I*z)
  static NumericalComplex Faddeeva(const NumericalComplex & z);
  // Gamma function: gamma(a) = \int_0^{\infty} t^{a-1}\exp(-t) dt
  static NumericalScalar Gamma(const NumericalScalar a);
  // Complex gamma function: gamma(a) = \int_0^{\infty} t^{a-1}\exp(-t) dt
  static NumericalComplex Gamma(const NumericalComplex & a);
  // Natural logarithm of the gamma function
  static NumericalScalar LnGamma(const NumericalScalar a);
  static NumericalScalar LogGamma(const NumericalScalar a);
  // Incomplete gamma function: gamma(a, x) = \int_0^x t^{a-1}\exp(-t) dt
  static NumericalScalar GammaInc(const NumericalScalar a,
                                  const NumericalScalar x);
  // Incomplete gamma function inverse with respect to x
  static NumericalScalar GammaIncInv(const NumericalScalar a,
                                     const NumericalScalar x);
  // Digamma function: psi(x) = ((dgamma/dx) / gamma)(x)
  static NumericalScalar DiGamma(const NumericalScalar x);
  static NumericalScalar Psi(const NumericalScalar x);
  // Inverse of the DiGamma function
  static NumericalScalar DiGammaInv(const NumericalScalar a);
  // Trigamma function: TriGamma(x) = ((d^2gamma/dx^2) / gamma)(x)
  static NumericalScalar TriGamma(const NumericalScalar x);
  // Hypergeometric function of type (1,1): hyperGeom_1_1(p1, q1, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) / (q1 + k)] * x^n / n!
  static NumericalScalar HyperGeom_1_1(const NumericalScalar p1,
                                       const NumericalScalar q1,
                                       const NumericalScalar x);
  // Complex hypergeometric function of type (1,1): hyperGeom_1_1(p1, q1, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) / (q1 + k)] * x^n / n!
  static NumericalComplex HyperGeom_1_1(const NumericalScalar p1,
                                        const NumericalScalar q1,
                                        const NumericalComplex & x);
  // Hypergeometric function of type (2,1): hyperGeom_2_1(p1, p2, q1, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) . (p2 + k) / (q1 + k)] * x^n / n!
  static NumericalScalar HyperGeom_2_1(const NumericalScalar p1,
                                       const NumericalScalar p2,
                                       const NumericalScalar q1,
                                       const NumericalScalar x);
  // Hypergeometric function of type (2,2): hyperGeom_2_1(p1, p2, q1, q2, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) . (p2 + k) / (q1 + k) / (q2 + k)] * x^n / n!
  static NumericalScalar HyperGeom_2_2(const NumericalScalar p1,
                                       const NumericalScalar p2,
                                       const NumericalScalar q1,
                                       const NumericalScalar q2,
                                       const NumericalScalar x);
  // Erf function erf(x) = 2 / \sqrt(\pi) . \int_0^x \exp(-t^2) dt
  static NumericalScalar Erf(const NumericalScalar x);
  // Erf function erfc(x) = 1 - erf(x)
  static NumericalScalar ErfC(const NumericalScalar x);
  // Inverse of the erf function
  static NumericalScalar ErfInv(const NumericalScalar x);
  // Real branch of Lambert W function (principal or secndary)
  static NumericalScalar LambertW(const NumericalScalar x,
                                  const Bool principal = true);

  // MarcumQ- function
  //      static NumericalScalar MarcumQFunction(const NumericalScalar a,const NumericalScalar b);

  // Next power of two
  static UnsignedLong NextPowerOfTwo(const UnsignedLong n);

}; /* class SpecFunc */

END_NAMESPACE_OPENTURNS

#endif /* OPENTURNS_SPECFUNC_HXX */
