//                                               -*- C++ -*-
/**
 *  @file  OTDistribution.hxx
 *  @brief The external header file of Open TURNS for subdir Distribution
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
 *  @date   2009-02-11 16:49:59 +0100 (Wed, 11 Feb 2009)
 *  Id      OTDistribution.hxx 1129 2009-02-11 15:49:59Z dutka
 */

#ifndef OPENTURNS_OTDISTRIBUTION_HXX
#define OPENTURNS_OTDISTRIBUTION_HXX

#include "AliMikhailHaqCopula.hxx"
#include "AliMikhailHaqCopulaFactory.hxx"
#include "Arcsine.hxx"
#include "ArcsineFactory.hxx"
#include "Bernoulli.hxx"
#include "BernoulliFactory.hxx"
#include "Beta.hxx"
#include "BetaFactory.hxx"
#include "Binomial.hxx"
#include "BinomialFactory.hxx"
#include "Burr.hxx"
#include "BurrFactory.hxx"
#include "ChiSquare.hxx"
#include "ChiSquareFactory.hxx"
#include "Chi.hxx"
#include "ChiFactory.hxx"
#include "ClaytonCopula.hxx"
#include "ClaytonCopulaFactory.hxx"
#include "ComposedCopula.hxx"
#include "ComposedDistribution.hxx"
#include "ConditionalDistribution.hxx"
#include "Dirac.hxx"
#include "DiracFactory.hxx"
#include "Dirichlet.hxx"
#include "DirichletFactory.hxx"
#include "DistFunc.hxx"
#include "Epanechnikov.hxx"
#include "Exponential.hxx"
#include "ExponentialFactory.hxx"
#include "FarlieGumbelMorgensternCopula.hxx"
#include "FarlieGumbelMorgensternCopulaFactory.hxx"
#include "FrankCopula.hxx"
#include "FrankCopulaFactory.hxx"
#include "FisherSnedecor.hxx"
#include "FisherSnedecorFactory.hxx"
#include "Gamma.hxx"
#include "GammaFactory.hxx"
#include "Geometric.hxx"
#include "GeometricFactory.hxx"
#include "Gumbel.hxx"
#include "GumbelCopula.hxx"
#include "GumbelCopulaFactory.hxx"
#include "GumbelFactory.hxx"
#include "Histogram.hxx"
#include "HistogramFactory.hxx"
#include "IndependentCopula.hxx"
#include "InverseNormal.hxx"
#include "InverseNormalFactory.hxx"
#include "KernelMixture.hxx"
#include "KernelSmoothing.hxx"
#include "Laplace.hxx"
#include "LaplaceFactory.hxx"
#include "Logistic.hxx"
#include "LogisticFactory.hxx"
#include "LogNormal.hxx"
#include "LogNormalFactory.hxx"
#include "LogUniform.hxx"
#include "LogUniformFactory.hxx"
#include "MinCopula.hxx"
#include "Mixture.hxx"
#include "Multinomial.hxx"
#include "MultinomialFactory.hxx"
#include "NegativeBinomial.hxx"
#include "NegativeBinomialFactory.hxx"
#include "NonCentralChiSquare.hxx"
#include "NonCentralStudent.hxx"
#include "Normal2DCDF.hxx"
#include "Normal3DCDF.hxx"
#include "Normal.hxx"
#include "NormalCopula.hxx"
#include "NormalCopulaFactory.hxx"
#include "NormalFactory.hxx"
#include "Poisson.hxx"
#include "PoissonFactory.hxx"
#include "PosteriorDistribution.hxx"
#include "RandomMixture.hxx"
#include "Rayleigh.hxx"
#include "RayleighFactory.hxx"
#include "Rice.hxx"
#include "RiceFactory.hxx"
#include "Student.hxx"
#include "StudentFactory.hxx"
#include "TracyWidomGOE.hxx"
#include "TracyWidomGSE.hxx"
#include "TracyWidomGUE.hxx"
#include "Trapezoidal.hxx"
#include "TrapezoidalFactory.hxx"
#include "Triangular.hxx"
#include "TriangularFactory.hxx"
#include "TruncatedDistribution.hxx"
#include "TruncatedNormal.hxx"
#include "TruncatedNormalFactory.hxx"
#include "Uniform.hxx"
#include "UniformFactory.hxx"
#include "UserDefined.hxx"
#include "UserDefinedPair.hxx"
#include "UserDefinedFactory.hxx"
#include "Weibull.hxx"
#include "WeibullFactory.hxx"
#include "ZipfMandelbrot.hxx"

#endif /* OPENTURNS_OTDISTRIBUTION_HXX */
