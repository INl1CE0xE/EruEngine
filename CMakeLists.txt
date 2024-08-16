cmake_minimum_required(VERSION 3.4...3.28 FATAL_ERROR)

set(PROJECT_NAME EruEngine)

project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
src/main.cpp
src/Renderer/shader.h
src/Renderer/texture.h
src/Renderer/external/stb_image.h
src/Renderer/external/stb_image.cpp
src/external/glm/glm.hpp
src/external/glm/gtc/matrix_transform.hpp
src/external/glm/gtc/type_ptr.hpp
)


set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} glad)
