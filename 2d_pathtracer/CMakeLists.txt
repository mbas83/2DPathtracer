# obtain project name from directory name
get_filename_component(GPUPRO_EXERCISE_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" GPUPRO_EXERCISE_NAME ${GPUPRO_EXERCISE_NAME})

project(${GPUPRO_EXERCISE_NAME} LANGUAGES CXX)

# Hide the default CMake targets in a filter
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "CMakePredefinedTargets")

# Set output directories (overwrite Visual Studio's Debug/Release scheme)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

add_compile_definitions(PROJECT_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

# Find all source files (dont do recursive, it might take the build directory)
file(GLOB_RECURSE GPUPRO_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp")
file(GLOB_RECURSE GPUPRO_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

# Executable
add_executable(${GPUPRO_EXERCISE_NAME}
	"${GPUPRO_HEADERS}"
	"${GPUPRO_SOURCES}"
)

if(MSVC)
	option(GPUPRO_USE_MESA "Use the software renderer MESA" OFF)
endif()

# Link the exec's
# If we're on Linux we need some extra libraries
if(WIN32)
	target_link_libraries(${GPUPRO_EXERCISE_NAME}
		glfw GPUProFramework
	)
else()
	target_link_libraries(${GPUPRO_EXERCISE_NAME}
		glfw dl X11 pthread GPUProFramework
	)
endif()
if(GPUPRO_USE_MESA)
	target_link_libraries(${GPUPRO_EXERCISE_NAME}
		mesa
	)
endif()

# Copy over the necessary DLLs
if(MSVC AND GPUPRO_USE_MESA)
	get_target_property(GPUPRO_OUTPUT_DIR ${GPUPRO_EXERCISE_NAME} RUNTIME_OUTPUT_DIRECTORY)
	add_custom_command(TARGET ${GPUPRO_EXERCISE_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
			"${GPUPRO_SHARED_LIB_ARTIFACTS}"
			"${GPUPRO_OUTPUT_DIR}/$<CONFIG>"
	)
endif()