//                                               -*- C++ -*-
/**
 *  @file  SpecFunc.cxx
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
 */
#include <cmath>
#include <limits>
#include <cstdlib>
#include <iostream>

#include "SpecFunc.hxx"
//#undef USE_DCDFLIB
#define USE_DCDFLIB
#ifdef USE_DCDFLIB
#include "Dcdflib.hxx"
#endif
#include "Debye.hxx"
#include "Exception.hxx"
#include "ResourceMap.hxx"

BEGIN_NAMESPACE_OPENTURNS

// 0.39894228040143267 = 1 / sqrt(2.pi)
const NumericalScalar SpecFunc::ISQRT2PI              = 0.39894228040143267;
// 0.57721566490153286 = Euler constant gamma
const NumericalScalar SpecFunc::EulerConstant         = 0.57721566490153286;
// 1.64493406684822643 = pi^2 / 6
const NumericalScalar SpecFunc::PI2_6                 = 1.64493406684822643;
// 1.28254983016118640 = pi / sqrt(6)
const NumericalScalar SpecFunc::PI_SQRT6              = 1.28254983016118640;
// 0.45005320754569466 = gamma * sqrt(6) / pi
const NumericalScalar SpecFunc::EULERSQRT6_PI         = 0.45005320754569466;
// 3.28986813369645287 = pi^2 / 3
const NumericalScalar SpecFunc::PI2_3                 = 3.28986813369645287;
// 0.55132889542179204 = sqrt(3) / pi
const NumericalScalar SpecFunc::SQRT3_PI              = 0.55132889542179204;
// 1.81379936423421785 = pi / sqrt(3)
const NumericalScalar SpecFunc::PI_SQRT3              = 1.81379936423421785;
// 1.81379936423421785 = pi / sqrt(3)
const NumericalScalar SpecFunc::ZETA3                 = 1.20205690315959429;
// NumericalScalar limits
const NumericalScalar SpecFunc::MinNumericalScalar    = std::numeric_limits<NumericalScalar>::min();
const NumericalScalar SpecFunc::LogMinNumericalScalar = log(MinNumericalScalar);
const NumericalScalar SpecFunc::MaxNumericalScalar    = std::numeric_limits<NumericalScalar>::max();
const NumericalScalar SpecFunc::LogMaxNumericalScalar = log(MaxNumericalScalar);
const NumericalScalar SpecFunc::NumericalScalarEpsilon = std::numeric_limits<NumericalScalar>::epsilon();

// Modified first kind Bessel function of order 0: BesselI0(x) = \sum_{m=0}\infty\frac{1}{m!^2}\left(\frac{x}{2}\right)^{2m}
NumericalScalar SpecFunc::SmallCaseBesselI0(const NumericalScalar x)
{
  const NumericalScalar x2(x * x);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  NumericalScalar value(1.0);
  NumericalScalar r(1.0);
  UnsignedLong k(1);
  while (fabs(r / value) >= precision)
    {
      r *= 0.25 * x2 / (k * k);
      value += r;
      ++k;
    }
  return value;
}

NumericalScalar SpecFunc::LargeCaseLogBesselI0(const NumericalScalar x)
{
  static NumericalScalar A[12] = {0.125, 7.03125e-02,
                                  7.32421875e-02, 1.1215209960938e-01,
                                  2.2710800170898e-01, 5.7250142097473e-01,
                                  1.7277275025845, 6.0740420012735,
                                  2.4380529699556e+01, 1.1001714026925e+02,
                                  5.5133589612202e+02, 3.0380905109224e+03
  };
  const NumericalScalar ax(fabs(x));
  UnsignedLong k0(12);
  if (ax >= 35.0) k0 = 9;
  if (ax >= 50.0) k0 = 7;
  NumericalScalar value(1.0);
  const NumericalScalar xR(1.0 / ax);
  NumericalScalar xRPow(xR);
  for (UnsignedLong k = 0; k < k0; ++k)
    {
      value += A[k] * xRPow;
      xRPow *= xR;
    }
  value = log(value) + ax - 0.5 * log(2.0 * M_PI * ax);
  return value;
}

NumericalScalar SpecFunc::BesselI0(const NumericalScalar x)
{
  if (x == 0.0) return 1.0;
  // Small argument
  if (fabs(x) <= 23.5) return SmallCaseBesselI0(x);
  // Large argument
  else return exp(LargeCaseLogBesselI0(x));
}

NumericalScalar SpecFunc::LogBesselI0(const NumericalScalar x)
{
  if (x == 0.0) return 0.0;
  // Small argument
  if (fabs(x) <= 23.5) return log(SmallCaseBesselI0(x));
  // Large argument
  else return LargeCaseLogBesselI0(x);
}

