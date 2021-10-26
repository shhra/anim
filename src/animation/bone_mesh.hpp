#ifndef BONE_MESH_H_
#define BONE_MESH_H_

#include "../core/shader.hpp"
#include <glad/glad.h> // holds all OpenGL type declarations

class BoneMesh {
public:
  BoneMesh() { loadVertices(); }

  void Draw(Shader &shader) {
    // glm::mat4 bone_model = glm::mat4(1.0f);
    // shader.setMat4("model", bone_model);
    shader.setVec3("objectColor", glm::vec3(0.8f, 0.7f, 1.f));

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
      0.f,  0.f,  0.f,   0.f,   0.f,  1.f,  hori, hori, vert,
      -hori, hori, vert, -hori,  -hori, vert, hori,  -hori, vert,
  };

  int indices[24] = {
      0, 5, 4, 0, 4, 3, 0, 3, 2, 0, 2, 5, 1, 5, 4, 1, 4, 3, 1, 3, 2, 1, 2, 5,
  };

  void loadVertices() {
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
                 GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glBindVertexArray(0);
  }
};

#endif // BONE_MESH_H_
