#ifndef GRID_H_
#define GRID_H_

#include "shader.hpp"
#include "vector"
#include <glad/glad.h> // holds all OpenGL type declarations
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


class Grid {
public:
  std::vector<glm::vec3> vertices;
  std::vector<glm::uvec2> indices;

  unsigned int vao;
  unsigned int length;

  Grid(int grids, int grid_size = 1) {
    int mid = ceil(grids / 2.0f);
    for (int i = -mid; i <= mid; i++) {
        float x = i * grid_size;
        vertices.push_back(glm::vec3(x, 0.f, -mid * grid_size));
        vertices.push_back(glm::vec3(x, 0.f, mid * grid_size));

        int z = i * grid_size;
        vertices.push_back(glm::vec3(-mid * grid_size, 0.f, z));
        vertices.push_back(glm::vec3(mid * grid_size, 0.f, z));
    }

    for (int i = 0; i <= vertices.size(); i+=2) {
      indices.push_back(glm::vec2(i, i + 1));
    }

    initGrid();
  }

  void Draw(Shader &shader) {
    glm::mat4 grid_model = glm::mat4(1.0f);
    // grid_model = glm::rotate(grid_model, 90.0f, glm::vec3(1.0f, 0.0f, 0.0f));
    shader.setMat4("model", grid_model);
    shader.setVec3("objectColor", glm::vec3(4.f));
    // This function now draws the grid.
    glBindVertexArray(vao);

    glLineWidth(2);
    glEnable(GL_LINE_SMOOTH);
    glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, nullptr);
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1);

    glBindVertexArray(0);
  }

private:
  void initGrid() {
    // create buffers
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Render data
    unsigned int vbo, ebo;
    glGenBuffers(1, &vbo); // Create a vertex buffer object
    glGenBuffers(1, &ebo); // Create a element buffer object

    // Bind these buffers
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3),
                 glm::value_ptr(vertices[0]), GL_STATIC_DRAW);

    // Bind buffers
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(glm::vec2),
                 glm::value_ptr(indices[0]), GL_STATIC_DRAW);

    // Enable the attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

    // Finally bind the vertex array
    // TODO: Check the ordering of binding these array buffers
    glBindVertexArray(0);

    length = indices.size() * 2;
  }
};

#endif // GRID_H_
