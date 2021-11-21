#ifndef MESH_H_
#define MESH_H_

#include <glad/glad.h> // holds all OpenGL type declarations

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "shader.hpp"

#include <string>
#include <vector>
using namespace std;

#define MAX_BONE_INFLUENCE 4

class Mesh {
public:
  // mesh Data
  vector<glm::vec3> positions;
  vector<glm::vec3> normals;
  vector<unsigned int> indices;
  unsigned int VAO;

  Mesh() {}

  void addVertex(glm::vec3 vertex) { positions.push_back(vertex); }
  void addNormal(glm::vec3 normal) { normals.push_back(normal); }
  void addIndex(unsigned int index) { indices.push_back(index); }

  // render the mesh
  void Draw(Shader &shader) {
    glm::mat4 model = glm::mat4(1.0f);
    shader.setMat4("model", model);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0); // u
  }

  void load() { setupMesh(); }

private:
  // render data
  unsigned int VBO, EBO;

  // initializes all the buffer objects/arrays
  void setupMesh() {
    // create buffers/arrays
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for
    // all its items. The effect is that we can simply pass a pointer to the
    // struct and it translates perfectly to a glm::vec3/2 array which again
    // translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions),
                 &positions[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)0);

    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(normals), &normals[0],
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          (void *)0);

    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(positions),
                 &positions[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
                 &indices[0], GL_STATIC_DRAW);

    glBindVertexArray(0);
  }
};

#endif // MESH_H_
