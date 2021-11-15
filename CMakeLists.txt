cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(visualization)



add_subdirectory(library/glfw)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUIILD_LOADER_EXAMPLE "" OFF)
add_subdirectory(library/tinygltf)

include_directories(include)

add_executable(visualization
  src/main.cpp
  src/glad.c
  src/core/shader.hpp
  src/core/mesh.hpp
  src/core/grid.hpp
  src/core/models.hpp
  src/core/transform.hpp
  src/core/transform.cpp
  src/core/window.hpp
  src/core/input.hpp

  src/app/scene.hpp
  
  src/animation/bone.hpp
  src/animation/bone_mesh.hpp
)

target_link_libraries(visualization glfw ${GLFW_LIBRARIES}
  tinygltf ${TINYGLTF_LIBRARIES})
