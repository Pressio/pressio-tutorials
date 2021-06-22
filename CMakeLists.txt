
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials CXX)
set (CMAKE_CXX_STANDARD 11)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/tpls/pressio/include)

set(TUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

add_executable(tut_ode_1 ${TUTDIR}/tut_ode_1.cc)
add_executable(tut_ode_2 ${TUTDIR}/tut_ode_2.cc)
add_executable(tut_rom_1 ${TUTDIR}/tut_rom_1.cc)
add_executable(tut_rom_2 ${TUTDIR}/tut_rom_2.cc)
add_subdirectory(tutorials/swe2d ${CMAKE_CURRENT_BINARY_DIR}/swe2d)
