cmake_minimum_required ( VERSION 2.8 )

project ( wrapper_linked_with_C_function C )

include ( OpenTURNSConfig )
include ( ${OPENTURNS_USE_FILE} )

set ( SOURCEFILES myCFunction.c wrapper.c )

add_library ( wcode SHARED ${SOURCEFILES} )
set_target_properties ( wcode PROPERTIES PREFIX "" )
target_link_libraries ( wcode ${OPENTURNS_LIBRARIES} -lOTbind )

install ( TARGETS wcode
          RUNTIME DESTINATION wrappers
          LIBRARY DESTINATION wrappers
          ARCHIVE DESTINATION wrappers
        )
        
install ( FILES wcode.xml DESTINATION wrappers )
