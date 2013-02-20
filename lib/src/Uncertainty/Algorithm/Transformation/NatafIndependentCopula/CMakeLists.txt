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

ot_add_source_file ( NatafIndependentCopulaEvaluation.cxx )
ot_add_source_file ( NatafIndependentCopulaGradient.cxx )
ot_add_source_file ( NatafIndependentCopulaHessian.cxx )
ot_add_source_file ( InverseNatafIndependentCopulaEvaluation.cxx )
ot_add_source_file ( InverseNatafIndependentCopulaGradient.cxx )
ot_add_source_file ( InverseNatafIndependentCopulaHessian.cxx )

ot_install_header_file ( NatafIndependentCopulaGradient.hxx )
ot_install_header_file ( InverseNatafIndependentCopulaEvaluation.hxx )
ot_install_header_file ( InverseNatafIndependentCopulaHessian.hxx )
ot_install_header_file ( OTNatafIndependentCopula.hxx )
ot_install_header_file ( NatafIndependentCopulaHessian.hxx )
ot_install_header_file ( NatafIndependentCopulaEvaluation.hxx )
ot_install_header_file ( InverseNatafIndependentCopulaGradient.hxx )
