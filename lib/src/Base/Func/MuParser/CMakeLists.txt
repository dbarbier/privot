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

ot_add_source_file ( muParserBase.cpp )
ot_add_source_file ( muParserBytecode.cpp )
ot_add_source_file ( muParserCallback.cpp )
ot_add_source_file ( muParserDLL.cpp )
ot_add_source_file ( muParserError.cpp )
ot_add_source_file ( muParserInt.cpp )
ot_add_source_file ( muParserTest.cpp )
ot_add_source_file ( muParserTokenReader.cpp )
ot_add_source_file ( muParser.cpp )

ot_install_header_file ( muParserBase.h )
ot_install_header_file ( muParserBytecode.h )
ot_install_header_file ( muParserCallback.h )
ot_install_header_file ( muParserDef.h )
ot_install_header_file ( muParserDLL.h )
ot_install_header_file ( muParserError.h )
ot_install_header_file ( muParserFixes.h )
ot_install_header_file ( muParserInt.h )
ot_install_header_file ( muParserStack.h )
ot_install_header_file ( muParserTemplateMagic.h )
ot_install_header_file ( muParserTest.h )
ot_install_header_file ( muParserToken.h )
ot_install_header_file ( muParserTokenReader.h )
ot_install_header_file ( muParser.h )

