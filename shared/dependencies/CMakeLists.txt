# Add targets for the different library dependencies

# This policy influences how if()-expressions expand the tested variables
cmake_policy(SET CMP0054 NEW)

# glm
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/glm/glm" "${CMAKE_BINARY_DIR}/dependencies/glm")

# Also header-only library
add_library(stbi INTERFACE)
target_include_directories(stbi
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}"
)

# GLAD needs to be build
add_library(glad
	"${CMAKE_CURRENT_SOURCE_DIR}/glad/src/glad.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/glad/include/glad/glad.h"
	"${CMAKE_CURRENT_SOURCE_DIR}/glad/include/KHR/khrplatform.h"
)
target_include_directories(glad
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/glad/include"
)
# GLI is header only with multiple files
add_library(gli INTERFACE)
target_include_directories(gli
	INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/gli/include"
)

# We only support 64-bit compilers
if("${CMAKE_SIZEOF_VOID_P}" STREQUAL "8")
    set(LIBRARY_SUBPATH "lib64")
    set(BINARY_SUBPATH "bin64")
else()
	message(FATAL_ERROR "We require the target to be 64-bit! Please select x64/AMD64 when configuring with MSVC or use MinGW-W64")
endif()

# Pre-compiled library 
# First check which artifact we link against depending on compiler version
if(MSVC)
	if(MSVC_VERSION GREATER_EQUAL 1900)
		message(STATUS "VC 15")
		set(GPUPRO_GLFW3_LIBRARY_ARTIFACT "vc15_glfw3.lib")
	elseif(MSVC_VERSION GREATER_EQUAL 1700)
		message(STATUS "VC 15")
		set(GPUPRO_GLFW3_LIBRARY_ARTIFACT "vc14_glfw3.lib")
	else()
		message(WARNING "Unknown Visual Studio version (version number ${MSVC_VERSION}); attempting to use GLFW for VS 14, which may or may not work")
		set(GPUPRO_GLFW3_LIBRARY_ARTIFACT "vc15_glfw3.lib")
	endif()
	set(GPUPRO_MESA_LIBRARY_ARTIFACT "opengl32.dll")
	set(GPUPRO_MESA_LIBRARY_IMPLIB "opengl32.lib")
	# Mesa library for software rendering
	# This is a shared library, so for windows platforms we also need to copy the DLL into the output directory
	add_library(mesa SHARED IMPORTED GLOBAL)
	set_target_properties(mesa PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/mesa/msvc/${BINARY_SUBPATH}/${GPUPRO_MESA_LIBRARY_ARTIFACT}")
	set_target_properties(mesa PROPERTIES IMPORTED_IMPLIB "${CMAKE_CURRENT_SOURCE_DIR}/mesa/msvc/${LIBRARY_SUBPATH}/${GPUPRO_MESA_LIBRARY_IMPLIB}")
	target_include_directories(mesa
		INTERFACE
			"${CMAKE_CURRENT_SOURCE_DIR}/mesa/include"
	)
	set(GPUPRO_SHARED_LIB_ARTIFACTS "${CMAKE_CURRENT_SOURCE_DIR}/mesa/msvc/${BINARY_SUBPATH}/${GPUPRO_MESA_LIBRARY_ARTIFACT}" PARENT_SCOPE)
	mark_as_advanced(GPUPRO_SHARED_LIB_ARTIFACTS)
	mark_as_advanced(GPUPRO_MESA_LIBRARY_ARTIFACT)
	mark_as_advanced(GPUPRO_MESA_LIBRARY_IMPLIB)
else()
	message(STATUS "Other")
	set(GPUPRO_GLFW3_LIBRARY_ARTIFACT "libglfw3.a")
endif()
mark_as_advanced(GPUPRO_GLFW3_LIBRARY_ARTIFACT)
# Then add it as imported library (GLOBAL so it is visible in parent scope)
add_library(glfw STATIC IMPORTED GLOBAL)
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_CURRENT_SOURCE_DIR}/glfw/${LIBRARY_SUBPATH}/${GPUPRO_GLFW3_LIBRARY_ARTIFACT}")
target_include_directories(glfw
	INTERFACE
		"${CMAKE_CURRENT_SOURCE_DIR}/glfw/include"
)