// Modified first kind Bessel function of order 1: BesselI1(x) = \sum_{m=0}\infty\frac{1}{m!(m+1)!}\left(\frac{x}{2}\right)^{2m+1}
NumericalScalar SpecFunc::SmallCaseBesselI1(const NumericalScalar x)
{
  const NumericalScalar x2(x * x);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  NumericalScalar value(1.0);
  NumericalScalar r(1.0);
  UnsignedLong k(1);
  while (fabs(r / value) >= precision)
    {
      r *= 0.25 * x2 / (k * (k + 1));
      value += r;
      ++k;
    }
  value *= 0.5 * x;
  return value;
}

NumericalScalar SpecFunc::LargeCaseLogBesselI1(const NumericalScalar x)
{
  static NumericalScalar B[12] = { -0.375, -1.171875e-01,
                                   -1.025390625e-01, -1.4419555664063e-01,
                                   -2.7757644653320e-01, -6.7659258842468e-01,
                                   -1.9935317337513, -6.8839142681099,
                                   -2.7248827311269e+01, -1.2159789187654e+02,
                                   -6.0384407670507e+02, -3.3022722944809e+03
  };
  const NumericalScalar ax(fabs(x));
  UnsignedLong k0(12);
  if (ax >= 35.0) k0 = 9;
  if (ax >= 50.0) k0 = 7;
  NumericalScalar value(1.0);
  const NumericalScalar xR(1.0 / ax);
  NumericalScalar xRPow(xR);
  for (UnsignedLong k = 0; k < k0; ++k)
    {
      value += B[k] * xRPow;
      xRPow *= xR;
    }
  value = log(value) + ax - 0.5 * log(2.0 * M_PI * ax);
  return value;
}

NumericalScalar SpecFunc::BesselI1(const NumericalScalar x)
{
  if (x == 0.0) return 0.0;
  // Small argument
  if (fabs(x) <= 22.0) return SmallCaseBesselI1(x);
  else
    {
      const NumericalScalar signX(x <= 0.0 ? -1.0 : 1.0);
      const NumericalScalar value(signX * exp(LargeCaseLogBesselI1(x)));
      return value;
    }
}

NumericalScalar SpecFunc::LogBesselI1(const NumericalScalar x)
{
  if (x <= 0.0) return -MaxNumericalScalar;
  // Small argument
  if (fabs(x) <= 22.0) return log(SmallCaseBesselI1(x));
  else return LargeCaseLogBesselI1(x);
}

// LnBeta function: LnBeta(a, b) = \log(Beta(a, b))
NumericalScalar SpecFunc::LnBeta(const NumericalScalar a,
                                 const NumericalScalar b)
{
  return lgamma(a) - lgamma(a + b) + lgamma(b);
}

// LogBeta = LnBeta
NumericalScalar SpecFunc::LogBeta(const NumericalScalar a,
                                  const NumericalScalar b)
{
  return LnBeta(a, b);
}

// Beta function: Beta(a, b) = \int_0^1 t^{a-1}(1-t)^{b-1} dt
NumericalScalar SpecFunc::Beta(const NumericalScalar a,
                               const NumericalScalar b)
{
  return exp(LnBeta(a, b));
}

// Incomplete Beta function: BetaInc(a, b, x) = \int_0^x t^{a-1}(1-t)^{b-1} dt
NumericalScalar SpecFunc::BetaInc(const NumericalScalar a,
                                  const NumericalScalar b,
                                  const NumericalScalar x)
{
  return BetaRatioInc(a, b, x) * Beta(a, b);
}

// Incomplete Beta function inverse
NumericalScalar SpecFunc::BetaIncInv(const NumericalScalar a,
                                     const NumericalScalar b,
                                     const NumericalScalar x)
{
  return BetaRatioIncInv(a, b, x / Beta(a, b));
}

// Incomplete Beta Ratio function: BetaRatioInc(a, b, x) = 1/beta(a, b) * \int_0^x t^{a-1}(1-t)^{b-1} dt
NumericalScalar SpecFunc::BetaRatioInc(const NumericalScalar a,
                                       const NumericalScalar b,
                                       const NumericalScalar x)
{
  if (x <= 0.0) return 0.0;
  if (x >= 1.0) return 1.0;
#ifdef USE_DCDFLIB
  NumericalScalar inf(a);
  NumericalScalar sup(b);
  NumericalScalar t(x);
  NumericalScalar y(1.0 - x);
  NumericalScalar w;
  NumericalScalar w1;
  int ierr;
  DCDFLIB::beta_inc(&inf, &sup, &t, &y, &w, &w1, &ierr);
  return w;
#else
  throw NotYetImplementedException(HERE);
#endif
}

// Incomplete Beta Ratio Function inverse
NumericalScalar SpecFunc::BetaRatioIncInv(const NumericalScalar a,
                                          const NumericalScalar b,
                                          const NumericalScalar x)
{
#ifdef USE_DCDFLIB
  int which(2);
  NumericalScalar inf(a);
  NumericalScalar sup(b);
  NumericalScalar p(x);
  NumericalScalar q(1.0 - x);
  NumericalScalar X;
  NumericalScalar Y;
  int status;
  NumericalScalar bound;
  DCDFLIB::cdfbet(&which, &p, &q, &X, &Y, &inf, &sup, &status, &bound);
  return X;
#else
  throw NotYetImplementedException(HERE);
#endif
}

