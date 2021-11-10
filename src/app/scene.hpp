#ifndef SCENE_H_
#define SCENE_H_

#include "../core/camera.hpp"
#include "../core/grid.hpp"
#include "../core/input.hpp"
#include "../core/shader.hpp"
#include "glm/glm.hpp"

class Scene {
public:
  Scene() { cam = Camera(10); }

  Scene(const char *vert_shader, const char *frag_shader) {
    cam = Camera(10);
    shader = Shader(vert_shader, frag_shader);
    shader.use();
    grid = Grid(10, 1);
  }

  void render(float screen_width, float screen_height) {
    glm::mat4 projection = glm::perspective(
        glm::radians(cam.Zoom), screen_width / screen_height, 0.1f, 100.0f);
    glm::mat4 view = cam.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);

    grid.Draw(shader);
  }

  void processInputs(float x, float y, bool pan, bool rotate) {
    // This is inefficient but anyway.
    if (rotate) {
      cam.ProcessMouseMovement(x, y);
    }
    if (pan) {
      cam.ProcessPanMovement(x, y);
    }
  }

private:
  Camera cam;
  Shader shader;
  Grid grid;
};
#endif // SCENE_H_
