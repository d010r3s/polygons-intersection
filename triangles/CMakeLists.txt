cmake_minimum_required(VERSION 3.16)
project(untitled5)
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

add_executable(untitled5 main.cpp)

target_link_libraries(untitled5 ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} -lopengl32 -lfreeglut)
