#ifndef JOINT_H_
#define JOINT_H_

#include "../core/scene.hpp"
#include "../core/transform.hpp"
#include "bone.hpp"
#include <glm/gtx/string_cast.hpp>
#include <memory>

namespace anim {

/** Basic functionalities related to scene skeletons.
 *
 * This structure acts as a proxy to access the functions that fills the scene
 * skeleton and updates the skeleton transforms.
 *
 * */
struct SkeletonTransformation {

  /**
   * Given the scene, it fills the scene with a new skeleton. The code is
   * straight-forward without any tricks.
   *
   * Each skeleton is assigned an id that is the position of the skeleton on
   * `scene->skeletons`
   *
   * */
  static void fillSkeletons(std::unique_ptr<core::Scene> &scene,
                            core::Skeleton skeleton, bool is_model = false) {

    int skeleton_id = scene->skeletons.size();

    // Create a new skeleton
    auto anim_skeleton = anim::Skeleton();
    anim_skeleton.id = skeleton_id;
    anim_skeleton.transform_start = scene->active_world_transform.size();
    anim_skeleton.joint_start = scene->joints.size();
    anim_skeleton.size = skeleton.size();

    scene->skeletons.push_back(anim_skeleton);
    scene->is_model_skeleton.push_back(is_model);

    for (auto &joint : skeleton.getJoints()) {
      scene->active_transform.push_back(joint.transform);
      scene->active_world_transform.push_back(joint.worldTransform);

      scene->bind_transform.push_back(joint.bindTransform);
      scene->bind_world_transform.push_back(joint.bindWorldTransform);

      // This assures that we aren't overflowing or underflowing the transforms
      assert(scene->active_world_transform.size() - 1 ==
             scene->skeletons[skeleton_id].transform_start + joint.id);
      auto anim_joint = anim::Joint();
      anim_joint.id = joint.id;
      anim_joint.parent = joint.parent;
      anim_joint.skeleton = skeleton_id;
      anim_joint.name = joint.name;
      scene->joints.push_back(anim_joint);

      scene->inverse_bind_transform.push_back(
          glm::inverse(joint.worldTransform.toMat4()));
    }

    for (int i = 0; i < skeleton.size(); ++i) {
      scene->model_transforms.push_back(glm::mat4(1.0f));
    }
  }

  /**
   * This code is responsible for updating the bone transforms. _Only useful in
   * visualizing bones_
   *
   * However this code provides a pattern that is used throughout the code base.
   *   1. Access the skeleton.
   *   2. Loop through `skeleton.size`. If we want to include root start loop
   *      with 0, else with 1.
   *   3. Using the index and `skeleton.joint_start` start accessing the joints.
   *   4. To access the transform, add `skeleton.transform_start` and
   * `joint.id`.
   * */
  static void updateTransforms(std::unique_ptr<core::Scene> &scene, int id) {
    auto skeleton = scene->skeletons[id];
    int start = skeleton.transform_start;
    for (int idx = 1; idx < skeleton.size; idx++) {
      int joint = idx + skeleton.joint_start;
      auto active_joint = scene->joints[joint];

      auto joint_world_transform =
          scene->active_world_transform[start + active_joint.id];

      auto towards = joint_world_transform.position;

      auto parentP =
          scene->active_world_transform[start + active_joint.parent].position;

      auto zAxis = glm::vec3(0.0, 0.0, 1.0);
      auto lookdir = (towards - parentP);
      auto rotate =
          glm::mat4_cast(scene->active_transform[start + active_joint.parent]
                             .lookAt(lookdir, zAxis)
                             .rotation);
      auto scaleFactor =
          joint_world_transform.scale *
          glm::length(
              scene->active_transform[start + active_joint.id].position);

      auto translate = glm::translate(glm::mat4(1.f), parentP);
      auto scale = glm::scale(glm::mat4(1.f), scaleFactor);

      auto world = translate * rotate * scale;
      auto &joint_model =
          scene->model_transforms[skeleton.joint_start + active_joint.id];
      joint_model = world;
    }
  }

  /**
   * Updates the world transforms. This is required by retargeting function.
   * After successful retargeting, it is necessary to apply the local transforms
   * to the world transform.
   *
   * Notice how the access pattern is same as above.
   * */
  static void updateWorldTransforms(std::unique_ptr<core::Scene> &scene,
                                    int id) {
    auto tar_skeleton = scene->skeletons[id];
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

      joint_world_transform = parent_world_transform * joint_local_transform;
    }
  }

  /**
   * This code gets used in the Inverse kinematics module. It is necessary part
   * for that block */
  static void updateActiveTransforms(std::unique_ptr<core::Scene> &scene,
                                     int id) {
    auto tar_skeleton = scene->skeletons[id];
    core::Transform &joint_local_transform =
        scene->active_transform[tar_skeleton.transform_start];

    const core::Transform joint_world_transform =
        scene->active_world_transform[tar_skeleton.transform_start];
    joint_local_transform = joint_world_transform;

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
      glm::mat4 parent_transform =
          glm::inverse(parent_world_transform.toMat4());
      glm::mat4 world_transform = joint_world_transform.toMat4();
      glm::mat4 local = parent_transform * world_transform;
      joint_local_transform = core::Transform(local);
    }
  }

  static void computeWorldTransforms(std::unique_ptr<core::Scene> &scene,
                                     int id) {
    auto tar_skeleton = scene->skeletons[id];
    for (int idx = 1; idx < tar_skeleton.size; idx++) {
      int joint_id = tar_skeleton.joint_start + idx;
      Joint &active = scene->joints[joint_id];

      core::Transform &joint_world_transform =
          scene->active_world_transform[tar_skeleton.transform_start +
                                        active.id];

      core::Transform parent_world_transform =
          scene->active_world_transform[tar_skeleton.transform_start +
                                        active.parent];

      joint_world_transform = parent_world_transform * joint_world_transform;
    }
  }
};
} // namespace anim

#endif // JOINT_H_
