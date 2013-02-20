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

ot_add_source_file ( Distribution.cxx )
ot_add_source_file ( DistributionImplementation.cxx )
ot_add_source_file ( UsualDistribution.cxx )
ot_add_source_file ( ContinuousDistribution.cxx )
ot_add_source_file ( DiscreteDistribution.cxx )
ot_add_source_file ( EllipticalDistribution.cxx )
ot_add_source_file ( NonEllipticalDistribution.cxx )
ot_add_source_file ( Copula.cxx )
ot_add_source_file ( CopulaImplementation.cxx )
ot_add_source_file ( SklarCopula.cxx )
ot_add_source_file ( ArchimedeanCopula.cxx )
ot_add_source_file ( RandomVector.cxx )
ot_add_source_file ( Event.cxx )
ot_add_source_file ( RandomVectorImplementation.cxx )
ot_add_source_file ( ConditionalRandomVector.cxx )
ot_add_source_file ( ConstantRandomVector.cxx )
ot_add_source_file ( CompositeRandomVector.cxx )
ot_add_source_file ( FunctionalChaosRandomVector.cxx )
ot_add_source_file ( UsualRandomVector.cxx )
ot_add_source_file ( EventDomainImplementation.cxx )
ot_add_source_file ( EventRandomVectorImplementation.cxx )
ot_add_source_file ( EventProcess.cxx )
ot_add_source_file ( StandardEvent.cxx )
ot_add_source_file ( DistributionFactory.cxx )
ot_add_source_file ( DistributionImplementationFactory.cxx )
ot_add_source_file ( ProcessImplementation.cxx )
ot_add_source_file ( Process.cxx )

ot_install_header_file ( RandomVectorImplementation.hxx )
ot_install_header_file ( SklarCopula.hxx )
ot_install_header_file ( Event.hxx )
ot_install_header_file ( RandomVector.hxx )
ot_install_header_file ( EllipticalDistribution.hxx )
ot_install_header_file ( ContinuousDistribution.hxx )
ot_install_header_file ( NonEllipticalDistribution.hxx )
ot_install_header_file ( CompositeRandomVector.hxx )
ot_install_header_file ( ConditionalRandomVector.hxx )
ot_install_header_file ( DistributionFactory.hxx )
ot_install_header_file ( DistributionImplementation.hxx )
ot_install_header_file ( FunctionalChaosRandomVector.hxx )
ot_install_header_file ( UsualRandomVector.hxx )
ot_install_header_file ( StandardEvent.hxx )
ot_install_header_file ( Distribution.hxx )
ot_install_header_file ( EventDomainImplementation.hxx )
ot_install_header_file ( EventRandomVectorImplementation.hxx )
ot_install_header_file ( EventProcess.hxx )
ot_install_header_file ( ArchimedeanCopula.hxx )
ot_install_header_file ( CopulaImplementation.hxx )
ot_install_header_file ( DistributionImplementationFactory.hxx )
ot_install_header_file ( DiscreteDistribution.hxx )
ot_install_header_file ( ConstantRandomVector.hxx )
ot_install_header_file ( OTModel.hxx )
ot_install_header_file ( Copula.hxx )
ot_install_header_file ( UsualDistribution.hxx )
ot_install_header_file ( ProcessImplementation.hxx )
ot_install_header_file ( Process.hxx )

