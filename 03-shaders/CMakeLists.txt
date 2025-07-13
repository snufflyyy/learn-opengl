cmake_minimum_required(VERSION 3.28)
project(learn-opengl C)

# needed for zed
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 99)

cmake_policy(SET CMP0072 NEW)
find_package(OpenGL REQUIRED)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(external/glfw)

include_directories(
    include

    external/glad/include
    external/glfw/include
)

add_executable(
    ${PROJECT_NAME}
    src/main.c
    src/window.c
    src/file.c
    src/shader.c

    external/glad/src/glad.c
)

target_link_libraries(${PROJECT_NAME} glfw OpenGL::GL)
