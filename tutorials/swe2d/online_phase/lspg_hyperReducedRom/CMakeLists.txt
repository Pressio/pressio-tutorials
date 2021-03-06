
configure_file(viewSolutionAndMakePlots.py viewSolutionAndMakePlots.py COPYONLY)
add_executable(run_lspg_with_hyperreduction ${CMAKE_CURRENT_SOURCE_DIR}/main.cc)
