
add_subdirectory(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/yaml-cpp-0.7.0
  ${CMAKE_CURRENT_BINARY_DIR}/yamlcpp)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/eigen-3.4.0
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-demoapps/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls #this is needed to include the pressio_cmake_config.h
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-rom/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-ops/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/pressio-log/include
  ${CMAKE_CURRENT_SOURCE_DIR}/../tpls/yaml-cpp-0.7.0/include)

set(exename pdaWfExe)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/cpp/main.cc)
target_link_libraries(${exename} PRIVATE yaml-cpp)
target_compile_options(${exename} PRIVATE $<$<COMPILE_LANGUAGE:CXX>:-march=native>)

add_subdirectory(2d_swe_galerkin_default)
add_subdirectory(2d_swe_galerkin_hypred_1)
add_subdirectory(2d_swe_lspg_default)
add_subdirectory(2d_swe_lspg_hypred_1)

if(PRESSIOTUTORIALS_ENABLE_TESTS)
  add_subdirectory(tests)
endif()
