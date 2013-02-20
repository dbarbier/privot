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

# Register current directory files
ot_add_current_dir_to_include_dirs ()

ot_add_source_file ( ApproximationAlgorithmImplementation.cxx )
ot_add_source_file ( ApproximationAlgorithm.cxx )
ot_add_source_file ( ApproximationAlgorithmImplementationFactory.cxx )
ot_add_source_file ( ClassifierImplementation.cxx )
ot_add_source_file ( Classifier.cxx )
ot_add_source_file ( FittingAlgorithmImplementation.cxx )
ot_add_source_file ( FittingAlgorithm.cxx )
ot_add_source_file ( CorrectedLeaveOneOut.cxx )
ot_add_source_file ( KFold.cxx )
ot_add_source_file ( Lasso.cxx )
ot_add_source_file ( LassoFactory.cxx )
ot_add_source_file ( LeastSquaresMetaModelSelection.cxx )
ot_add_source_file ( LeastSquaresMetaModelSelectionFactory.cxx )
ot_add_source_file ( PenalizedLeastSquaresAlgorithm.cxx )
ot_add_source_file ( PenalizedLeastSquaresAlgorithmFactory.cxx )
ot_add_source_file ( FFTImplementation.cxx )
ot_add_source_file ( FFT.cxx )
ot_add_source_file ( KissFFT.cxx )

ot_install_header_file ( OTAlgo.hxx )
ot_install_header_file ( ApproximationAlgorithmImplementation.hxx )
ot_install_header_file ( ApproximationAlgorithm.hxx )
ot_install_header_file ( ApproximationAlgorithmImplementationFactory.hxx )
ot_install_header_file ( ClassifierImplementation.hxx )
ot_install_header_file ( Classifier.hxx )
ot_install_header_file ( FittingAlgorithmImplementation.hxx )
ot_install_header_file ( FittingAlgorithm.hxx )
ot_install_header_file ( CorrectedLeaveOneOut.hxx )
ot_install_header_file ( KFold.hxx )
ot_install_header_file ( Lasso.hxx )
ot_install_header_file ( LassoFactory.hxx )
ot_install_header_file ( LeastSquaresMetaModelSelection.hxx )
ot_install_header_file ( LeastSquaresMetaModelSelectionFactory.hxx )
ot_install_header_file ( PenalizedLeastSquaresAlgorithm.hxx )
ot_install_header_file ( PenalizedLeastSquaresAlgorithmFactory.hxx )
ot_install_header_file ( FFTImplementation.hxx )
ot_install_header_file ( FFT.hxx )
ot_install_header_file ( KissFFT.hxx )
ot_install_header_file ( kissfft.hh )
