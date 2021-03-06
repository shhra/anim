#ifndef RENDERER_H_
#define RENDERER_H_

#include "../animation/joint.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "shader.hpp"
#include "ui.hpp"
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <memory>

namespace core {

//! A renderer is responsible for rendering a virtual scene with a camera.
//! This renderer is extremely simple and uses only single camera system.
struct Renderer {

  Renderer(std::unique_ptr<Camera> &cam, std::unique_ptr<Scene> &scene)
      : cam(cam), scene(scene) {}

  void render(Shader &shader, bool draw_bone = false) {
    glm::mat4 projection =
        glm::perspective(glm::radians(cam->Zoom), 4.f / 3.f, 0.1f, 1000.f);

    glm::mat4 view = cam->GetViewMatrix();

    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    shader.setBool("is_skin", false);

    scene->grid.Draw(shader);

    for (int i = 0; i < scene->is_model_skeleton.size(); i++) {
      if (scene->is_model_skeleton[i]) {
        updateInverseBindTransforms(shader, i);
      }
    }

    for (auto &mesh : scene->meshes) {
      int mesh_id = mesh->id;
      glm::mat4 model_transform =
          scene->mesh_transforms[mesh_id]; // TODO: Use map.
      shader.setMat4("model", model_transform);
      glBindVertexArray(mesh->vao);
      glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT,
                     nullptr);
      glBindVertexArray(0);
    }

    shader.setBool("is_skin", false);
    if (draw_bone) {
      for (auto &world : scene->model_transforms) {
        shader.setMat4("model", world);
        scene->bone_model.Draw(shader);
      }
    }
  }

  void updateInverseBindTransforms(Shader &shader, int skeleton_id) {
    if (scene->skeletons.size() == 0) {
      return;
    }
    auto skeleton = scene->skeletons[skeleton_id];
    for (int idx = 0; idx < skeleton.size; idx++) {
      int joint = skeleton.joint_start + idx;
      auto ac_joint = scene->joints[joint];
      int access_idx = skeleton.transform_start + ac_joint.id;
      auto world_transform = scene->active_world_transform[access_idx].toMat4();
      auto bind_world_transform =
          scene->bind_world_transform[access_idx].toMat4();
      std::string name = "inversebindPose[" + std::to_string(idx) + "]";
      shader.setMat4(name, glm::inverse(bind_world_transform));
      std::string world_name = "worldPose[" + std::to_string(idx) + "]";
      shader.setMat4(world_name, world_transform);
    }
    shader.setBool("is_skin", true);
  }

private:
  std::unique_ptr<Camera> &cam;
  std::unique_ptr<Scene> &scene;
};
} // namespace core

#endif // RENDERER_H_
