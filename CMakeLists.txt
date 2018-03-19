cmake_minimum_required(VERSION 3.5)
project(nx_udp)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")

find_package(asio REQUIRED)
find_package(Threads REQUIRED)


include_directories(${ASIO_INCLUDE_DIRS})


set(NXUDP_SOURCES
        src/client.h
        src/server.h
        src/client_waiter.h
        src/program_options.h
        src/client.cpp
        src/server.cpp
        src/client_waiter.cpp)


add_library(nxudp SHARED ${NXUDP_SOURCES})
target_link_libraries(nxudp Threads::Threads)


add_executable(app src/main.cpp)
target_link_libraries(app nxudp)


install(TARGETS app nxudp
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib)