find_path( ASIO_INCLUDE_DIRS asio.hpp
        HINTS
            "${PROJECT_SOURCE_DIR}/vendor/asio/asio/include"
            "/usr/include"
            "/usr/local/include"
            "/opt/local/include"
            "C:/asio")

if ( ASIO_INCLUDE_DIRS )
    set( ASIO_FOUND TRUE )
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DASIO_STANDALONE=YES -DASIO_NO_DEPRECATED" )

    message( STATUS "${Green}Found ASIO include at: ${ASIO_INCLUDE_DIRS}.${Reset}" )
else ( )
    message( FATAL_ERROR "${Red}Failed to locate ASIO dependency.${Reset}" )
endif ( )
