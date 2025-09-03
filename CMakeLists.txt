
cmake_minimum_required(VERSION 3.18.0)
project(pressio-tutorials CXX)

function(get_version_number PATH_TO_FILE OUT)
  file(READ ${PATH_TO_FILE} tmpver)

  string(FIND ${OUT} MAJOR wantMajor)
  string(FIND ${OUT} MINOR wantMinor)
  string(FIND ${OUT} PATCH wantPatch)
  if (wantMajor GREATER -1)
    string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${tmpver})
  endif()
  if (wantMinor GREATER -1)
    string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${tmpver})
  endif()
  if (wantPatch GREATER -1)
    string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${tmpver})
  endif()
  set(${OUT} ${CMAKE_MATCH_1} PARENT_SCOPE)
endfunction()


#===================================================
# versioning info
#===================================================

get_version_number("version.txt" PressioTutorials_VERSION_MAJOR)
get_version_number("version.txt" PressioTutorials_VERSION_MINOR)
get_version_number("version.txt" PressioTutorials_VERSION_PATCH)
set(PressioTutorials_VERSION "${PressioTutorials_VERSION_MAJOR}.${PressioTutorials_VERSION_MINOR}.${PressioTutorials_VERSION_PATCH}")
message("pressio-tutorials version = ${PressioTutorials_VERSION}")

#===================================================
# fetch pressio libraries
#===================================================

set(PRESSIO_REPOS "demoapps" "rom" "ops" "log")
foreach(repo IN LISTS PRESSIO_REPOS)
  if (NOT EXISTS ${CMAKE_SOURCE_DIR}/tpls/pressio-${repo})
    execute_process(
      COMMAND git clone --branch ${PressioTutorials_VERSION}
              https://github.com/Pressio/pressio-${repo}.git
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tpls
      OUTPUT_QUIET
    )
  endif()
endforeach()

#===================================================
# c++ standard
#===================================================
# commands to test if compiler supports standard
include(CheckCXXCompilerFlag)
check_cxx_compiler_flag("-std=c++17" COMPILER_SUPPORT_CPP17)
if(NOT COMPILER_SUPPORT_CPP17)
  message(FATAL_ERROR "Compiler does not support -std=c++17. This is required.")
endif()
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/end-to-end-roms)
