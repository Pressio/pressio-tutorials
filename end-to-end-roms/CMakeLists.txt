
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/yaml-cpp-0.7.0
  ${CMAKE_CURRENT_BINARY_DIR}/yamlcpp)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen/eigen3
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-demoapps/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/yaml-cpp-0.7.0/include)

set(exename pdaWfExe)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cc)
target_link_libraries(${exename} PRIVATE yaml-cpp)
target_compile_options(${exename} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-march=native>)

add_subdirectory(2d_swe_default_galerkin)