// Debye function of order n: DebyeN(x, n) = n / x^n \int_0^x t^n/(\exp(t)-1) dt
NumericalScalar SpecFunc::Debye(const NumericalScalar x,
                                const UnsignedLong n)
{
  if ((n == 0) || (n > 20)) throw InvalidArgumentException(HERE) << "Error: cannot compute Debye function of order outside of {1,...,20}";
  if (x < 0.0) return 0.0;
  // The threshold is such that the overall error is less than 1.0e-16
  if (x < 1.0e-8) return 1.0 - n * x / (2.0 * (n + 1.0));
  return debyen(x, static_cast<int>(n)) * n / pow(x, n);
}

// Dawson function: Dawson(x) = \exp(-x^2) * \int_0^x \exp(t^2) dt
NumericalScalar SpecFunc::Dawson (NumericalScalar x)
{
  //----------------------------------------------------------------------
  // Coefficients for R(9,9) approximation for  |x| < 2.5
  //----------------------------------------------------------------------
  static NumericalScalar p1[10] =
    {
      -2.6902039878870478241e-12, 4.18572065374337710778e-10,
      -1.34848304455939419963e-8, 9.28264872583444852976e-7,
      -1.23877783329049120592e-5, 4.07205792429155826266e-4,
      -0.00284388121441008500446, 0.0470139022887204722217,
      -0.138868086253931995101,   1.00000000000000000004
    };

  static NumericalScalar q1[10] =
    {
      1.71257170854690554214e-10, 1.19266846372297253797e-8,
      4.32287827678631772231e-7,  1.03867633767414421898e-5,
      1.7891096528424624934e-4,   0.00226061077235076703171,
      0.0207422774641447644725,   0.132212955897210128811,
      0.527798580412734677256,    1.0
    };

  //----------------------------------------------------------------------
  // Coefficients for R(9,9) approximation in J-fraction form
  // for  x in [2.5, 3.5)
  //----------------------------------------------------------------------

  static NumericalScalar p2[10] =
    {
      -1.7095380470085549493,  -37.9258977271042880786,
      26.1935631268825992835,   12.5808703738951251885,
      -22.7571829525075891337,    4.56604250725163310122,
      -7.3308008989640287075,   46.5842087940015295573,
      -17.3717177843672791149,    0.500260183622027967838
    };

  static NumericalScalar q2[9] =
    {
      1.82180093313514478378, 1100.67081034515532891,
      -7.08465686676573000364,  453.642111102577727153,
      40.6209742218935689922,   302.890110610122663923,
      170.641269745236227356,    951.190923960381458747,
      0.206522691539642105009
    };

  //----------------------------------------------------------------------
  // Coefficients for R(9,9) approximation in J-fraction form
  // for  x in [3.5, 5.0]
  //----------------------------------------------------------------------

  static NumericalScalar p3[10] =
    {
      -4.55169503255094815112,  -18.6647123338493852582,
      -7.36315669126830526754,  -66.8407240337696756838,
      48.450726508149145213,     26.9790586735467649969,
      -33.5044149820592449072,     7.50964459838919612289,
      -1.48432341823343965307,    0.499999810924858824981
    };

  static NumericalScalar q3[9] =
    {
      44.7820908025971749852,    99.8607198039452081913,
      14.0238373126149385228,  3488.17758822286353588,
      -9.18871385293215873406, 1240.18500009917163023,
      -68.8024952504512254535,    -2.3125157538514514307,
      0.250041492369922381761
    };

  //----------------------------------------------------------------------
  // Coefficients for R(9,9) approximation in J-fraction form
  // for  |x| > 5.0
  //----------------------------------------------------------------------

  static NumericalScalar p4[10] =
    {
      -8.11753647558432685797,  -38.404388247745445343,
      -22.3787669028751886675,   -28.8301992467056105854,
      -5.99085540418222002197,  -11.3867365736066102577,
      -6.5282872752698074159,    -4.50002293000355585708,
      -2.50000000088955834952,    0.5000000000000004884
    };

  static NumericalScalar q4[9] =
    {
      269.382300417238816428,     50.4198958742465752861,
      61.1539671480115846173,   208.210246935564547889,
      19.7325365692316183531,   -12.2097010558934838708,
      -6.99732735041547247161,   -2.49999970104184464568,
      0.749999999999027092188
    };

  const NumericalScalar ax(fabs(x));
  // Underflow for large argument
  if (ax > 0.5 / MinNumericalScalar) return 0.0;
  // Simple asymptotic expression for large argument
  if (ax > 1.0 / sqrt(0.5 * MinNumericalScalar)) return 0.5 / x;
  // Simple serie for small argument
  if (ax < sqrt(0.5 * MinNumericalScalar)) return x;
  const NumericalScalar  y(x * x);
  // First range
  if (y < 6.25)
    {
      NumericalScalar sump(p1[0]);
      NumericalScalar sumq(q1[0]);
      for (UnsignedLong i = 1; i < 10; ++i)
        {
          sump = sump * y + p1[i];
          sumq = sumq * y + q1[i];
        }
      return x * sump / sumq;
    }
  // Second range
  if (y < 12.25)
    {
      NumericalScalar frac(0.0);
      for (UnsignedLong i = 0; i < 9; ++i) frac = q2[i] / (p2[i] + y + frac);
      return (p2[9] + frac) / x;
    }
  // Third range
  if (y <= 25.0)
    {
      NumericalScalar frac(0.0);
      for (UnsignedLong i = 0; i < 9; ++i) frac = q3[i] / (p3[i] + y + frac);
      return (p3[9] + frac) / x;
    }
  // Fourth and last range
  NumericalScalar frac(0.0);
  for (UnsignedLong i = 0; i < 9; ++i) frac = q4[i] / (p4[i] + y + frac);
  return (0.5 + 0.5 * (p4[9] + frac) / y) / x;
}

