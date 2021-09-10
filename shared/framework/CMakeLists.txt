# Framework
file(GLOB_RECURSE GPUPRO_FRAMEWORK_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/*.h")
file(GLOB_RECURSE GPUPRO_FRAMEWORK_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

add_library(GPUProFramework
	"${GPUPRO_FRAMEWORK_HEADERS}"
	"${GPUPRO_FRAMEWORK_SOURCES}"
)
# It is important to still declare the include directory so it transiently gets included by the exercise
target_include_directories(GPUProFramework
	PUBLIC
		"${CMAKE_CURRENT_SOURCE_DIR}/include"
)
# Disable 'fopen' warning
target_compile_definitions(GPUProFramework PUBLIC _CRT_SECURE_NO_WARNINGS)
target_link_libraries(GPUProFramework
	glm glad stbi glfw gli
)
target_compile_features(GPUProFramework PUBLIC cxx_std_11)