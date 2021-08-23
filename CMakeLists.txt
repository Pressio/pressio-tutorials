
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

add_executable(
  ode_steppers_t1
  ${TUTDIR}/ode_steppers_1.cc)

add_executable(
  ode_steppers_t2
  ${TUTDIR}/ode_steppers_2.cc)

add_executable(
  nonlinsolvers_newtonraphson_t1
  ${TUTDIR}/nonlinsolvers_newtonraphson_1.cc)

add_executable(
  nonlinsolvers_newtonraphson_t2
  ${TUTDIR}/nonlinsolvers_newtonraphson_2.cc)

add_executable(
  nonlinsolvers_gn_t1
  ${TUTDIR}/nonlinsolvers_gn_1.cc)

add_executable(
  nonlinsolvers_gn_t2
  ${TUTDIR}/nonlinsolvers_gn_2.cc)

add_executable(
  rom_linear_decoder_t1
  ${TUTDIR}/rom_linear_decoder_eigen.cc)

add_executable(
  rom_linear_decoder_t2
  ${TUTDIR}/rom_linear_decoder_arbitrary_types.cc)


#add_executable(tut_rom_1 ${TUTDIR}/tut_rom_1.cc)
#add_executable(tut_rom_2 ${TUTDIR}/tut_rom_2.cc)
#add_subdirectory(tutorials/swe2d ${CMAKE_CURRENT_BINARY_DIR}/swe2d)
