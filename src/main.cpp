#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "animation/bvhimporter.hpp"
#include "core/camera.hpp"
#include "core/grid.hpp"
#include "core/models.hpp"
#include "core/shader.hpp"
#include "core/ui.hpp"
#include "core/window.hpp"

#include "app/scene.hpp"

int main() {

  Window window = Window(1280, 720);

  window.initWindow();
  window.createContext();
  Scene scene("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
              "/home/shailesh/Projects/Study/Visualization/src/vis.frag");

  Ui ui(window.getContext());
  window.postContext(&scene);
  while (!glfwWindowShouldClose(window.getContext())) {
    // Load UI stuffs
    ui.loadFrame();
    ui.setup();
    window.handleInput();
    window.render(scene, ui);
    window.pollevents(scene);
  }

  ui.terminate();
  window.terminate();

  return 0;
}
