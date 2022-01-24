#ifndef IKSCENE_H_
#define IKSCENE_H_

#include "../core/ui.hpp"
#include "scene.hpp"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <memory>

namespace app {
struct IKScene : public Scene {
public:
  IKScene()
      : cam(std::move(std::make_unique<core::Camera>(10))),
        scene(std::move(std::make_unique<core::Scene>())) {
    initialize();
  }

  virtual void initialize() { createScene(); }

  virtual void handleUpdates(int *i) {
    anim::SkeletonTransformation::updateTransforms(scene, 0);
  }

  virtual std::unique_ptr<core::Camera> &getCam() { return this->cam; }

  virtual void render(Shader &shader) {
    // Scene is so simple renderer is redundant.
    glm::mat4 projection =
        glm::perspective(glm::radians(cam->Zoom), 4.f / 3.f, 0.1f, 1000.f);

    glm::mat4 view = cam->GetViewMatrix();

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setBool("is_skin", false);
    scene->grid.Draw(shader);

    for (auto &world : scene->model_transforms) {
      shader.setMat4("model", world);
      scene->bone_model.Draw(shader);
    }
  }

  virtual void addUI() {}

private:
  std::unique_ptr<core::Scene> scene = nullptr;
  std::unique_ptr<core::Camera> cam = nullptr;

  void createScene() {
    // Create three transforms.
    core::SceneManager::addgrid(scene, Grid(20, 1));
    createRoot();
  }

  void createRoot() {
    core::Transform joint_one_transform = core::Transform(
        glm::vec3(0.0f),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0, 0.0f)),
        glm::vec3(1.0f));
    scene->active_transform.push_back(joint_one_transform);
    scene->active_world_transform.push_back(joint_one_transform);

    scene->bind_transform.push_back(joint_one_transform);
    scene->bind_world_transform.push_back(joint_one_transform);

    scene->inverse_bind_transform.push_back(
        glm::inverse(joint_one_transform.toMat4()));

    scene->model_transforms.push_back(glm::mat4(1.0f));

    anim::Joint root_joint = anim::Joint();
    root_joint.id = 0;
    root_joint.parent = -1;
    root_joint.skeleton = 0;
    root_joint.name = "root";
    scene->joints.push_back(root_joint);

    // Create skeleton
    anim::Skeleton skeleton = anim::Skeleton();
    skeleton.id = 0;
    skeleton.transform_start = 0;
    skeleton.joint_start = 0;
    skeleton.size = 1;
    scene->skeletons.push_back(skeleton);
  }
};
} // namespace app

#endif // IKSCENE_H_