// Complex Faddeeva function: faddeeva(z) = \exp(-z^2)\erfc(-I*z)
NumericalComplex SpecFunc::Faddeeva(const NumericalComplex & z)
{
  static const NumericalScalar factor(1.12837916709551257388);
  const NumericalScalar xI(z.real());
  const NumericalScalar yI(z.imag());
  NumericalScalar u(0.0);
  NumericalScalar v(0.0);
  NumericalScalar u2(0.0);
  NumericalScalar v2(0.0);
  const NumericalScalar xAbs(fabs(xI));
  const NumericalScalar yAbs(fabs(yI));
  const NumericalScalar x(xAbs / 6.3);
  const NumericalScalar y(yAbs / 4.4);
  NumericalScalar qRho(x * x + y * y);
  NumericalScalar xQuad(xAbs * xAbs - yAbs * yAbs);
  NumericalScalar yQuad(2.0 * xAbs * yAbs);
  Bool a(qRho < 0.085264);
  // Power series for small magnitude argument
  if (a)
    {
      qRho = (1.0 - 0.85 * y) * sqrt(qRho);
      UnsignedLong n(static_cast<UnsignedLong>(round(6 + 72 * qRho)));
      UnsignedLong j(2 * n + 1);
      NumericalScalar xSum(1.0 / j);
      NumericalScalar ySum(0.0);
      for (UnsignedLong i = n; i > 0; --i)
        {
          j -= 2;
          NumericalScalar xAux((xSum * xQuad - ySum * yQuad) / i);
          ySum = (xSum * yQuad + ySum * xQuad) / i;
          xSum = xAux + 1.0 / j;
        }
      NumericalScalar u1(-factor * (xSum * yAbs + ySum * xAbs) + 1.0);
      NumericalScalar v1(factor * (xSum * xAbs - ySum * yAbs));
      NumericalScalar dAux(exp(-xQuad));
      u2 = dAux * cos(yQuad);
      v2 = -dAux * sin(yQuad);
      u = u1 * u2 - v1 * v2;
      v = u1 * v2 + v1 * u2;
    }
  // Laplace continued fraction for large argument, truncated Taylor expansion for intermediate argument
  else
    {
      NumericalScalar h(0.0);
      UnsignedLong kApn(0);
      UnsignedLong nu(0);
      NumericalScalar h2(0.0);
      if (qRho > 1.0)
        {
          qRho = sqrt(qRho);
          nu = static_cast<UnsignedLong>(round(3 + (1442 / (26 * qRho + 77))));
        }
      else
        {
          qRho = (1.0 - y) * sqrt(1.0 - qRho);
          h = 1.88 * qRho;
          h2 = 2.0 * h;
          kApn = static_cast<UnsignedLong>(round(7 + 34 * qRho));
          nu = static_cast<UnsignedLong>(round(16 + 26 * qRho));
        }
      Bool b(h > 0.0);
      NumericalScalar qLambda(0.0);
      if (b) qLambda = pow(h2, kApn);
      NumericalScalar rX(0.0);
      NumericalScalar rY(0.0);
      NumericalScalar sX(0.0);
      NumericalScalar sY(0.0);
      for (UnsignedLong n = nu + 1; n > 0; --n)
        {
          NumericalScalar tX(yAbs + h + n * rX);
          NumericalScalar tY(xAbs - n * rY);
          NumericalScalar c(0.5 / (tX * tX + tY * tY));
          rX = c * tX;
          rY = c * tY;
          if (b && (n <= kApn + 1))
            {
              tX = qLambda + sX;
              sX = rX * tX - rY * sY;
              sY = rY * tX + rX * sY;
              qLambda /= h2;
            }
        }
      if (h == 0.0)
        {
          u = factor * rX;
          v = factor * rY;
        }
      else
        {
          u = factor * sX;
          v = factor * sY;
        }
      if (yAbs == 0.0) u = exp(-xAbs * xAbs);
    }
  // Deal with the other quadrants
  if (yI < 0.0)
    {
      if (a)
        {
          u2 *= 2.0;
          v2 *= 2.0;
        }
      else
        {
          xQuad = -xQuad;
          NumericalScalar w1(2.0 * exp(xQuad));
          u2 = w1 * cos(yQuad);
          v2 = -w1 * sin(yQuad);
        }
      u = u2 - u;
      v = v2 - v;
      if (xI > 0.0) v = -v;
    }
  else
    {
      if (xI < 0.0) v = -v;
    }
  return NumericalComplex(u, v);
}

