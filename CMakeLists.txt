
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials CXX)
set (CMAKE_CXX_STANDARD 11)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/pressio/include)

set(TUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

add_executable(
  utils_logger
  ${TUTDIR}/utils_logger.cc)

#=================================
# nonlinear solvers
#=================================
add_executable(
  nonlinsolvers_newtonraphson_known_types
  ${TUTDIR}/nonlinsolvers_newtonraphson_1.cc)

add_executable(
  nonlinsolvers_newtonraphson_custom_types
  ${TUTDIR}/nonlinsolvers_newtonraphson_2.cc)

add_executable(
  nonlinsolvers_gn_known_types
  ${TUTDIR}/nonlinsolvers_gn_1.cc)

add_executable(
  nonlinsolvers_gn_custom_types
  ${TUTDIR}/nonlinsolvers_gn_2.cc)

#=================================
# ODE
#=================================
add_executable(
  ode_steppers_known_types
  ${TUTDIR}/ode_steppers_1.cc)

add_executable(
  ode_steppers_custom_types
  ${TUTDIR}/ode_steppers_2.cc)

#=================================
# rom
#=================================
add_executable(
  rom_linear_decoder_known_types
  ${TUTDIR}/rom_linear_decoder_eigen.cc)

add_executable(
  rom_linear_decoder_custom_types
  ${TUTDIR}/rom_linear_decoder_arbitrary_types.cc)

#---- galerkin -----
add_executable(
  rom_default_explicit_galerkin_known_types
  ${TUTDIR}/rom_default_explicit_galerkin_eigen.cc)

add_executable(
  rom_default_explicit_galerkin_custom_types
  ${TUTDIR}/rom_default_explicit_galerkin_custom_types.cc)

add_executable(
  rom_default_implicit_galerkin_known_types
  ${TUTDIR}/rom_default_implicit_galerkin_eigen.cc)

# #add_subdirectory(tutorials/swe2d ${CMAKE_CURRENT_BINARY_DIR}/swe2d)
