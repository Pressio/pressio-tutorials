cmake_minimum_required(VERSION 3.18)

project(burgers_tutorial CXX)

# Ensure C++17 (root also enforces this, but keep local for safety)
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORT_CPP17)
if(NOT COMPILER_SUPPORT_CPP17)
	message(FATAL_ERROR "Compiler does not support -std=c++17. This is required.")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(exename burgers)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/burgers.cpp)

# Pull in the headers
target_include_directories(${exename} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen-3.4.0
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls                 # pressio_cmake_config.h
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-rom/include
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-ops/include
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-log/include
)
