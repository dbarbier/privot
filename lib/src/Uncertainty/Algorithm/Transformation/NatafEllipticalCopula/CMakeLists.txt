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

ot_add_source_file ( NatafEllipticalCopulaEvaluation.cxx )
ot_add_source_file ( NatafEllipticalCopulaGradient.cxx )
ot_add_source_file ( NatafEllipticalCopulaHessian.cxx )
ot_add_source_file ( InverseNatafEllipticalCopulaEvaluation.cxx )
ot_add_source_file ( InverseNatafEllipticalCopulaGradient.cxx )
ot_add_source_file ( InverseNatafEllipticalCopulaHessian.cxx )

ot_install_header_file ( InverseNatafEllipticalCopulaGradient.hxx )
ot_install_header_file ( NatafEllipticalCopulaEvaluation.hxx )
ot_install_header_file ( InverseNatafEllipticalCopulaEvaluation.hxx )
ot_install_header_file ( NatafEllipticalCopulaGradient.hxx )
ot_install_header_file ( NatafEllipticalCopulaHessian.hxx )
ot_install_header_file ( InverseNatafEllipticalCopulaHessian.hxx )
ot_install_header_file ( OTNatafEllipticalCopula.hxx )
