#ifndef IKSCENE_H_
#define IKSCENE_H_

#include "../animation/cube_mesh.hpp"
#include "../animation/inverse_kinematics.hpp"
#include "../core/ui.hpp"
#include "imgui.h"
#include "scene.hpp"
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <memory>

namespace app {
struct IKScene : public Scene {
public:
  IKScene()
      : cam(std::move(std::make_unique<core::Camera>(12))),
        scene(std::move(std::make_unique<core::Scene>())) {
    initialize();
  }

  virtual void initialize() {
    createScene();
    anim::SkeletonTransformation::updateTransforms(scene, 0);
  }

  virtual void handleUpdates(int *i) {

    // apply IK
    auto targetTransform = core::Transform(
        cube_position,
        glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0)),
        glm::vec3(1.0));

    auto ik = anim::InverseKinematics::applyIK(scene, scene->skeletons[0],
                                               targetTransform);

    if (ik) {
      anim::SkeletonTransformation::updateTransforms(scene, 0);
    }
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

    glm::mat4 cube_point = glm::mat4(1.0f);
    cube_point = glm::translate(cube_point, cube_position);
    cube_point = glm::scale(cube_point, glm::vec3(0.25));
    shader.setMat4("model", cube_point);
    cube_model.Draw(shader);
  }

  virtual void addUI() {
    for (auto &skeleton : scene->skeletons) {
      Ui::addSkeletonTree(skeleton, scene->joints, &selected_transform);
    }
    if (selected_transform > -1) {
      // Create a popup window for the transform values.
      if (is_first_copy == true) {
        copy = scene->active_transform[selected_transform];
        is_first_copy = false;
      }
      ImGui::OpenPopup("Transform");
    }
    if (ImGui::BeginPopup("Transform")) {
      auto transform = &scene->active_transform[selected_transform];
      // Save transform values..
      ImGui::SliderFloat3("Position", &transform->position[0], -10.0f, 10.0f);
      ImGui::SliderFloat4("Rotation", &transform->rotation[0], -1.0f, 1.0f);
      // Create a reset button that sets transform to the copied values.
      if (ImGui::Button("Reset")) {
        *transform = copy;
      }
      ImGui::SameLine();
      if (ImGui::Button("Close")) {
        ImGui::CloseCurrentPopup();
        selected_transform = -1;
      }
      ImGui::EndPopup();
    }

    std::string cube = "Cube position";
    if (ImGui::CollapsingHeader(cube.c_str())) {
      // Create a slider for the cube position
      ImGui::SliderFloat3("Position", &cube_position[0], -5.0f, 5.0f);
    }
  }

private:
  std::unique_ptr<core::Scene> scene = nullptr;
  std::unique_ptr<core::Camera> cam = nullptr;
  anim::CubeMesh cube_model = anim::CubeMesh();

  void createScene() {
    // Create three transforms.
    core::SceneManager::addgrid(scene, Grid(20, 1));
    createRoot();
    addJoint1();
    addJoint2();
    addJoint3();
  }

  int selected_transform = -1;
  core::Transform copy;
  bool is_first_copy = true;
  glm::vec3 cube_position = glm::vec3(0.0f, 4.375f, 0.0f);

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

  void addJoint1() {
    // Local transform
    core::Transform joint_transform = core::Transform(
        glm::vec3(0.0, 1.0, 0.0f),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::vec3(1.0f));
    scene->active_transform.push_back(joint_transform);

    auto joint_world_transform =
        joint_transform * scene->active_world_transform[0];

    scene->active_world_transform.push_back(joint_world_transform);

    scene->bind_transform.push_back(joint_transform);
    scene->bind_world_transform.push_back(joint_world_transform);

    scene->inverse_bind_transform.push_back(
        glm::inverse(joint_world_transform.toMat4()));

    scene->model_transforms.push_back(glm::mat4(1.0f));

    anim::Joint joint = anim::Joint();
    joint.id = 1;
    joint.parent = 0;
    joint.skeleton = 0;
    joint.name = "joint_1";
    scene->joints.push_back(joint);

    auto skeleton = &scene->skeletons[0];
    skeleton->size = 2;
  }

  void addJoint2() {
    // Local transform
    core::Transform joint_transform = core::Transform(
        glm::vec3(0.0, 2.0, 0.0f),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::vec3(1.0f));
    scene->active_transform.push_back(joint_transform);

    auto joint_world_transform =
        joint_transform * scene->active_world_transform[1];

    scene->active_world_transform.push_back(joint_world_transform);

    scene->bind_transform.push_back(joint_transform);
    scene->bind_world_transform.push_back(joint_world_transform);

    scene->inverse_bind_transform.push_back(
        glm::inverse(joint_world_transform.toMat4()));

    scene->model_transforms.push_back(glm::mat4(1.0f));

    anim::Joint joint = anim::Joint();
    joint.id = 2;
    joint.parent = 1;
    joint.skeleton = 0;
    joint.name = "joint_2";
    scene->joints.push_back(joint);

    auto skeleton = &scene->skeletons[0];
    skeleton->size = 3;
  }

  void addJoint3() {
    // Local transform
    core::Transform joint_transform = core::Transform(
        glm::vec3(0.0, 1.0, 0.0f),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)),
        glm::vec3(1.0f));
    scene->active_transform.push_back(joint_transform);

    auto joint_world_transform =
        joint_transform * scene->active_world_transform[2];

    scene->active_world_transform.push_back(joint_world_transform);

    scene->bind_transform.push_back(joint_transform);
    scene->bind_world_transform.push_back(joint_world_transform);

    scene->inverse_bind_transform.push_back(
        glm::inverse(joint_world_transform.toMat4()));

    scene->model_transforms.push_back(glm::mat4(1.0f));

    anim::Joint joint = anim::Joint();
    joint.id = 3;
    joint.parent = 2;
    joint.skeleton = 0;
    joint.name = "joint_3";
    scene->joints.push_back(joint);

    auto skeleton = &scene->skeletons[0];
    skeleton->size = 4;
  }
};
} // namespace app

#endif // IKSCENE_H_
