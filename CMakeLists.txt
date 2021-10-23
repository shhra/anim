cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(visualization)


add_subdirectory(library/glfw)
add_subdirectory(library/assimp)


include_directories(include)


add_executable(visualization
  src/main.cpp
  src/glad.c
  src/core/shader.hpp
  src/core/mesh.hpp
  src/core/grid.hpp
  src/core/models.hpp
)

target_link_libraries(visualization glfw ${GLFW_LIBRARIES}
  assimp ${ASSIMP_LIBRARIES})
