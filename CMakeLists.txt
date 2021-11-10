cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(visualization)


add_subdirectory(library/glfw)

set(BUILD_SHARED_LIBS FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_NO_EXPORT TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_TESTS FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_INSTALL_PDB FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ZLIB TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_ASSIMP_TOOLS FALSE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_COLLADA_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_FBX_IMPORTER TRUE CACHE BOOL "x" FORCE)
set(ASSIMP_BUILD_BLEND_IMPORTER TRUE CACHE BOOL "x" FORCE)
add_subdirectory(library/assimp)


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
  assimp ${ASSIMP_LIBRARIES})
