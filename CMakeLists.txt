cmake_minimum_required(VERSION 3.17)

project(
    3DRenderer
    VERSION 0.1
    LANGUAGES C CXX
    )

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(INCLUDE_ROOT "${PROJECT_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out )

# Setup libraries
include(FetchContent)
FetchContent_Declare(glfw GIT_REPOSITORY https://github.com/glfw/glfw.git GIT_TAG master)
FetchContent_Declare(glm GIT_REPOSITORY https://github.com/g-truc/glm.git GIT_TAG master)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
message("Fetching GLFW")
FetchContent_MakeAvailable(glfw)
message("Fetching GLM")
FetchContent_MakeAvailable(glm)

find_package(OpenGL REQUIRED)

add_subdirectory(src)

# COPY ASSETS FOLDER (USE SAME COMMAND FOR OTHER FOLDERS/DATA)
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR}/out)