#                                               -*- cmake -*-
#
#  CMakeLists.txt
#
#  Copyright (C) 2005-2013 EDF-EADS-Phimeca
#
#  This library is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Lesser General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public
#  along with this library.  If not, see <http://www.gnu.org/licenses/>.
#
#  @author dutka
#  @date   2010-02-04 16:44:49 +0100 (Thu, 04 Feb 2010)
#

install ( FILES
          ValidSolveLinearSystemRect.sce
          AbdoRackwitz.txt
          Cas1_eads.txt
          Cas1.txt
          Cas2.txt
          Cas3_eads.txt
          Cas4.txt
          Cas5.txt
          Cas6.txt
          Cas7_eads.txt
          CoefficientOfVariation.txt
          Curvature.txt
          Debye.txt
          DefaultDistribution.txt
          DirectionalSampling.txt
          dProb.txt
          equality_constrained_quadratic_quadratic.txt
          GeneralizedNataf.txt
          IntegrationND.txt
          InverseErf.txt
          lambda_U.txt
          LambertW.txt
          NonCentralStudent.txt
          PSORM.txt
          qnorm.txt
          Sensibilite_ref1.txt
          Sensibilite_ref2.txt
          Sensibilite_ref3.txt
          Sensibilite_ref4.txt
          SensibiliteValidBeta.txt
          SinhTanh.txt
          Student2D.txt
          ti.txt
          truncnorm.txt
          ValidAbdoMixture.txt
          ValidAndersonDarling.txt
          ValidBeta.txt
          ValidBoundConstrainedAlgoithm.txt
          ValidChiSquare.txt
          ValidClaytonCopula.txt
          ValidConditionalArchimedean.txt
          ValidCovarianceAlgorithm.txt
          ValidCovarianceGauss.txt
          ValidCovarianceMatrixLapack.sce
          ValidCovariance.txt
          ValidCramerVonMises.txt
          ValidDiff.txt
          ValidDistFunc.txt
          ValidDistribution.txt
          ValidElliptical.txt
          validElliptical.sce
          validEnumerate.txt
          ValidEpanechnikov.txt
          ValidEstimatedTruncatedNormal.txt
          ValidEstimateLogistic.txt
          ValidEstimate.txt
          ValidExponential.txt
          ValidFORMKernelMixture.txt
          ValidFrankCopula.txt
          ValidGammaEstimate.txt
          ValidGamma.txt
          ValidGeometric.txt
          ValidGumbelCopula.txt
          ValidGumbelEstimate.txt
          ValidGumbel.txt
          ValidHisto.txt
          ValidIndependentCopula.txt
          ValidInverseIsoprobabilisticTransformationEllipticalCopula.txt
          ValidInverseNatafCopula2.txt
          ValidInverseNatafCopula.txt
          ValidIsoProbabilisicTransformationEllipticalCopula.txt
          ValidLeastSquareAlgorithm.txt
          ValidLinearCombination.txt
          ValidLaplace.txt
          ValidLogistic.txt
          ValidLogNormal.txt
          ValidMultiNomial.txt
          ValidMultiStudent.txt
          ValidNatafEllipticalcopula.txt
          ValidNormal1.txt
          ValidNormalCopula.txt
          ValidNumericalSample.R
          ValidOrthogonalProductFactory.txt
          ValidOrthogonalUniVariateFactory.txt
          ValidPoisson.txt
          ValidPoutreArticle.txt
          ValidProductFunction.txt
          ValidSampleComputation.sce
          ValidQuadraticFunction.txt
          ValidQuadraticTaylor.txt
          ValidRayleigh.txt
          valid_SORM.txt
          ValidSORM.txt
          ValidSpecFunc.txt
          ValidStudentEstimate.txt
          ValidStudent.txt
          ValidTriangular.txt
          ValidTruncatedNormalFactory.txt
          ValidTruncatedNormal.txt
          ValidUniform.txt
          ValidUniVariatePolynomial.txt
          ValidWeibull.txt
          ValidWilks.txt
          Variance.txt
          Weibull_moment.txt
          ziggurat_exponential.txt
          ziggurat.txt
          ziggurat_normal.txt
          ValidZipfMandelbrot.mw
          DESTINATION ${OPENTURNS_VALIDATION_PATH}
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
          