include(FindPythonInterp)
set (python_file "test_lspg_default.py")
set (build_dir "${CMAKE_CURRENT_BINARY_DIR}../../../..")
add_test(
    NAME test_lspg_default
    COMMAND ${PYTHON_EXECUTABLE} ${python_file} ${build_dir}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)