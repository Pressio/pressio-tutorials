
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio/include)

set(EXENAMEROOT solvers_eigen)
add_executable(${EXENAMEROOT}_exe1 main1.cc)
