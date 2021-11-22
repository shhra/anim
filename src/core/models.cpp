// This tiny gltf should be included in only one .cc file.
// In this case the model loading will handle everything here.
// This way, the application will get the gltf model

#include "models.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <tiny_gltf.h>

void Model::loadModel(std::string const &path) {
  tinygltf::TinyGLTF loader;
  bool result = loader.LoadASCIIFromFile(&glTFModel, &err, &warn, path);
  if (result) {
    std::cout << "Model loaded successfully." << std::endl;
  }
  // Once the model is loaded, it consists of different scene.
  // Therefore we can only load the default scene.
  auto &scene = glTFModel.scenes[glTFModel.defaultScene];

  // Each scene will have multiple nodes, there we have to load the nodes for
  // each of these scene.
  for (auto &node : scene.nodes) {
    loadNode(glTFModel.nodes[node], glm::mat4(1.0));
  }
}

void Model::loadNode(tinygltf::Node &node, glm::mat4 transform) {
  // Each gltf file consists of multiple nodes. Nodes can form a hierarchial
  // structure. Therefore these nodes can be traversered recursively.
  glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  glm::vec3 translation = glm::vec3(0.0f);
  glm::vec3 scale = glm::vec3(1.0f);
  if (node.rotation.size() == 4) {
    rotation = glm::make_quat(node.rotation.data());
  }

  if (node.translation.size() == 3) {
    translation = glm::make_vec3(node.translation.data());
  }

  if (node.scale.size() == 3) {
    scale = glm::make_vec3(node.scale.data());
  }

  glm::mat4 localTransform = glm::translate(glm::mat4(1.0f), translation) *
                             glm::mat4(rotation) *
                             glm::scale(glm::mat4(1.0f), scale);
  if (node.matrix.size() == 16) {
    localTransform = glm::make_mat4x4(node.matrix.data());
  }

  auto worldTransform = transform * localTransform;

  if (node.mesh >= 0) {
    loadMesh(glTFModel.meshes[node.mesh], worldTransform);
  }

  for (auto &child : node.children) {
    loadNode(glTFModel.nodes[child], worldTransform);
  }
}

void Model::loadMesh(tinygltf::Mesh &mesh, glm::mat4 transform) {
  glm::mat3 normalTransform =
      glm::transpose(glm::inverse(glm::mat3(transform)));

  for (auto &primitive : mesh.primitives) {

    // TODO Create a mesh over here.
    meshes.push_back(Mesh());

    auto &active_mesh = meshes[meshes.size() - 1];
    if (primitive.indices >= 0) {
      loadIndices(primitive);
    }

    // Extract the vertex data.
    for (auto &attribute : primitive.attributes) {
      auto &accessor = glTFModel.accessors[attribute.second];
      auto &bufferView = glTFModel.bufferViews[accessor.bufferView];
      auto &buffer = glTFModel.buffers[bufferView.buffer];
      int byteStride = accessor.ByteStride(bufferView);

      if (attribute.first == "POSITION") {
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::vec4 pos = glm::vec4(0.0f);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const float *data = (float *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              pos[i] = data[i];
            }
            active_mesh.addVertex(glm::vec3(transform * pos));
          }
        } else {
          std::cout << "NOT IMPLMENTED FOR TYPE!" << std::endl;
        }

      } else if (attribute.first == "NORMAL") {
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::vec3 norm = glm::vec3(0.0f);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const float *data = (float *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              norm[i] = data[i];
            }
            // TODO: Add this normal into the mesh data.
            active_mesh.addNormal(glm::normalize(normalTransform * norm));
          }
        } else {
          std::cout << "NOT IMPLMENTED FOR TYPE!" << std::endl;
        }
      }
    }
  }
}

void Model::loadIndices(tinygltf::Primitive &primitive) {
  auto &index_accessor = glTFModel.accessors[primitive.indices];
  auto &buffer_view = glTFModel.bufferViews[index_accessor.bufferView];
  auto &buffer = glTFModel.buffers[buffer_view.buffer];

  // access the ptr.
  const uint8_t *base =
      &buffer.data.at(buffer_view.byteOffset + index_accessor.byteOffset);

  auto &active_mesh = meshes[meshes.size() - 1];
  // Depending upon the type of data that is being stored, we can convert
  // the pointer into corresponding type.
  switch (index_accessor.componentType) {
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
    const uint32_t *p = (uint32_t *)base;
    for (size_t i = 0; i < index_accessor.count; ++i) {
      active_mesh.addIndex(p[i]);
    }

  }; break;
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
    const uint16_t *p = (uint16_t *)base;
    for (size_t i = 0; i < index_accessor.count; ++i) {
      active_mesh.addIndex(p[i]);
    }
  }; break;
  case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
    for (size_t i = 0; i < index_accessor.count; ++i) {
      active_mesh.addIndex(base[i]);
    }
  }; break;
  }
}
