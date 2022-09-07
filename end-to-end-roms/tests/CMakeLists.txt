
enable_testing()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../cpp)

configure_file(fake_basis.bin ${CMAKE_CURRENT_BINARY_DIR}/fake_basis.bin COPYONLY)
configure_file(fake_affine_shift.txt ${CMAKE_CURRENT_BINARY_DIR}/fake_affine_shift.txt COPYONLY)
configure_file(fake_sample_mesh_gids.txt ${CMAKE_CURRENT_BINARY_DIR}/fake_sample_mesh_gids.txt COPYONLY)
configure_file(fake_stencil_mesh_gids.txt ${CMAKE_CURRENT_BINARY_DIR}/fake_stencil_mesh_gids.txt COPYONLY)

set(testName test1)
add_executable(${testName} main.cc)
add_test(NAME ${testName} COMMAND ${testName})
set_tests_properties(
  ${testName}
  PROPERTIES
  PASS_REGULAR_EXPRESSION "PASS"
  FAIL_REGULAR_EXPRESSION "FAILED"
)