// Gamma function: Gamma(a) = \int_0^{\infty} t^{a-1}\exp(-t) dt
NumericalScalar SpecFunc::Gamma(const NumericalScalar a)
{
  return tgamma(a);
}

// Complex gamma function: Gamma(a) = \int_0^{\infty} t^{a-1}\exp(-t) dt,
// Computed using Lanczos approximation, using a C++ translation of
// Paul Godfrey's matlab implementation
// http://home.att.net/~numericana/answer/info/godfrey.htm#matlabgamma
NumericalComplex SpecFunc::Gamma(const NumericalComplex & a)
{
  if (a.imag() == 0.0) return Gamma(a.real());
  const NumericalScalar sqrt2Pi(sqrt(2.0 * M_PI));
  NumericalComplex z(a);
  Bool flip(false);
  if (z.real() < 0.0)
    {
      z = -z;
      flip = true;
    }
  const UnsignedLong coefficientsSize(11);
  static const NumericalScalar coefficients[coefficientsSize] =
    {
      1.000000000000000174663,      5716.400188274341379136,
      -14815.30426768413909044,     14291.49277657478554025,
      -6348.160217641458813289,     1301.608286058321874105,
      -108.1767053514369634679,     2.605696505611755827729,
      -0.7423452510201416151527e-2, 0.5384136432509564062961e-7,
      -0.4023533141268236372067e-8
    };
  const NumericalScalar g(coefficientsSize - 2.0);
  NumericalComplex t(z + g);
  NumericalComplex s(0.0);
  NumericalComplex ss(t - 0.5);
  for (UnsignedLong k = coefficientsSize - 1; k > 0; --k)
    {
      s += coefficients[k] / t;
      t -= 1.0;
    }
  s += coefficients[0];
  s = log(s * sqrt2Pi) + (z - 0.5) * log(ss) - ss;
  NumericalComplex f(exp(s));
  if (flip) f = -M_PI / (a * f * sin(M_PI * a));
  return f;
}

// Natural logarithm of the Gamma function
NumericalScalar SpecFunc::LnGamma(const NumericalScalar a)
{
  return lgamma(a);
}

// LogGamma = LnGamma
NumericalScalar SpecFunc::LogGamma(const NumericalScalar a)
{
  return LnGamma(a);
}

// Incomplete Gamma function: GammaInc(a, x) = \int_0^x t^{a-1}\exp(-t) dt
NumericalScalar SpecFunc::GammaInc(const NumericalScalar a,
                                   const NumericalScalar x)
{
#ifdef USE_DCDFLIB
  NumericalScalar k(a);
  NumericalScalar t(x);
  NumericalScalar ans;
  NumericalScalar qans;
  int ind(0);
  DCDFLIB::gamma_inc(&k, &t, &ans, &qans, &ind);
  return ans * SpecFunc::Gamma(a);
#else
  NumericalScalar factor(pow(x, a) * exp(-x));
  NumericalScalar denominator(a);
  NumericalScalar term(x / denominator);
  NumericalScalar sum(term);
  NumericalScalar eps(0.0);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      ++denominator;
      term *= x / denominator;
      sum += term;
      eps = fabs(term / sum);
    }
  while (eps > precision);
  return factor * sum;
#endif
}

// Incomplete Gamma function inverse with respect to x
NumericalScalar SpecFunc::GammaIncInv(const NumericalScalar a,
                                      const NumericalScalar x)
{
#ifdef USE_DCDFLIB
  NumericalScalar k(a);
  NumericalScalar X;
  NumericalScalar X0(0.0);
  NumericalScalar p(x);
  NumericalScalar q(1.0 - x);
  int ierr;
  DCDFLIB::gamma_inc_inv(&k, &X, &X0, &p, &q, &ierr);
  return X;
#else
  throw NotYetImplementedException(HERE);
#endif
}

