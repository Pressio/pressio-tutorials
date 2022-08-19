
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/wf.yaml wf.yaml COPYONLY)

ADD_CUSTOM_TARGET(link_target ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_CURRENT_BINARY_DIR}/../${exename} ${CMAKE_CURRENT_BINARY_DIR}/${exename})

# add_custom_command(
#     OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${exename}
#     COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/../${exename} ${CMAKE_CURRENT_SOURCE_DIR}
#     COMMENT "Copying"
#     )
