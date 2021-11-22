#ifndef BONE_MESH_H_
#define BONE_MESH_H_

#include "../core/shader.hpp"
#include <glad/glad.h> // holds all OpenGL type declarations

class BoneMesh {
public:
  BoneMesh() { loadVertices(); }

  void Draw(Shader &shader) {
    auto c = glm::vec3(0.8f, 0.7f, 1.f);
    // shader.setVec3("lightColor", c);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
  };

private:
  float vert = 0.2f;
  float hori = 0.1f;
  unsigned int vao;

  // 0 - bottom, 1 is top
  //   -- | +-
  //   -------
  //   -+ | ++
  // float vertices[18] = {
  //     0.f,  0.f,  0.f,   0.f,   1.f,  0.f,  -hori, vert, -hori,
  //     hori, vert, -hori, -hori, vert, hori, hori,  vert, hori,
  // };

  // 0 - bottom, 1 is top
  //   -+ | ++
  //   -------
  //   -- | +-
  float vertices[18] = {
      0.f,   0.f,  0.f,  0.f,   0.f,   1.f,  hori, hori,  vert,
      -hori, hori, vert, -hori, -hori, vert, hori, -hori, vert,
  };

  int indices[24] = {
      0, 5, 4, 0, 4, 3, 0, 3, 2, 0, 2, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 1, 2, 5,
  };

  float normals[18] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  void loadVertices() {
    calculateNormals();

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

    for(int i = 0; i < 18; i +=3) {
      auto data = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
      auto result = glm::normalize(data);
      normals[i] = result.x;
      normals[i + 1] = result.y;
      normals[i + 2] = result.z;

    }
  }
};

#endif // BONE_MESH_H_
