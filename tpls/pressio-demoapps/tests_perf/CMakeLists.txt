
include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/../include)

set(exename  perf_exe)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
configure_file(drive.py drive.py COPYONLY)
configure_file(plot.py plot.py COPYONLY)
