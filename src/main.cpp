#include "app/demoscene.hpp"
#include "app/enhanced.hpp"
#include "app/ikscene.hpp"
#include "app/scene.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/files.hpp"
#include "core/input.hpp"
#include "core/ui.hpp"
#include "core/window.hpp"
#include <filesystem>

int main() {
  core::Window window = core::Window(1280, 720);

  window.initWindow();
  window.createContext();

  Shader shader(std::filesystem::path("../src/vis.vert").string().c_str(),
                std::filesystem::path("../src/vis.frag").string().c_str());
  shader.use();

  // std::unique_ptr<app::Scene> app = std::make_unique<app::DemoScene>();
  std::unique_ptr<app::Scene> app = std::make_unique<app::IKScene>();
  // std::unique_ptr<app::Scene> app = std::make_unique<app::SimpleScene>();

  core::Input input(app->getCam(), 640.f, 360.0f);
  window.registerCallbacks(&input);

  Ui::Init(window.getContext());
  // window.postContext(&scene);
  int i = 0;
  while (!glfwWindowShouldClose(window.getContext())) {
    // Load UI stuffs
    Ui::loadFrame();
    Ui::setup();
    window.handleInput(&input);
    app->handleUpdates(&i);
    window.clear();
    app->render(shader);
    app->addUI();
    Ui::draw();
    window.swap();
    window.pollevents();
    i++;
  }

  Ui::terminate();
  window.terminate();
  return 0;
}
