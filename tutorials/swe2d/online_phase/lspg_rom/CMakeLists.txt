
configure_file(viewSolutionAndMakePlots.py viewSolutionAndMakePlots.py COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/../../webplotsetting.py webplotsetting.py COPYONLY)
add_executable(run_lspg ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
