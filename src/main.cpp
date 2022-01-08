#include "core/input.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "animation/bvhimporter.hpp"
#include "core/camera.hpp"
#include "core/grid.hpp"
#include "core/models.hpp"
#include "core/renderer.hpp"
#include "core/scene.hpp"
#include "core/shader.hpp"
#include "core/ui.hpp"
#include "core/window.hpp"

int main() {

  core::Window window = core::Window(1280, 720);

  window.initWindow();
  window.createContext();


  Shader shader("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
                "/home/shailesh/Projects/Study/Visualization/src/vis.frag");
  shader.use();

  std::shared_ptr<core::Camera> cam = std::make_shared<core::Camera>(10);
  std::shared_ptr<core::Scene> scene = std::make_shared<core::Scene>();

  core::SceneManager::addgrid(scene, Grid(10));
  std::shared_ptr<core::CameraManager> cam_manager =
      std::make_shared<core::CameraManager>(cam);
  core::Renderer renderer(cam, scene);

  core::Input input(cam_manager, 640.f, 360.0f);
  window.registerCallbacks(&input);

  Ui ui(window.getContext());
  // window.postContext(&scene);
  while (!glfwWindowShouldClose(window.getContext())) {
    // Load UI stuffs
    ui.loadFrame();
    ui.setup();
    window.handleInput(&input);
    window.render(renderer, ui, shader);
    window.pollevents();
  }

  ui.terminate();
  window.terminate();

  return 0;
}