// Digamma function: Psi(x) = ((dgamma/dx) / gamma)(x)
// Derivative of a Lanczos approximation of log(Gamma)
NumericalScalar SpecFunc::DiGamma(const NumericalScalar x)
{
  // Check that the argument is not non positive, i.e. not in {0, -1, -2, ...}
  if ((x <= 0.0) && (x == round(x))) throw InvalidArgumentException(HERE) << "Error: the argument of the DiGamma function cannot be a non positive integer.";
  // Approximation for small arguments
  // Here, 0.025 is a bound that insure NumericalScalar precision approximation
  if ( fabs(x) <= 0.025 ) return -1.0 / x - .57721566490153286 + (1.6449340668482264 + (-1.2020569031595943 + (1.0823232337111381 + (-1.0369277551433699 + (1.0173430619844491 + (-1.0083492773819228 + (1.0040773561979442 + (-1.0020083928260822 + 1.0009945751278180 * x) * x) * x) * x) * x) * x) * x) * x) * x;
  // If the argument is negative, use the reflexion formula
  if (x < 0.0) return -M_PI / tan(M_PI * x) + DiGamma(1.0 - x);
  // Shift the argument until it reaches the asymptotic expansion region
  // Here, 7.69 is a bound that insure NumericalScalar precision of the approximation
  NumericalScalar z(x);
  NumericalScalar value(0.0);
  while ( z < 7.33 )
    {
      value -= 1.0 / z;
      z += 1.0;
    }
  // Use the asymptotic expansion in Horner form
  const NumericalScalar y(1.0 / (z * z));
  return value + log(z) - 0.5 / z + (-0.83333333333333333e-1 + (0.83333333333333333e-2 + (-0.39682539682539683e-2 + (0.41666666666666667e-2 + (-0.75757575757575758e-2 + (0.21092796092796093e-1 + (-0.83333333333333333e-1 + (.44325980392156863 - 3.0539543302701197 * y) * y) * y) * y) * y) * y) * y) * y) * y;
}

NumericalScalar SpecFunc::Psi(const NumericalScalar x)
{
  return DiGamma(x);
}

// Inverse of the DiGamma function
NumericalScalar SpecFunc::DiGammaInv(const NumericalScalar a)
{
  // Initialization using an asymptotic approximation of the DiGamma function
  NumericalScalar x( a < -2.22 ? -1.0 / (a - EulerConstant) : exp(a) + 0.5);
  // Use a Newton scheme with six iterations
  NumericalScalar d(0.0);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      d = (DiGamma(x) - a) / TriGamma(x);
      x -= d;
    }
  while (fabs(d) > precision);
  return x;
}

// Trigamma function: TriGamma(x) = ((d^2gamma/dx^2) / gamma)(x)
NumericalScalar SpecFunc::TriGamma(const NumericalScalar x)
{
  // Check that the argument is not non positive, i.e. not in {0, -1, -2, ...}
  if ((x <= 0.0) && (x == round(x))) throw InvalidArgumentException(HERE) << "Error: the argument of the TriGamma function cannot be a non positive integer.";
  // Approximation for small arguments
  // Here, 0.02 is a bound that insure NumericalScalar precision approximation
  if ( fabs(x) <= 0.02 ) return 1.0 / (x * x) + 1.6449340668482264 + (-2.4041138063191886 + (3.2469697011334144 + (-4.1477110205734796 + (5.0867153099222453 + (-6.0500956642915368 + (7.0285414933856097 + (-8.0160671426086576 + (9.0089511761503616 - 10.004941886041195 * x) * x) * x) * x) * x) * x) * x) * x) * x;
  // If the argument is negative, use the reflexion formula
  if (x < 0.0) return pow(M_PI / sin(M_PI * x), 2.0) - TriGamma(1.0 - x);
  // Shift the argument until it reaches the asymptotic expansion region
  // Here, 7.69 is a bound that insure NumericalScalar precision of the approximation
  NumericalScalar z(x);
  NumericalScalar value(0.0);
  while ( z < 7.69 )
    {
      value += 1.0 / (z * z);
      z += 1.0;
    }
  // Use the asymptotic expansion in Horner form
  const NumericalScalar y(1.0 / (z * z));
  return value + 0.5 * y + (1. + (.16666666666666667 + (-0.33333333333333333e-1 + (0.23809523809523810e-1 + (-0.33333333333333333e-1 + (0.75757575757575758e-1 + (-.25311355311355311 + (1.1666666666666667 + (-7.0921568627450980 + 54.971177944862155 * y) * y) * y) * y) * y) * y) * y) * y) * y) / z;
}

