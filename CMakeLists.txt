cmake_minimum_required(VERSION 3.5)
project(nxudp)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

find_package(asio REQUIRED)
find_package(Threads REQUIRED)


include_directories(${ASIO_INCLUDE_DIRS})


set(NXUDP_SOURCES
	    src/export.h
        src/endpoint_utils.h
        src/endpoint_utils.cpp
        src/program_options.h
        src/program_options.cpp
        src/client.h
        src/client.cpp
        src/server.h
        src/server.cpp
        src/client_waiter.h
        src/client_waiter.cpp
        src/int_buffer.h
        src/int_buffer.cpp
        src/print_stream.h
        src/client_info.cpp
        src/client_info.h)


if(WIN32)
    # used in export.h for exporting nxudp library target
    add_definitions(-DNXUDP_EXPORTS)
endif()


add_library(nxudp SHARED ${NXUDP_SOURCES})
target_link_libraries(nxudp Threads::Threads)

add_executable(app src/main.cpp)
target_link_libraries(app nxudp Threads::Threads)

add_dependencies(app nxudp)

install(TARGETS app nxudp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)