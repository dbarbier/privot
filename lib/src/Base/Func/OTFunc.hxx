//                                               -*- C++ -*-
/**
 *  @file  OTFunc.hxx
 *  @brief The external header file of Open TURNS for subdir Func
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
 *  Id      OTFunc.hxx 1129 2009-02-11 15:49:59Z dutka
 */

#ifndef OPENTURNS_OTFUNC_HXX
#define OPENTURNS_OTFUNC_HXX

#include "AggregatedNumericalMathEvaluationImplementation.hxx"
#include "AnalyticalNumericalMathEvaluationImplementation.hxx"
#include "AnalyticalNumericalMathGradientImplementation.hxx"
#include "AnalyticalNumericalMathHessianImplementation.hxx"
#include "Basis.hxx"
#include "BasisSequence.hxx"
#include "BasisSequenceImplementation.hxx"
#include "BasisSequenceFactory.hxx"
#include "BasisSequenceFactoryImplementation.hxx"
#include "ComposedNumericalMathEvaluationImplementation.hxx"
#include "ComposedNumericalMathFunction.hxx"
#include "ComposedNumericalMathGradientImplementation.hxx"
#include "ComposedNumericalMathHessianImplementation.hxx"
#include "ComputedNumericalMathEvaluationImplementation.hxx"
#include "ComputedNumericalMathGradientImplementation.hxx"
#include "ComputedNumericalMathHessianImplementation.hxx"
#include "ConstantNumericalMathGradientImplementation.hxx"
#include "ConstantNumericalMathHessianImplementation.hxx"
#include "DatabaseNumericalMathEvaluationImplementation.hxx"
#include "DualLinearCombinationEvaluationImplementation.hxx"
#include "DualLinearCombinationGradientImplementation.hxx"
#include "DualLinearCombinationHessianImplementation.hxx"
#include "DynamicalFunction.hxx"
#include "DynamicalFunctionImplementation.hxx"
#include "ExpertMixture.hxx"
#include "IndicatorNumericalMathEvaluationImplementation.hxx"
#include "LAR.hxx"
#include "LARLasso.hxx"
#include "LARFSZero.hxx"
#include "Library.hxx"
#include "LibraryLoader.hxx"
#include "LinearCombinationEvaluationImplementation.hxx"
#include "LinearCombinationGradientImplementation.hxx"
#include "LinearCombinationHessianImplementation.hxx"
#include "LinearNumericalMathEvaluationImplementation.hxx"
#include "LinearNumericalMathFunction.hxx"
#include "LinearNumericalMathGradientImplementation.hxx"
#include "MethodBoundNumericalMathEvaluationImplementation.hxx"
#include "NoNumericalMathEvaluationImplementation.hxx"
#include "NoNumericalMathGradientImplementation.hxx"
#include "NoNumericalMathHessianImplementation.hxx"
#include "NumericalMathEvaluationImplementation.hxx"
#include "NumericalMathFunction.hxx"
#include "NumericalMathFunctionImplementation.hxx"
#include "NumericalMathGradientImplementation.hxx"
#include "NumericalMathHessianImplementation.hxx"
#include "PiecewiseEvaluationImplementation.hxx"
#include "ProductNumericalMathEvaluationImplementation.hxx"
#include "ProductNumericalMathFunction.hxx"
#include "ProductNumericalMathGradientImplementation.hxx"
#include "ProductNumericalMathHessianImplementation.hxx"
#include "ProductPolynomialEvaluationImplementation.hxx"
#include "ProductPolynomialGradientImplementation.hxx"
#include "ProductPolynomialHessianImplementation.hxx"
#include "QuadraticNumericalMathEvaluationImplementation.hxx"
#include "RestrictedEvaluationImplementation.hxx"
#include "RestrictedGradientImplementation.hxx"
#include "RestrictedHessianImplementation.hxx"
#include "SpatialFunction.hxx"
#include "TemporalFunction.hxx"
#include "UniVariatePolynomial.hxx"
#include "UniVariatePolynomialImplementation.hxx"
#include "WrapperData.hxx"
#include "WrapperFile.hxx"
#include "WrapperFile_version1.hxx"
#include "WrapperFile_version2.hxx"
#include "WrapperObject.hxx"
#include "XMLTags.hxx"
#include "OTSpecFunc.hxx"
#include "BoxCoxEvaluationImplementation.hxx"
#include "BoxCoxTransform.hxx"
#include "InverseBoxCoxEvaluationImplementation.hxx"
#include "InverseBoxCoxTransform.hxx"
#include "TrendEvaluationImplementation.hxx"
#include "TrendTransform.hxx"
#include "InverseTrendEvaluationImplementation.hxx"
#include "InverseTrendTransform.hxx"

#endif /* OPENTURNS_OTFUNC_HXX */
