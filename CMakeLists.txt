cmake_minimum_required(VERSION 2.8)

project( Cube )

ADD_DEFINITIONS(
	-std=c++0x
	-Wall
)

# add my custom headers directory
include_directories ("${PROJECT_SOURCE_DIR}")

# finds OpenGL, GLU and X11
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)

set(GL_LIBRARY glfw3 GL GLU X11 Xxf86vm pthread Xrandr Xi  GLEW)
set(SOURCES
	engine.hpp
	engine_shaders.hpp)
add_executable(Cube main.cpp ${SOURCES})


target_link_libraries(Cube ${GL_LIBRARY} m)
