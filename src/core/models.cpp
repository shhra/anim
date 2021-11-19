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

  localTransform = transform * localTransform;

  if (node.mesh >= 0) {
    loadMesh(glTFModel.meshes[node.mesh], localTransform);
  }

  for(auto &child : node.children) {
    loadNode(glTFModel.nodes[child], localTransform);
  }

}

void Model::loadMesh(tinygltf::Mesh &mesh, glm::mat4 transform) {

  // I am not interested in normal matrix for time being.
  // But the normal matrix is a good have matrix.
  // Therefore it will be in TODO


}
