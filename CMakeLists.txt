cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
cmake_policy(VERSION 3.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GpuPro)

# Import the dependency library targets
add_subdirectory("shared/dependencies" "${CMAKE_BINARY_DIR}/dependencies")
add_subdirectory("shared/framework" "${CMAKE_BINARY_DIR}/framework")

# macro to list all subdirectories
MACRO(SUBDIRLIST result curdir)
  FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
  SET(dirlist "")
  FOREACH(child ${children})
    IF(IS_DIRECTORY ${curdir}/${child})
      LIST(APPEND dirlist ${child})
    ENDIF()
  ENDFOREACH()
  SET(${result} ${dirlist})
ENDMACRO()

# automatically detect project dirs and add them as subdirectory
set (EXCLUDE_DIR "build")
SUBDIRLIST(SUBDIRS ${CMAKE_CURRENT_SOURCE_DIR})
foreach(SUBDIR ${SUBDIRS})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SUBDIR}/CMakeLists.txt")
        add_subdirectory(${SUBDIR})
    endif()
endforeach()

# list project names manually in case the foreach does not work
#add_subdirectory(ex1_buffer)
#add_subdirectory(ex2)