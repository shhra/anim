#ifndef MESH_H_
#define MESH_H_

#include <algorithm>
#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>

namespace core {

class Mesh {
public:
  int id;
  // mesh Data
  std::vector<glm::vec3> positions;
  std::vector<glm::vec3> normals;
  std::vector<unsigned int> indices;
  // Create a list of skinning indices.
  std::vector<glm::ivec4> joint_indices;
  // Create a list of skinning weights.
  std::vector<glm::vec4> joint_weights;

  // Bounding box min.
  glm::vec3 bb_min = {};

  // Bounding box pax.
  glm::vec3 bb_max = {};
  unsigned int vao;

  Mesh() {}
  void addVertex(glm::vec3 vertex) { positions.push_back(vertex); }
  void addNormal(glm::vec3 normal) { normals.push_back(normal); }
  void addIndex(unsigned int index) { indices.push_back(index); }
  void addJoint(glm::ivec4 joint_index) {
    joint_indices.push_back(joint_index);
  }
  void addWeight(glm::vec4 weight) { joint_weights.push_back(weight); }
  void Draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // u
  }

  void load() { setupMesh(); }

private:
  // render data
  unsigned int VBO, EBO;

  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &VBO);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER,
                 positions.size() * sizeof(glm::vec3) +
                     normals.size() * sizeof(glm::vec3) +
                     joint_indices.size() * sizeof(glm::ivec4) +
                     joint_weights.size() * sizeof(glm::vec4),
                 nullptr, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, positions.size() * sizeof(glm::vec3),
                    &positions[0]);
    glBufferSubData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3),
                    normals.size() * sizeof(glm::vec3), &normals[0]);

    glBufferSubData(GL_ARRAY_BUFFER,
                    (positions.size() + normals.size()) * sizeof(glm::vec3),
                    joint_indices.size() * sizeof(glm::ivec4),
                    &joint_indices[0]);

    glBufferSubData(GL_ARRAY_BUFFER,
                    (positions.size() + normals.size()) * sizeof(glm::vec3) +
                        joint_indices.size() * sizeof(glm::ivec4),
                    joint_weights.size() * sizeof(glm::vec4),
                    &joint_weights[0]);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)(sizeof(glm::vec3) * positions.size()));

    glVertexAttribIPointer(
        2, 4, GL_INT, sizeof(glm::ivec4),
        (void *)(sizeof(glm::vec3) * (positions.size() + normals.size())));

    glVertexAttribPointer(
        3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4),
        (void *)(sizeof(glm::vec3) * (positions.size() + normals.size()) +
                 sizeof(glm::ivec4) * joint_indices.size()));

    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glBindVertexArray(0);
  }
};

} // namespace core
#endif // MESH_H_
