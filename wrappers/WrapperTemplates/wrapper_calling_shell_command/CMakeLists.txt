cmake_minimum_required ( VERSION 2.8 )

project ( wrapper_calling_shell_command C )

include ( OpenTURNSConfig )
include ( ${OPENTURNS_USE_FILE} )

set ( SOURCEFILES wrapper.c )

add_library ( wcode SHARED ${SOURCEFILES} )
set_target_properties ( wcode PROPERTIES PREFIX "" )
target_link_libraries ( wcode ${OPENTURNS_LIBRARIES} -lOTbind )

install ( TARGETS wcode
          RUNTIME DESTINATION wrappers
          LIBRARY DESTINATION wrappers
          ARCHIVE DESTINATION wrappers
        )
        
install ( FILES wcode.xml DESTINATION wrappers )

add_executable( code_C1 code_C1.c )
if ( UNIX )
  target_link_libraries ( code_C1 m )
endif ()

install ( TARGETS code_C1
          DESTINATION bin
        )
