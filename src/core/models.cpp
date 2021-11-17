// This tiny gltf should be included in only one .cc file.
// In this case the model loading will handle everything here.
// This way, the application will get the gltf model
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

#include "models.hpp"


void loadPrimitives(tinygltf::Model &tgModel);
void loadPosition(tinygltf::Model &tgModel);


void Model::loadModel(std::string const &path) {
  tinygltf::Model tgModel;
  tinygltf::TinyGLTF loader;
  bool result = loader.LoadASCIIFromFile(&tgModel, &err, &warn, path);
  if (result) {
    std::cout << "Model loaded successfully." << std::endl;
  }
  loadPrimitives(tgModel);
}

//! Iterates through the meshes to load the mesh attributes.
void loadPrimitives(tinygltf::Model& tgModel) {
  for (std::size_t i = 0; i < tgModel.meshes.size(); i++) {
    auto active_mesh = tgModel.meshes[i];
    std::cout << "Loading Mesh: " << active_mesh.name << std::endl;

    for (const auto &primitive : active_mesh.primitives) {
      // Here handle each primitive.
      // Anything responsible for creating the mesh goes inside here.

      if (primitive.indices < 0)
        break; // Stop parsing this primitive

      // This can be used to bind the mesh.

      auto start = primitive.attributes.begin();
      auto end = primitive.attributes.end();

      for (; start != end; start++) {
        std::cout << "Looping for mesh " << i << std::endl;
        std::cout << "Attribute Info: " << start->first << std::endl;
        // Get the attribute accessor
        auto accessor = tgModel.accessors[start->second];
        auto bufferview = tgModel.bufferViews[accessor.bufferView];
        auto buffer = tgModel.buffers[bufferview.buffer];
        // Check for positions
        if (start->first.compare("POSITION") == 0) {
          // This attribute refers to the position. It will be loaded into the
          // memory.
          std::cout << "Loading the position stuff over here." << std::endl;
          loadPosition(tgModel);
        }
      }
    }
  }
}

void loadPosition(tinygltf::Model &tgModel) {

}
