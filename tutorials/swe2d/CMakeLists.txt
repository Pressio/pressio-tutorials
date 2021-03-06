
set(exename run_fom)
add_executable(${exename} ${CMAKE_CURRENT_SOURCE_DIR}/main_fom.cc)
target_include_directories(${exename} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# inside the build, we need to copy the fom driver to a subdirectory online/fom where
# we can run the predictive fom 
add_custom_command(TARGET run_fom POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:run_fom> ${CMAKE_CURRENT_BINARY_DIR}/online_phase/fom/gigi.exe
    COMMENT "Created ${PROJECT_BINARY_DIR}/run_fom.exe"
)

add_subdirectory(offline_phase)
add_subdirectory(online_phase)
