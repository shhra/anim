#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../animation/joint.hpp"
#include "../animation/bone.hpp"
#include "mesh.hpp"
#include "scene.hpp"
#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <tiny_gltf.h>
#include <vector>

namespace core {

static std::map<int, GLenum> drawMode =
    std::map<int, GLenum>{{TINYGLTF_MODE_POINTS, GL_POINTS},
                          {TINYGLTF_MODE_LINE, GL_LINE},
                          {TINYGLTF_MODE_LINE_LOOP, GL_LINE_LOOP},
                          {TINYGLTF_MODE_LINE_STRIP, GL_LINE_STRIP},
                          {TINYGLTF_MODE_TRIANGLES, GL_TRIANGLES},
                          {TINYGLTF_MODE_TRIANGLE_STRIP, GL_TRIANGLE_STRIP},
                          {TINYGLTF_MODE_TRIANGLE_FAN, GL_TRIANGLE_FAN}};

static std::map<int, int> accessorType = std::map<int, int>{
    {TINYGLTF_TYPE_SCALAR, 1},
    {TINYGLTF_TYPE_VEC2, 2},
    {TINYGLTF_TYPE_VEC3, 3},
    {TINYGLTF_TYPE_VEC4, 4},
};

//! This class handles reading of the model file using gltf.
class Model {
public:
  // Set the model data.
  std::vector<Mesh> meshes;
  Skeleton skeleton;
  bool applyGamma;
  Model(std::string const &path, bool gamma = false) : applyGamma(gamma) {
    // load my mesh.
    // Find all the relevant meshes and load it.
    loadModel(path);
  }

  void load() {
    for (auto &mesh : meshes) {
      mesh.load();
    }
  }

  void Draw(Shader &shader) {
    for (auto &mesh : meshes) {
      mesh.Draw(shader);
    }
  }

private:
  // This is a horrible way because data is being copied twice.
  tinygltf::Model glTFModel;
  std::string err, warn;
  void loadModel(std::string const &path);
  void loadIndices(tinygltf::Primitive &primitive);
  void loadNode(tinygltf::Node &nodes, int nodeIdx, glm::mat4 transform);
  void loadMesh(tinygltf::Mesh &mesh, int skin_id, glm::mat4 transform);
  void loadSkeleton();

  std::vector<int> joint_order = {};
  std::size_t index(int joint) {
    for (size_t i = 0; i < joint_order.size(); i++) {
      if (joint_order[i] == joint) {
        return i;
      }
    }
    return (std::size_t)-1;
  };
};
} // namespace core
#endif
