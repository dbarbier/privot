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

ot_add_source_file ( EnumerateFunction.cxx )
ot_add_source_file ( EnumerateFunctionImplementation.cxx )
ot_add_source_file ( LinearEnumerateFunction.cxx )
ot_add_source_file ( HyperbolicAnisotropicEnumerateFunction.cxx )
ot_add_source_file ( OrthogonalUniVariatePolynomial.cxx )
ot_add_source_file ( CharlierFactory.cxx )
ot_add_source_file ( HermiteFactory.cxx )
ot_add_source_file ( JacobiFactory.cxx )
ot_add_source_file ( KrawtchoukFactory.cxx )
ot_add_source_file ( LaguerreFactory.cxx )
ot_add_source_file ( LegendreFactory.cxx )
ot_add_source_file ( MeixnerFactory.cxx )
ot_add_source_file ( OrthogonalFunctionFactory.cxx )
ot_add_source_file ( OrthogonalProductPolynomialFactory.cxx )
ot_add_source_file ( OrthogonalUniVariatePolynomialFactory.cxx )
ot_add_source_file ( OrthogonalUniVariatePolynomialFamily.cxx )
ot_add_source_file ( OrthonormalizationAlgorithm.cxx )
ot_add_source_file ( OrthonormalizationAlgorithmImplementation.cxx )
ot_add_source_file ( ChebychevAlgorithm.cxx )
ot_add_source_file ( GramSchmidtAlgorithm.cxx )
ot_add_source_file ( StandardDistributionPolynomialFactory.cxx )
ot_add_source_file ( OrthogonalBasis.cxx )

ot_install_header_file ( OTOrthogonalBasis.hxx )
ot_install_header_file ( OrthogonalFunctionFactory.hxx )
ot_install_header_file ( OrthogonalUniVariatePolynomialFactory.hxx )
ot_install_header_file ( OrthogonalProductPolynomialFactory.hxx )
ot_install_header_file ( StandardDistributionPolynomialFactory.hxx )
ot_install_header_file ( OrthogonalBasis.hxx )
ot_install_header_file ( GramSchmidtAlgorithm.hxx )
ot_install_header_file ( OrthogonalUniVariatePolynomial.hxx )
ot_install_header_file ( OrthonormalizationAlgorithm.hxx )
ot_install_header_file ( OrthogonalUniVariatePolynomialFamily.hxx )
ot_install_header_file ( ChebychevAlgorithm.hxx )
ot_install_header_file ( OrthonormalizationAlgorithmImplementation.hxx )
ot_install_header_file ( EnumerateFunction.hxx )
ot_install_header_file ( EnumerateFunctionImplementation.hxx )
ot_install_header_file ( LinearEnumerateFunction.hxx )
ot_install_header_file ( HyperbolicAnisotropicEnumerateFunction.hxx )
ot_install_header_file ( CharlierFactory.hxx )
ot_install_header_file ( HermiteFactory.hxx )
ot_install_header_file ( JacobiFactory.hxx )
ot_install_header_file ( KrawtchoukFactory.hxx )
ot_install_header_file ( LaguerreFactory.hxx )
ot_install_header_file ( LegendreFactory.hxx )
ot_install_header_file ( MeixnerFactory.hxx )
