cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)
project(visualization)



add_subdirectory(library/glfw)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)
set(TINYGLTF_BUIILD_LOADER_EXAMPLE "" OFF)
add_subdirectory(library/tinygltf)

include_directories(include)

# SET(IMGUI_HEADERS
#   library/imgui/imgui.h
#   library/imgui/imconfig.h
#   library/imgui/imgui_impl_glfw.h
#   library/imgui/imgui_impl_opengl3.h
#   library/imgui/imgui_impl_opengl3_loaders.h
#   library/imgui/imgui_internal.h
#   library/imgui/imstb_rectpack.h
#   library/imgui/imstb_textedit.h
#   library/imgui/imstb_truetype.h)

include_directories(library/imgui)

file(GLOB IMGUI_SOURCES
  library/imgui/*.cpp)

source_group("Header Files" FILES ${IMGUI_HEADERS})

add_executable(visualization
  src/main.cpp
  src/glad.c
  src/tinygltf.cc
  src/core/shader.hpp
  src/core/mesh.hpp
  src/core/grid.hpp
  src/core/models.hpp
  src/core/models.cpp
  src/core/transform.hpp
  src/core/transform.cpp
  src/core/window.hpp
  src/core/input.hpp
  src/core/ui.hpp

  src/core/renderer.hpp
  src/core/scene.hpp

  src/app/scene.hpp
  src/app/demoscene.hpp
  src/app/ikscene.hpp

  src/animation/bone.hpp
  src/animation/bone.cpp
  src/animation/bone_mesh.hpp
  src/animation/bvhimporter.hpp
  src/animation/bvhimporter.cpp
  src/animation/loader.hpp
  src/animation/retargeter.hpp
  src/animation/retargeter.cpp
  ${IMGUI_SOURCES})

target_link_libraries(visualization glfw ${GLFW_LIBRARIES}
  tinygltf ${TINYGLTF_LIBRARIES})

