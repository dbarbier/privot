// SWIG file dist_module.i
// @author schueller
// @date   2012-07-16 12:24:33 +0200 (Mon, 16 Jul 2012)

%module(package="openturns", docstring="Probabilistic distributions.") dist_bundle2
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
%include LogNormal.i
%include LogNormalFactory.i
%include LogUniform.i
%include LogUniformFactory.i
%include Mixture.i
%include Multinomial.i
%include MultinomialFactory.i
%include NegativeBinomial.i
%include NegativeBinomialFactory.i
%include NonCentralChiSquare.i
%include NonCentralStudent.i
%include Normal.i
%include NormalFactory.i
%include Poisson.i
%include PoissonFactory.i
%include RandomMixture.i
%include Rayleigh.i
%include RayleighFactory.i
%include Rice.i
%include RiceFactory.i
%include Student.i
%include StudentFactory.i
%include Trapezoidal.i
%include TrapezoidalFactory.i
%include Triangular.i
%include TriangularFactory.i
%include TruncatedDistribution.i
%include TruncatedNormal.i
%include TruncatedNormalFactory.i
%include Uniform.i
%include UniformFactory.i
%include UserDefinedPair.i
%include UserDefined.i
%include UserDefinedFactory.i
%include Weibull.i
%include WeibullFactory.i
%include ZipfMandelbrot.i
%include ConditionalDistribution.i
%include PosteriorDistribution.i

/* At last we include template definitions */
%include UncertaintyDistTemplateDefs.i
