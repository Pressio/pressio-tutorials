
cmake_minimum_required(VERSION 2.8.12)
project(pressio-tutorials)

# for now, tutorials depend only on pressio with Eigen
include_directories(
  ${EIGEN_INCLUDE_DIR} ${PRESSIO_INCLUDE_DIR}
  )

# where the mains are located
set(MAINDIR ${CMAKE_CURRENT_SOURCE_DIR}/tutorials)

#-------------------
# eigen includes
set(EIGENINCDIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}/functors)

# eigen fom
add_executable(adr2d_eigen_fom ${MAINDIR}/main_eigen_fom.cc)
target_include_directories(adr2d_eigen_fom BEFORE PUBLIC ${EIGENINCDIRS})
target_link_libraries(adr2d_eigen_fom ${TRILLIBNAMES})
