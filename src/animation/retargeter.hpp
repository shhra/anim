#ifndef VISUALIZATION_RETARGETER_HPP
#define VISUALIZATION_RETARGETER_HPP

#include "../core/scene.hpp"
#include "bone.hpp"
#include <glm/ext/quaternion_common.hpp>
#include <memory>
#include <string>

namespace anim {
// Animation retargetter is a class that can be used to retarget animation.
struct AnimationRetargetter {
  using MapDatabase = std::vector<std::pair<std::string, std::string>>;
  enum TransformType {
    BIND_TRANSFORM,
    BIND_WORLD_TRANSFORM,
    POSE_TRANSFORM,
    POSE_WORLD_TRANSFORM
  };

  static MapDatabase createMap(std::vector<std::string> &source,
                               std::vector<std::string> &target) {
    MapDatabase db = {};
    if (source.size() != target.size()) {
      std::cerr << "Make sure the source and target are same." << std::endl;
    }

    for (int i = 0; i < source.size(); i++) {
      db.push_back(std::make_pair(source[i], target[i]));
    }
    return db;
  }

  static void retarget(std::unique_ptr<core::Scene> &scene,
                       Skeleton &src_skeleton, Skeleton &tar_skeleton,
                       MapDatabase &data) {

    for (auto &src_tar_pair : data) {
      auto src_joint = findJoint(scene, src_skeleton, src_tar_pair.first);
      auto tar_joint = findJoint(scene, tar_skeleton, src_tar_pair.second);

      rotate(scene, src_skeleton, tar_skeleton, src_joint, tar_joint);
    }

    const core::Transform joint_local_transform =
        scene->active_transform[tar_skeleton.transform_start];

    core::Transform &joint_world_transform =
        scene->active_world_transform[tar_skeleton.transform_start];
    joint_world_transform = joint_local_transform;

    for (int idx = 1; idx < tar_skeleton.size; idx++) {
      int joint_id = tar_skeleton.joint_start + idx;

      Joint &active = scene->joints[joint_id];

      core::Transform &joint_local_transform =
          scene->active_transform[tar_skeleton.transform_start + active.id];

      core::Transform &joint_world_transform =
          scene->active_world_transform[tar_skeleton.transform_start +
                                        active.id];

      core::Transform parent_world_transform =
          scene->active_world_transform[tar_skeleton.transform_start +
                                        active.parent];

      joint_world_transform = joint_local_transform * parent_world_transform;
    }
  }

  static Joint *findJoint(std::unique_ptr<core::Scene> &scene, Skeleton &src,
                          std::string &name) {
    for (int idx = 0; idx < src.size; ++idx) {
      int joint_id = src.joint_start + idx;
      if (scene->joints[joint_id].name == name) {
        return &scene->joints[joint_id];
      }
    }
    return nullptr;
  }

  static void rotate(std::unique_ptr<core::Scene> &scene, Skeleton &src_sk,
                     Skeleton &tar_sk, Joint *src_bone, Joint *tar_bone) {

    // Source parent joint

    // Source parent's rotation
    glm::quat src;
    if (src_bone->parent == -1) {
      src = getRotation(scene, src_sk, src_bone->id, BIND_WORLD_TRANSFORM);
    } else {
      src = getRotation(scene, src_sk, src_bone->parent, BIND_WORLD_TRANSFORM);
    }
    const glm::quat src_a = src;

    glm::quat tar;
    if (tar_bone->parent == -1) {
      tar = getRotation(scene, tar_sk, tar_bone->id, BIND_WORLD_TRANSFORM);
    } else {
      tar = getRotation(scene, tar_sk, tar_bone->parent, BIND_WORLD_TRANSFORM);
    }
    tar = glm::inverse(tar);
    const glm::quat tar_a = tar;

    auto src_bind_world_rotation =
        getRotation(scene, src_sk, src_bone->id, BIND_WORLD_TRANSFORM);
    auto tar_bind_world_rotation =
        getRotation(scene, tar_sk, tar_bone->id, BIND_WORLD_TRANSFORM);

    auto tar_src_diff =
        glm::conjugate(src_bind_world_rotation) * tar_bind_world_rotation;

    auto src_world_rotation =
        src_a * getRotation(scene, src_sk, src_bone->id, POSE_TRANSFORM);

    if (glm::dot(src_world_rotation, src_bind_world_rotation) < 0) {
      src_world_rotation = src_world_rotation * (-tar_src_diff);
    } else {
      src_world_rotation = src_world_rotation * tar_src_diff;
    }

    core::Transform &target_local_rotation =
        scene->active_transform[tar_sk.transform_start + tar_bone->id];
    target_local_rotation.rotation = tar_a * src_world_rotation;
  };

  static glm::quat getRotation(std::unique_ptr<core::Scene> &scene,
                               Skeleton &sk, int bone_id, TransformType t) {
    if (t == POSE_TRANSFORM)
      return scene->active_transform[sk.transform_start + bone_id].rotation;
    else if (t == BIND_WORLD_TRANSFORM)
      return scene->bind_world_transform[sk.transform_start + bone_id].rotation;
    return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
  }

  static glm::vec3 handle_zeros(glm::vec3 &v) {
    auto vec = v;
    if (std::abs(vec[0]) <= 1e-2)
      vec[0] = 0.0f;
    if (std::abs(vec[1]) <= 1e-2)
      vec[1] = 0.0f;
    if (std::abs(vec[2]) <= 1e-2)
      vec[2] = 0.0f;
    return vec;
  };

  static glm::vec3 div(glm::vec3 &a, glm::vec3 &b) {
    auto data = a;
    data[0] = (b[0] != 0.0f) ? a.x / b.x : 0.0f;
    data[1] = (b[1] != 0.0f) ? a.y / b.y : 0.0f;
    data[2] = (b[2] != 0.0f) ? a.z / b.z : 0.0f;
    return data;
  };
};

} // namespace anim

#endif // VISUALIZATION_RETARGETER_HPP
