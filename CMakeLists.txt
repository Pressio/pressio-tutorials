
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials)
set (CMAKE_CXX_STANDARD 11)

# for now, tutorials depend only on pressio with Eigen
include_directories(${EIGEN_INCLUDE_DIR} ${PRESSIO_INCLUDE_DIR})

# where the tutorial mains are located
set(TUTDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)
# where the Burgers1d app is located
set(BURG1DDIR ${CMAKE_CURRENT_SOURCE_DIR}/burgers1d)

add_executable(tutorial1 ${TUTDIR}/tutorial1.cc)
add_executable(tutorial2 ${TUTDIR}/tutorial2.cc)
add_executable(tutorial3 ${TUTDIR}/tutorial3.cc)

#add_executable(tutorial4 ${TUTDIR}/tutorial4/tutorial4.cc)
#target_include_directories(tutorial4 BEFORE PUBLIC ${BURG1DDIR})
