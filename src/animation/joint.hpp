#ifndef JOINT_H_
#define JOINT_H_

#include "../core/scene.hpp"
#include "../core/transform.hpp"
#include "bone.hpp"
#include <glm/gtx/string_cast.hpp>
#include <memory>

namespace anim {

struct SkeletonTransformation {
  static void fillSkeletons(std::unique_ptr<core::Scene> &scene,
                            core::Skeleton skeleton) {

    int skeleton_id = scene->skeletons.size();

    // Create a new skeleton
    auto anim_skeleton = std::make_unique<anim::Skeleton>();
    anim_skeleton->id = skeleton_id;
    anim_skeleton->transform_start = scene->active_transform.size();
    anim_skeleton->joint_start = scene->joints.size();
    anim_skeleton->size = skeleton.size();

    scene->skeletons.push_back(std::move(anim_skeleton));

    for (auto &joint : skeleton.getJoint()) {
      scene->active_transform.push_back(joint.transform);
      scene->active_world_transform.push_back(joint.worldTransform);

      scene->bind_transform.push_back(joint.bindTransform);
      scene->bind_world_transform.push_back(joint.bindWorldTransform);

      assert(scene->active_transform.size() - 1 ==
             scene->skeletons[skeleton_id]->transform_start + joint.id);
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

  static void updateTransforms(std::unique_ptr<core::Scene> &scene, int id) {
    auto skeleton = scene->skeletons[id].get();
    int start = skeleton->transform_start;
    for (int idx = 1; idx < skeleton->size; idx++) {
      int joint = idx + skeleton->joint_start;
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
          scene->model_transforms[skeleton->joint_start + active_joint.id];
      joint_model = world;
    }
  }
};

} // namespace anim

#endif // JOINT_H_
