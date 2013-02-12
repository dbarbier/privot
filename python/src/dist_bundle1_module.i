// SWIG file dist_module.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%module(package="openturns", docstring="Probabilistic distributions.") dist_bundle1
#pragma SWIG nowarn=302,509
%feature("autodoc","1");

%{
#include "OTconfig.hxx"
#include "OTBase.hxx"
#include "OTModel.hxx"
#include "OTDistribution.hxx"
%}

%include typemaps.i
%include OTtypes.i
%include OTexceptions.i
%include std_vector.i

%import common_module.i
%import typ_module.i
%include TestResult.i
%import base_module.i
%import model_copula_module.i

/* Uncertainty/Distribution (except copulas) */
%include DistFunc.i
%include Arcsine.i
%include ArcsineFactory.i
%include Bernoulli.i
%include BernoulliFactory.i
%include Beta.i
%include BetaFactory.i
%include Binomial.i
%include BinomialFactory.i
%include Burr.i
%include BurrFactory.i
%include Chi.i
%include ChiFactory.i
%include ChiSquare.i
%include ChiSquareFactory.i
%include Dirac.i
%include DiracFactory.i
%include Dirichlet.i
%include DirichletFactory.i
%include Epanechnikov.i
%include Exponential.i
%include ExponentialFactory.i
%include FisherSnedecor.i
%include FisherSnedecorFactory.i
%include Gamma.i
%include GammaFactory.i
%include Geometric.i
%include GeometricFactory.i
%include Gumbel.i
%include GumbelFactory.i
%include HistogramPair.i
%include Histogram.i
%include HistogramFactory.i
%include InverseNormal.i
%include InverseNormalFactory.i
%include KPermutationsDistribution.i
%include KernelMixture.i
%include KernelSmoothing.i
%include Laplace.i
%include LaplaceFactory.i
%include Logistic.i
%include LogisticFactory.i

/* At last we include template definitions */
%include UncertaintyDistTemplateDefs.i
