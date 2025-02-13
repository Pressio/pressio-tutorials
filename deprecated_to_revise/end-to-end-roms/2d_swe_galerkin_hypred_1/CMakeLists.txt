
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wf.yaml wf.yaml COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/plot.py plot.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/clean.py clean.py COPYONLY)

execute_process(
  COMMAND
  ln -sf
  ${CMAKE_CURRENT_BINARY_DIR}/../${exename}
  ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUT_VARIABLE OUTP)
