#include "app/demoscene.hpp"
#include "app/scene.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/input.hpp"
#include "core/ui.hpp"
#include "core/window.hpp"

int main() {
  core::Window window = core::Window(1280, 720);

  window.initWindow();
  window.createContext();

  Shader shader("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
                "/home/shailesh/Projects/Study/Visualization/src/vis.frag");
  shader.use();

  std::unique_ptr<app::SceneCreator> app = std::make_unique<app::DemoScene>();


  core::Input input(app->getCam(), 640.f, 360.0f);
  window.registerCallbacks(&input);


  Ui ui(window.getContext());
  // window.postContext(&scene);
  int i = 0;
  while (!glfwWindowShouldClose(window.getContext())) {
    // Load UI stuffs
    ui.loadFrame();
    ui.setup();
    window.handleInput(&input);
    app->handleUpdates(i);
    window.clear();
    app->render(shader);
    ui.draw();
    window.swap();
    window.pollevents();
    i++;
  }

  ui.terminate();
  window.terminate();

  return 0;
}
