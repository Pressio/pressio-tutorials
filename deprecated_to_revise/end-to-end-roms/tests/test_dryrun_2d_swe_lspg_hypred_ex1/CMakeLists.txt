include(FindPythonInterp)
set (python_file "test.py")

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/../../2d_swe_lspg_hypred_1/wf.yaml
  ${CMAKE_CURRENT_BINARY_DIR}/wf.yaml COPYONLY)

add_test(
  NAME test_dryrun_2d_swe_lspg_hypred_ex1
  COMMAND ${PYTHON_EXECUTABLE} ${python_file} "${CMAKE_CURRENT_BINARY_DIR}"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
