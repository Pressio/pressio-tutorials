
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio/include)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/runall_for_ci.sh runall_for_ci.sh COPYONLY)
set(EXENAMEROOT ode_eigen)

add_executable(${EXENAMEROOT}_exe1 main1.cc)
add_subdirectory(tutorial2)
add_subdirectory(tutorial3)
