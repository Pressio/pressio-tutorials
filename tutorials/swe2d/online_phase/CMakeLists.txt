
# this is needed by both executables built below
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/..)

add_subdirectory(lspg_rom)
add_subdirectory(lspg_hyperReducedRom)