// Hypergeometric function of type (1,1): HyperGeom_1_1(p1, q1, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) / (q1 + k)] * x^n / n!
NumericalScalar SpecFunc::HyperGeom_1_1(const NumericalScalar p1,
                                        const NumericalScalar q1,
                                        const NumericalScalar x)
{
  if (q1 == p1) return exp(x);
  if (x == 0) return 1.0;
  NumericalScalar term(1.0);
  NumericalScalar t(x);
  NumericalScalar pochhammerP1(p1);
  if (x < 0)
    {
      pochhammerP1 = q1 - p1;
      t = -x;
      term = exp(x);
    }
  NumericalScalar pochhammerQ1(q1);
  NumericalScalar factorial(1.0);
  NumericalScalar sum(term);
  NumericalScalar eps;
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      term *= pochhammerP1 * t / (pochhammerQ1 * factorial);
      ++pochhammerP1;
      ++pochhammerQ1;
      ++factorial;
      sum += term;
      eps = fabs(term / sum);
    }
  while (eps > precision);
  return sum;
}

// Complex hypergeometric function of type (1,1): HyperGeom_1_1(p1, q1, x) = \sum_{n=0}^{\infty} [\prod_{k=0}^{n-1} (p1 + k) / (q1 + k)] * x^n / n!
NumericalComplex SpecFunc::HyperGeom_1_1(const NumericalScalar p1,
                                         const NumericalScalar q1,
                                         const NumericalComplex & x)
{
  NumericalComplex pochhammerP1(p1);
  NumericalComplex pochhammerQ1(q1);
  NumericalScalar factorial(1.0);
  NumericalComplex term(1.0);
  NumericalComplex sum(term);
  NumericalComplex eps(0.0);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      term *= pochhammerP1 * x / (pochhammerQ1 * factorial);
      pochhammerP1 += 1.0;
      pochhammerQ1 += 1.0;
      ++factorial;
      sum += term;
      eps = term / sum;
    }
  while (abs(eps) > precision);
  return sum;
}

// Hypergeometric function of type (2,1): HyperGeom_2_1(p1, p2, q1, x) = sum_{n=0}^{\infty} [prod_{k=0}^{n-1} (p1 + k) . (p2 + k) / (q1 + k)] * x^n / n!
NumericalScalar SpecFunc::HyperGeom_2_1(const NumericalScalar p1,
                                        const NumericalScalar p2,
                                        const NumericalScalar q1,
                                        const NumericalScalar x)
{
  NumericalScalar pochhammerP1(p1);
  NumericalScalar pochhammerP2(p2);
  NumericalScalar pochhammerQ1(q1);
  NumericalScalar factorial(1.0);
  NumericalScalar term(1.0);
  NumericalScalar sum(term);
  NumericalScalar eps(0.0);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      term *= pochhammerP1 * pochhammerP2 * x / (pochhammerQ1 * factorial);
      ++pochhammerP1;
      ++pochhammerP2;
      ++pochhammerQ1;
      ++factorial;
      sum += term;
      eps = fabs(term / sum);
    }
  while (eps > precision);
  return sum;
}

// Hypergeometric function of type (2,2): HyperGeom_2_1(p1, p2, q1, q2, x) = sum_{n=0}^{\infty} [prod_{k=0}^{n-1} (p1 + k) . (p2 + k) / (q1 + k) / (q2 + k)] * x^n / n!
NumericalScalar SpecFunc::HyperGeom_2_2(const NumericalScalar p1,
                                        const NumericalScalar p2,
                                        const NumericalScalar q1,
                                        const NumericalScalar q2,
                                        const NumericalScalar x)
{
  if (x == 0.0) return 1.0;
  NumericalScalar pochhammerP1(p1);
  NumericalScalar pochhammerP2(p2);
  NumericalScalar pochhammerQ1(q1);
  NumericalScalar pochhammerQ2(q2);
  NumericalScalar factorial(1.0);
  NumericalScalar term(0.0);
  NumericalScalar sum(term);
  NumericalScalar eps(0.0);
  const NumericalScalar logX(log(fabs(x)));
  NumericalScalar signX(x > 0.0 ? 1.0 : -1.0);
  NumericalScalar signTerm(1.0);
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  do
    {
      term += log(pochhammerP1) + log(pochhammerP2) + logX - log(pochhammerQ1) - log(pochhammerQ2) - log(factorial);
      ++pochhammerP1;
      ++pochhammerP2;
      ++pochhammerQ1;
      ++pochhammerQ2;
      ++factorial;
      sum += signTerm * exp(term);
      signTerm *= signX;
      eps = fabs(term / sum);
    }
  while (eps > precision);
  return sum;
}

// Erf function Erf(x) = 2 / sqrt(Pi) . \int_0^x \exp(-t^2) dt
NumericalScalar SpecFunc::Erf(const NumericalScalar x)
{
  return erf(x);
}

// Erfc function ErfC(x) = 1 - Erf(x)
NumericalScalar SpecFunc::ErfC(const NumericalScalar x)
{
  return erfc(x);
}

