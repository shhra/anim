#ifndef ENHANCED_H
#define ENHANCED_H
#include "../core/files.hpp"
#include "../core/ui.hpp"
#include "imgui.h"
#include "scene.hpp"

namespace app {
struct Enhanced : public Scene {
  Enhanced()
      : cam(std::move(std::make_unique<core::Camera>(20))),
        scene(std::move(std::make_unique<core::Scene>())) {
    initialize();
  }

  virtual void initialize() {
    emptyScene();
  }

  std::unique_ptr<core::Camera> &getCam() { return this->cam; }

  virtual void handleUpdates(int *i) {
  }

  virtual void render(Shader &shader) { renderer.render(shader, true); }

  virtual void addUI() {
    ImGui::Begin("Files");

    ImGui::End();

  }

private:
  std::unique_ptr<core::Scene> scene = nullptr;
  std::unique_ptr<core::Camera> cam = nullptr;
  core::Renderer renderer = core::Renderer(cam, scene);

  Files assets;

  void emptyScene() {
    core::SceneManager::addgrid(scene, Grid(20, 1));
  }
};
} // namespace app

#endif /* ENHANCED_H */
