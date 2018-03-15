cmake_minimum_required(VERSION 3.5)
project(nx_udp)


set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules")
set(CMAKE_CXX_STANDARD 11)


find_package(asio REQUIRED)
find_package(Threads REQUIRED)


include_directories(${ASIO_INCLUDE_DIRS})


add_executable(app src/main.cpp src/ProgramOptions.h)


target_link_libraries(app Threads::Threads)
