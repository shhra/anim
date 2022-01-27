#ifndef CUBE_MESH_H_
#define CUBE_MESH_H_

#include "../core/shader.hpp"
#include <glad/glad.h> // holds all OpenGL type declarations

namespace anim {

class CubeMesh {
public:
  CubeMesh() { loadVertices(); }

  void Draw(Shader &shader) {
    auto c = glm::vec3(0.8f, 0.7f, 1.f);
    // shader.setVec3("lightColor", c);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
  }

  glm::vec3 bmin() { return bbmin; }
  glm::vec3 bmax() { return bbmax; }

private:
  unsigned int vao;

  float vertices[24] = {
      -0.5f, -0.5f, -0.5f, // Inside left-bottom
      -0.5f, -0.5f, 0.5f,  // Outside left-bottom
      0.5f,  -0.5f, -0.5f, // Inside right-bottom
      0.5f,  -0.5f, 0.5f,  // Outside right-bottom
      -0.5f, 0.5f,  -0.5f, // Inside left-top
      -0.5f, 0.5f,  0.5f,  // Outside left-top
      0.5f,  0.5f,  -0.5f, // Inside right-top
      0.5f,  0.5f,  0.5f   // Outside right-top
  };

  int indices[36] = {
      0, 3, 2, 0, 3, 1, // Lower Face
      4, 7, 6, 4, 7, 5, // Upper Face
      4, 2, 0, 4, 2, 6, // Inside Face
      5, 3, 1, 5, 3, 7, // Outside Face
      4, 1, 5, 4, 1, 0, // Outside Face
      7, 2, 3, 7, 2, 6, // Outside Face
  };

  float normals[36] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                       0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  glm::vec3 bbmin = glm::vec3(-0.5, -0.5, -0.5);
  glm::vec3 bbmax = glm::vec3(0.5, 0.5, 0.5f);

  void loadVertices() {
    // calculateNormals();

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), nullptr,
                 GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals),
                    normals);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float),
                          (void *)sizeof(vertices));

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
  }

  void calculateNormals() {
    for (int i = 0; i < 24; i += 3) {
      glm::vec3 data[3];
      for (int j = 0; j < 3; j++) {
        data[j].x = vertices[indices[i + j] * 3 + 0];
        data[j].y = vertices[indices[i + j] * 3 + 1];
        data[j].z = vertices[indices[i + j] * 3 + 2];
      }
      glm::vec3 ba, ca;
      if (i < 12) {
        ba = data[1] - data[0];
        ca = data[2] - data[0];

      } else {
        ba = data[2] - data[0];
        ca = data[1] - data[0];
      }
      auto cross = 1.f * glm::cross(ba, ca);
      auto result = glm::normalize(cross);

      for (int j = 0; j < 3; j++) {
        normals[indices[i + j] * 3 + 0] += result.x;
        normals[indices[i + j] * 3 + 1] += result.y;
        normals[indices[i + j] * 3 + 2] += result.z;
      }
    }

    for (int i = 0; i < 18; i += 3) {
      auto data = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
      auto result = glm::normalize(data);
      normals[i] = result.x;
      normals[i + 1] = result.y;
      normals[i + 2] = result.z;
    }
  }
};
} // namespace anim

#endif // CUBE_MESH_H_
