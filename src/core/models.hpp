#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "mesh.hpp"
#include "shader.hpp"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "../src/animation/bone.hpp"

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

//! This class handles reading of the motion file.
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
  tinygltf::Model tgModel;
  tinygltf::TinyGLTF loader;
  std::string err, warn;
  void loadModel(std::string const &path) {
    bool result = loader.LoadASCIIFromFile(&tgModel, &err, &warn, path);
    if (result) {
      std::cout << "Model loaded successfully." << std::endl;
    }
  }

  void loadPrimitives();
  void loadBuffers();
  void loadJoints();
  void loadSkinningWeights();
};

#endif
