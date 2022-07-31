
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

#===================================================
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/pressio/include)

set(TUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

# #===================================================
# # add all executables
# #===================================================
# add_executable(
#   utils_logger
#   ${TUTDIR}/utils_logger.cc)

#------------------
# nonlinear solvers
#------------------
add_executable(
  nonlinsolvers_newtonraphson_known_types_eigen
  ${TUTDIR}/nonlinsolvers_newtonraphson_known_types_eigen.cc)

add_executable(
  nonlinsolvers_newtonraphson_custom_types
  ${TUTDIR}/nonlinsolvers_newtonraphson_custom_types.cc)

add_executable(
  nonlinsolvers_gn_known_types_eigen
  ${TUTDIR}/nonlinsolvers_gn_known_types_eigen.cc)

add_executable(
  nonlinsolvers_gn_custom_types
  ${TUTDIR}/nonlinsolvers_gn_custom_types.cc)

add_executable(
  nonlinsolvers_gn_qr_known_types
  ${TUTDIR}/nonlinsolvers_gn_qr_known_types.cc)

add_executable(
  nonlinsolvers_lm_known_types
  ${TUTDIR}/nonlinsolvers_lm_known_types.cc)

add_executable(
  nonlinsolvers_lm_custom_types
  ${TUTDIR}/nonlinsolvers_lm_custom_types.cc)

add_executable(
  nonlinsolvers_irls_known_types
  ${TUTDIR}/nonlinsolvers_irls_known_types.cc)

#------------------
# ODE
#------------------
add_executable(
  ode_steppers_known_types_eigen
  ${TUTDIR}/ode_steppers_known_types_eigen.cc)

add_executable(
  ode_steppers_custom_types
  ${TUTDIR}/ode_steppers_custom_types.cc)
