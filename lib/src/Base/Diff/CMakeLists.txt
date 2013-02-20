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

ot_add_source_file ( FiniteDifferenceStepImplementation.cxx )
ot_add_source_file ( FiniteDifferenceStep.cxx )
ot_add_source_file ( ConstantStep.cxx )
ot_add_source_file ( BlendedStep.cxx )
ot_add_source_file ( FiniteDifferenceGradient.cxx )
ot_add_source_file ( FiniteDifferenceHessian.cxx )
ot_add_source_file ( NonCenteredFiniteDifferenceGradient.cxx )
ot_add_source_file ( CenteredFiniteDifferenceGradient.cxx )
ot_add_source_file ( CenteredFiniteDifferenceHessian.cxx )

ot_install_header_file ( FiniteDifferenceStepImplementation.hxx )
ot_install_header_file ( FiniteDifferenceStep.hxx )
ot_install_header_file ( ConstantStep.hxx )
ot_install_header_file ( BlendedStep.hxx )
ot_install_header_file ( FiniteDifferenceGradient.hxx )
ot_install_header_file ( FiniteDifferenceHessian.hxx )
ot_install_header_file ( NonCenteredFiniteDifferenceGradient.hxx )
ot_install_header_file ( CenteredFiniteDifferenceGradient.hxx )
ot_install_header_file ( OTDiff.hxx )
ot_install_header_file ( CenteredFiniteDifferenceHessian.hxx )

# Recurse in subdirectories
add_subdirectory ( Ev3 )