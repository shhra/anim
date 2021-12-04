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
  // First load the skeletons from the model.
  loadSkeleton();

  // Once the model is loaded, it consists of different scene.
  // Therefore we can only load the default scene.
  auto &scene = glTFModel.scenes[glTFModel.defaultScene];

  // Each scene will have multiple nodes, there we have to load the nodes for
  // each of these scene.
  for (auto &node : scene.nodes) {
    loadNode(glTFModel.nodes[node], node, glm::mat4(1.0));
  }

  skeleton.setLocalTransform();
  //  skeleton.log();
}

void Model::loadNode(tinygltf::Node &node, int nodeIdx, glm::mat4 transform) {
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

  skeleton.setWorldTransforms(node.name, nodeIdx, worldTransform);

  if (node.mesh >= 0) {
    loadMesh(glTFModel.meshes[node.mesh], node.skin, worldTransform);
  }

  for (auto &child : node.children) {
    loadNode(glTFModel.nodes[child], child, worldTransform);
  }
}

void Model::loadMesh(tinygltf::Mesh &mesh, int skin_id, glm::mat4 transform) {
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
      // Configure accessor.
      auto &accessor = glTFModel.accessors[attribute.second];
      // Use accessor to find the correct bufferView.
      auto &bufferView = glTFModel.bufferViews[accessor.bufferView];
      // Using the correct bufferView, find the correct buffer.
      auto &buffer = glTFModel.buffers[bufferView.buffer];
      // Calculate the byte stride.
      int byteStride = accessor.ByteStride(bufferView);

      // Load the position data into the active mesh.
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
      }

      // Load the normal data into the active mesh.
      if (attribute.first == "NORMAL") {
        std::cout << "Going to implement normal attribute" << std::endl;
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::vec3 norm = glm::vec3(0.0f);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const float *data = (float *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              norm[i] = data[i];
            }
            active_mesh.addNormal(glm::normalize(normalTransform * norm));
          }
        }
      }

      // Load the joint data into the active mesh.
      if (attribute.first == "JOINTS_0") {
        std::cout << "Going to implement joint attribute" << std::endl;
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT) {
          // Now load the joint data into the mesh.
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::ivec4 joint = glm::ivec4(0);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const uint16_t *data = (uint16_t *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              joint[i] = glTFModel.skins[skin_id].joints[data[i]];
            }
            active_mesh.addJoint(joint);
            std::cout << "Joint data: " << glm::to_string(joint) << std::endl;
          }
        } else if (accessor.componentType ==
                   TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE) {
          // Now load this joint data into the mesh.
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::ivec4 joint = glm::ivec4(0);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const uint8_t *data = (uint8_t *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              joint[i] = glTFModel.skins[skin_id].joints[data[i]];
            }
            active_mesh.addJoint(joint);
            std::cout << "Joint data: " << glm::to_string(joint) << std::endl;
          }
        } else {
          std::cout << "NOT IMPLMENTED FOR TYPE!" << std::endl;
        }
      }

      // Load the skinning weights into the active mesh.
      if (attribute.first == "WEIGHTS_0") {
        std::cout << "Going to implement skinning weights" << std::endl;
        // Parse the weight data as a float by checking the component type to be
        // a float. Do not implement for other cases.
        if (accessor.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT) {
          for (size_t v_pos = 0; v_pos < accessor.count; v_pos++) {
            glm::vec4 weight = glm::vec4(0.0f);
            auto *base =
                &buffer.data.at(bufferView.byteOffset + accessor.byteOffset);
            const float *data = (float *)(base + byteStride * v_pos);
            for (size_t i = 0; i < accessorType[accessor.type]; i++) {
              weight[i] = data[i];
            }
            active_mesh.addWeight(weight);
            // std::cout << "Weights data: " << glm::to_string(weight)
            //           << std::endl;
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

void Model::loadSkeleton() {
  // To load a skeleton, first access the skeleton in glTFModel. Once we have
  // the skeleton, we can load the joints.
  for (auto &source_skeleton : glTFModel.skins) {
    // Print the name of the source_skeleton.
    std::cout << "Skeleton name: " << source_skeleton.name << std::endl;

    for (auto &joint : source_skeleton.joints) {
      skeleton.addJoint(glTFModel.nodes[joint].name, joint, -1);
    }

    for (auto &joint : source_skeleton.joints) {
      for (auto child : glTFModel.nodes[joint].children) {
        skeleton.setParent(child, joint);
      }
    }

    // TODO: Load these inverse bind matrices.

    // Check if inverse bind matrices exists for this source_skeleton, use it to
    // creates accessor, bufferView and buffer.
    if (source_skeleton.inverseBindMatrices != -1) {
      auto &inverse_bind_matrices =
          glTFModel.accessors[source_skeleton.inverseBindMatrices];
      auto &buffer_view =
          glTFModel.bufferViews[inverse_bind_matrices.bufferView];
      auto &buffer = glTFModel.buffers[buffer_view.buffer];

      // access the ptr.
      const uint8_t *base = &buffer.data.at(buffer_view.byteOffset +
                                            inverse_bind_matrices.byteOffset);

      // Depending upon the type of data that is being stored, we can convert
      // the pointer into corresponding type.
      switch (inverse_bind_matrices.componentType) {
      case TINYGLTF_COMPONENT_TYPE_FLOAT: {
        const float *p = (float *)base;
        for (size_t i = 0; i < inverse_bind_matrices.count; ++i) {
          glm::mat4 inverse_bind_matrix = glm::mat4(1.0f);
          for (size_t j = 0; j < 16; ++j) {
            std::memcpy(&inverse_bind_matrix, p + i * 16, sizeof(float) * 16);
          }
        }
      }; break;
      default:
        std::cout << "NOT IMPLMENTED FOR TYPE!" << std::endl;
      }
    }
  }
}