// Inverse of the Erf function
NumericalScalar SpecFunc::ErfInv(const NumericalScalar x)
{
  //#ifdef USE_DCDFLIB
  //      NumericalScalar p(0.5 * (x + 1.0));
  //      NumericalScalar q(1.0 - p);
  //      return M_SQRT1_2 * DCDFLIB::dinvnr((NumericalScalar*)&p, (NumericalScalar*)&q);
  //#else
  NumericalScalar p(0.5 * (x + 1.0));
  static const NumericalScalar a[6] =
    {
      -3.969683028665376e+01,  2.209460984245205e+02,
      -2.759285104469687e+02,  1.383577518672690e+02,
      -3.066479806614716e+01,  2.506628277459239e+00
    };
  static const NumericalScalar b[5] =
    {
      -5.447609879822406e+01,  1.615858368580409e+02,
      -1.556989798598866e+02,  6.680131188771972e+01,
      -1.328068155288572e+01
    };
  static const NumericalScalar c[6] =
    {
      -7.784894002430293e-03, -3.223964580411365e-01,
      -2.400758277161838e+00, -2.549732539343734e+00,
      4.374664141464968e+00,  2.938163982698783e+00
    };
  static const NumericalScalar d[4] =
    {
      7.784695709041462e-03,  3.224671290700398e-01,
      2.445134137142996e+00,  3.754408661907416e+00
    };
  NumericalScalar q;
  NumericalScalar t;
  NumericalScalar u;
  q = std::min(p, 1.0 - p);
  if (q > 0.02425)
    {
      /* Rational approximation for central region. */
      u = q - 0.5;
      t = u * u;
      u = u * (((((a[0] * t + a[1]) * t + a[2]) * t + a[3]) * t + a[4]) * t + a[5])
        / (((((b[0] * t + b[1]) * t + b[2]) * t + b[3]) * t + b[4]) * t + 1.0);
    }
  else
    {
      /* Rational approximation for tail region. */
      t = sqrt(-2.0 * log(q));
      u = (((((c[0] * t + c[1]) * t + c[2]) * t + c[3]) * t + c[4]) * t + c[5])
        / ((((d[0] * t + d[1]) * t + d[2]) * t + d[3]) * t + 1.0);
    }
  /* The relative error of the approximation has absolute value less
     than 1.15e-9.  One iteration of Halley's rational method (third
     order) gives full machine precision... */
  t = 0.5 + 0.5 * Erf(u * M_SQRT1_2) - q;    /* f(u) = error */
  // 2.50662827463100050241576528481 = sqrt(2.pi)
  t = t * 2.50662827463100050241576528481 * exp(0.5 * u * u);   /* f(u)/df(u) */
  u = u - t / (1.0 + 0.5 * u * t);     /* Halley's method */
  return (p > 0.5 ? -M_SQRT1_2 * u : M_SQRT1_2 * u);
  //#endif
}

/* Evaluation of the principal barnch of Lambert W function.
   Based on formulas exposed in:
   Robert M. Corless, G. H. Gonnet, D. E. G. Hare, D. J. Jeffrey, and D. E. Knuth, "On the Lambert W Function", Advances in Computational Mathematics, volume 5, 1996, pp. 329--359
*/
NumericalScalar SpecFunc::LambertW(const NumericalScalar x,
                                   const Bool principal)
{
  NumericalScalar w(-1.0);
  // -0.36787944117144232159552377016146086 = -1 / exp(1)
  if (x <= -0.3678794411714423215955238) return w;
  // Principal branch, defined over [-1/e,+inf], LambertW >= -1
  if (principal)
    {
      if (x == 0) return 0.0;
      if(x < 6.46)
        {
          w = x * (3.0 + 4.0 * x) / (3.0 + x * (7.0 + 2.5 * x));
        }
      // Large argument, use asymptotic expansion, formula 4.18
      else
        {
          const NumericalScalar t1(log(x));
          w = t1 - log(t1);
        }
    }
  // Second real branch, defined over [-1/e, 0[, LambertW <= -1
  else
    {
      if (x >= 0.0) return -INFINITY;
      if (x < -0.1)
        {
          w = -2.0;
        }
      else
        {
          const NumericalScalar t1(log(-x));
          w = t1 - log(-t1);
        }
    }
  // Halley's iteration
  const NumericalScalar precision(ResourceMap::GetAsNumericalScalar( "SpecFunc-Precision" ));
  for (UnsignedLong i = 0; i < 3; ++i)
    {
      const NumericalScalar expW(exp(w));
      const NumericalScalar numerator(w * expW - x);
      const NumericalScalar dw(numerator / (expW * (w + 1.0) - 0.5 * (w + 2.0) * numerator / (w + 1.0)));
      w -= dw;
      if (fabs(dw) < precision) break;
    }
  return w;
}

UnsignedLong SpecFunc::NextPowerOfTwo(const UnsignedLong n)
{
  UnsignedLong powerOfTwo(1);
  while (powerOfTwo < n) powerOfTwo <<= 1;
  return powerOfTwo;
}

END_NAMESPACE_OPENTURNS
