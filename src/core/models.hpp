#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../src/animation/bone.hpp"
#include "mesh.hpp"
#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

//! This class handles reading of the model file using gltf.
class Model {
public:
  // Set the model data.
  vector<Texture> textures;
  vector<Mesh> meshes;
  vector<Skeleton> skeletons;
  bool applyGamma;
  Model(std::string const &path, bool gamma = false) : applyGamma(gamma) {
    // load my mesh.
    // Find all the relevant meshes and load it.
    loadModel(path);
  }

  void Draw(Shader &shader) {
    for (auto &mesh : meshes) {
      mesh.Draw(shader);
    }
  }

private:
  std::string err, warn;
  void loadModel(std::string const &path);
};

#endif
