
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials CXX)
set (CMAKE_CXX_STANDARD 11)

# for now, tutorials depend only on pressio with Eigen
include_directories(${EIGEN_INCLUDE_DIR} ${PRESSIO_INCLUDE_DIR})

set(TUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

add_executable(tutorial1 ${TUTDIR}/tutorial1.cc)
add_executable(tutorial2 ${TUTDIR}/tutorial2.cc)
add_executable(tutorial3 ${TUTDIR}/tutorial3.cc)
add_executable(tutorial4 ${TUTDIR}/tutorial4.cc)
add_executable(tutorial5 ${TUTDIR}/tutorial5.cc)
add_subdirectory(tutorials/swe2d ${CMAKE_CURRENT_BINARY_DIR}/swe2d)
