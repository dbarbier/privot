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
        
install ( FILES WrapperTemplates/wrapper_calling_shell_command/README
                WrapperTemplates/wrapper_calling_shell_command/AUTHORS
                WrapperTemplates/wrapper_calling_shell_command/wrapper.c
                WrapperTemplates/wrapper_calling_shell_command/code_C1.c
                WrapperTemplates/wrapper_calling_shell_command/ChangeLog
                WrapperTemplates/wrapper_calling_shell_command/test.py
                WrapperTemplates/wrapper_calling_shell_command/wcode.xml
                WrapperTemplates/wrapper_calling_shell_command/CMakeLists.txt
                WrapperTemplates/wrapper_calling_shell_command/code_C1.data
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_calling_shell_command
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
        
install ( FILES WrapperTemplates/wrapper_calling_shell_command/customize
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_calling_shell_command
          PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        )

install ( FILES WrapperTemplates/wrapper_linked_with_C_function/README
                WrapperTemplates/wrapper_linked_with_C_function/AUTHORS
                WrapperTemplates/wrapper_linked_with_C_function/wrapper.c
                WrapperTemplates/wrapper_linked_with_C_function/ChangeLog
                WrapperTemplates/wrapper_linked_with_C_function/test.py
                WrapperTemplates/wrapper_linked_with_C_function/wcode.xml
                WrapperTemplates/wrapper_linked_with_C_function/myCFunction.h
                WrapperTemplates/wrapper_linked_with_C_function/CMakeLists.txt
                WrapperTemplates/wrapper_linked_with_C_function/myCFunction.c
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_linked_with_C_function
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
        
install ( FILES WrapperTemplates/wrapper_linked_with_C_function/customize
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_linked_with_C_function
          PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        )

install ( FILES WrapperTemplates/wrapper_linked_with_F77_function/README
                WrapperTemplates/wrapper_linked_with_F77_function/AUTHORS
                WrapperTemplates/wrapper_linked_with_F77_function/wrapper.c
                WrapperTemplates/wrapper_linked_with_F77_function/ChangeLog
                WrapperTemplates/wrapper_linked_with_F77_function/test.py
                WrapperTemplates/wrapper_linked_with_F77_function/wcode.xml
                WrapperTemplates/wrapper_linked_with_F77_function/code.f
                WrapperTemplates/wrapper_linked_with_F77_function/CMakeLists.txt
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_linked_with_F77_function
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
        
install ( FILES WrapperTemplates/wrapper_linked_with_F77_function/customize
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_linked_with_F77_function
          PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        )

install ( FILES WrapperTemplates/wrapper_python_distributed/input_template
                WrapperTemplates/wrapper_python_distributed/README.txt
                WrapperTemplates/wrapper_python_distributed/start_it.bat
                WrapperTemplates/wrapper_python_distributed/TODO.txt
                WrapperTemplates/wrapper_python_distributed/VERSION.txt
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_python_distributed
          PERMISSIONS OWNER_READ GROUP_READ WORLD_READ
        )
        
install ( FILES WrapperTemplates/wrapper_python_distributed/ot_script.py
                WrapperTemplates/wrapper_python_distributed/program.py
                WrapperTemplates/wrapper_python_distributed/start_it.sh
                WrapperTemplates/wrapper_python_distributed/program_wrapper.py
          DESTINATION ${OPENTURNS_DATA_PATH}/openturns/WrapperTemplates/wrapper_python_distributed
          PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ OWNER_EXECUTE GROUP_EXECUTE WORLD_EXECUTE
        )
