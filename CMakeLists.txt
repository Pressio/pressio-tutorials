
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials)
set (CMAKE_CXX_STANDARD 11)

# for now, tutorials depend only on pressio with Eigen
include_directories(${EIGEN_INCLUDE_DIR} ${PRESSIO_INCLUDE_DIR})

# where the tutorial mains are located
set(MAINDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

add_executable(tutorial1 ${MAINDIR}/tutorial1.cc)
add_executable(tutorial2 ${MAINDIR}/tutorial2.cc)
