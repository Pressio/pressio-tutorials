cmake_minimum_required(VERSION 3.18)

project(burgers_tutorial CXX)

set(exename burgers)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/burgers.cpp)

# Require C++20 for this example (needed for auto in template parameters)
set_target_properties(${exename} PROPERTIES
	CXX_STANDARD 20
	CXX_STANDARD_REQUIRED ON
	CXX_EXTENSIONS OFF
)

# Pull in the headers
target_include_directories(${exename} PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen-3.4.0
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls                 # pressio_cmake_config.h
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-rom/include
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-ops/include
	${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-log/include
)
