#include "models.h"

//! Iterates through the meshes to load the mesh attributes.
void Model::loadPrimitives() {
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
        // Handle attribute here.

        // Access vertices data using accessor.

        // Access indices data using index accessor.
      }
    }
  }
}

void Model::loadBuffers() {
  for (std::size_t i = 0; i < tgModel.bufferViews.size(); i++) {
    // Buffer handling goes in here. This is reponsible for binding the
    // gl buffers.

    // To access the corresponding buffer use: "bufferView.buffer"
  }
}
