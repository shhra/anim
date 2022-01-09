#include "animation/bvhimporter.hpp"
#include "animation/joint.hpp"
#include "core/input.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <memory>
#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/camera.hpp"
#include "core/grid.hpp"
#include "core/models.hpp"
#include "core/renderer.hpp"
#include "core/scene.hpp"
#include "core/shader.hpp"
#include "core/ui.hpp"
#include "core/window.hpp"
#include "core/mesh.hpp"
#include "animation/loader.hpp"

int main() {

  core::Window window = core::Window(1280, 720);

  window.initWindow();
  window.createContext();

  Shader shader("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
                "/home/shailesh/Projects/Study/Visualization/src/vis.frag");
  shader.use();

  std::shared_ptr<core::Camera> cam = std::make_shared<core::Camera>(20);
  std::shared_ptr<core::Scene> scene = std::make_shared<core::Scene>();

  core::Model model(
      "/home/shailesh/Projects/Study/Visualization/assets/anim.gltf");

  model.load();
  model.skeleton.bindTransforms(); // << this will be removed.


  anim::BVHImporter bvh =
      anim::BVHImporter("/home/shailesh/Projects/Study/PFNN/pfnn/data/animations/"
                  "LocomotionFlat02_000.bvh");

  core::SceneManager::addgrid(scene, Grid(20, 1));

  anim::AnimDatabase db = anim::AnimDatabase();

  auto animation = std::move(anim::AnimationLoader::loadMotionData(scene, bvh, db));
  // for (auto &mesh: model.meshes) {
  //   std::unique_ptr<core::Mesh> m = std::make_unique<core::Mesh>(mesh);
  //   core::SceneManager::addMesh(scene, m);
  // }

  std::shared_ptr<core::CameraManager> cam_manager =
      std::make_shared<core::CameraManager>(cam);
  core::Renderer renderer(cam, scene);

  core::Input input(cam_manager, 640.f, 360.0f);
  window.registerCallbacks(&input);

  Ui ui(window.getContext());
  // window.postContext(&scene);
  int i = 0;
  while (!glfwWindowShouldClose(window.getContext())) {
    // Load UI stuffs
    if (i >= animation->frames.size()) {
      i = 0;
    }
    ui.loadFrame();
    ui.setup();
    anim::AnimationLoader::setFrame(scene, animation, db, i);
    window.handleInput(&input);
    window.render(renderer, ui, shader);
    window.pollevents();
    i++;
  }

  ui.terminate();
  window.terminate();

  return 0;
}
