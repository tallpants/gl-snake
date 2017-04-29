cmake_minimum_required(2.8.0)
project(gl_snake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./bin")

add_executable(snake src/snake.cpp)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS} ${GLUT_INCLUDE_DIRS})

target_link_libraries(snake ${OPENGL_LIBRARIES} ${GLUT_LIBRARY})