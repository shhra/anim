#ifndef SCENE_HPP_
#define SCENE_HPP_

#include "../animation/bone_mesh.hpp"
#include "grid.hpp"
#include "mesh.hpp"
#include "transform.hpp"
#include <glm/fwd.hpp>
#include <memory>

namespace anim {
struct Joint {
  int id;
  int parent;
  int skeleton;
  std::string name;
};

struct Skeleton {
  int id;
  int transform_start;
  int joint_start;
  int size;
};
}; // namespace anim

namespace core {

struct Scene {
  Grid grid;
  anim::BoneMesh bone_model = anim::BoneMesh();

  std::vector<std::unique_ptr<Mesh>> meshes;
  std::vector<glm::mat4> mesh_transforms = {};

  std::vector<anim::Skeleton> skeletons = {};

  //! Active pose.
  std::vector<Transform> active_transform = {};
  std::vector<Transform> active_world_transform = {};

  //! Bind Pose.
  std::vector<Transform> bind_transform = {};
  std::vector<Transform> bind_world_transform = {};

  //! Inverse bind pose
  std::vector<glm::mat4> inverse_bind_transform = {};

  //! joints.
  std::vector<anim::Joint> joints = {};

  //! bone model transforms
  std::vector<glm::mat4> model_transforms = {};
};

struct SceneManager {

  static void addgrid(std::unique_ptr<Scene> &scene, Grid grid) {
    scene->grid = grid;
  }

  static void addMesh(std::unique_ptr<Scene> &scene, std::unique_ptr<Mesh> &mesh) {
    int id = mesh->id;
    scene->meshes.push_back(std::move(mesh));
    scene->mesh_transforms.push_back(glm::mat4(1.0f));
  }
};
} // namespace core
#endif // SCENE_H_
