
cmake_minimum_required(VERSION 3.18.0)
project(pressio-tutorials CXX)

#===================================================
# c++ standard
#===================================================
# commands to test if compiler supports standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++14" COMPILER_SUPPORT_CPP14)
if(NOT COMPILER_SUPPORT_CPP14)
  message(FATAL_ERROR "Compiler does not support -std=c++14. This is required.")
endif()
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/end-to-end-roms)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/ode-eigen)
