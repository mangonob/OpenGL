cmake_minimum_required(VERSION 3.6)
project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework GLUT -framework OpenGL")


set(SOURCE_FILES main.cpp LoadShaders.cpp LoadShaders.h)
add_executable(OpenGL ${SOURCE_FILES})


set(HOMEBREW_BASE /usr/local/Cellar)
set(GLEW_ROOT_DIR ${HOMEBREW_BASE}/glew/2.0.0)
set(GLFW_ROOT_DIR ${HOMEBREW_BASE}/glfw/3.2.1)

set(GLEW_INCLUDE_DIRS ${GLEW_ROOT_DIR}/include)
set(GLFW_INCLUDE_DIRS ${GLFW_ROOT_DIR}/include)

set(GLEW_LIBRARY_DIRS ${GLEW_ROOT_DIR}/lib/libGLEW.a)
set(GLFW_LIBRARY_DIRS ${GLEW_ROOT_DIR}/lib/libglfw.dylib)


link_libraries(${GLEW_LIBRARY_DIRS} ${GLFW_LIBRARY_DIRS})
target_include_directories(${GLEW_INCLUDE_DIRS} ${GLFW_INCLUDE_DIRS})
