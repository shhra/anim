#ifndef ANIMATIONLOADER_H_
#define ANIMATIONLOADER_H_

#include "bvhimporter.hpp"
#include "joint.hpp"
#include <memory>

namespace anim {

struct AnimDatabase {
  AnimDatabase() : animation_db({}) {}
  int *operator[](Animation *data) { return &animation_db[data]; }
  std::unordered_map<Animation *, int> animation_db;
};

struct AnimationLoader {

  static std::unique_ptr<Animation>
  loadMotionData(std::unique_ptr<core::Scene> &scene, BVHImporter &data,
                 AnimDatabase &db) {
    int start = scene->active_transform.size();
    int end = start + data.animation.end;
    auto animation = std::make_unique<Animation>();

    animation->start = start;
    animation->end = end;
    animation->frame_time = data.animation.frame_time;
    animation->frames = std::move(data.animation.frames);
    SkeletonTransformation::fillSkeletons(scene, data.skeleton, false);

    scene->active_transform.insert(scene->active_transform.begin() + start,
                                   data.transforms.begin(),
                                   data.transforms.end());

    if (db[animation.get()])
      *(db[animation.get()]) = scene->skeletons.size() - 1;

    return animation;
  }

  static void initialize(std::unique_ptr<core::Scene> &scene,
                         std::unique_ptr<Animation> &anim, AnimDatabase &db) {

    auto skeleton = scene->skeletons[*db[anim.get()]];
    auto frame = anim->frames[0];

    int frame_idx = anim->start + frame.start;
    int transform_idx = skeleton.transform_start;

    core::Transform &root_transform = scene->active_transform[transform_idx];
    root_transform = scene->active_transform[frame_idx];
    core::Transform &root_world = scene->active_world_transform[transform_idx];
    root_world = scene->active_transform[transform_idx];

    frame_idx++;
    for (int idx = 1; idx < skeleton.size; idx++) {

      int joint = skeleton.joint_start + idx;
      auto ac_joint = scene->joints[joint];

      core::Transform &joint_transform =
          scene->active_transform[transform_idx + ac_joint.id];
      joint_transform = scene->active_transform[frame_idx++];

      core::Transform &bind_transform =
          scene->bind_transform[transform_idx + ac_joint.id];
      bind_transform = scene->active_transform[transform_idx + ac_joint.id];

      core::Transform &active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];

      active_world_transform =
          scene->active_transform[transform_idx + ac_joint.id] *
          scene->active_world_transform[transform_idx + ac_joint.parent];

      core::Transform &bind_world_transform =
          scene->bind_world_transform[transform_idx + ac_joint.id];

      bind_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];
    }
  }

  static void setFrame(std::unique_ptr<core::Scene> &scene,
                       std::unique_ptr<Animation> &anim, AnimDatabase &db,
                       int frame_id) {

    auto skeleton = scene->skeletons[*db[anim.get()]];
    auto frame = anim->frames[frame_id];

    int frame_idx = anim->start + frame.start;
    int transform_idx = skeleton.transform_start;

    core::Transform &root_transform = scene->active_transform[transform_idx];
    root_transform = scene->active_transform[frame_idx];
    core::Transform &root_world = scene->active_world_transform[transform_idx];
    root_world = scene->active_transform[transform_idx];

    frame_idx++;
    for (int idx = 1; idx < skeleton.size; idx++) {

      int joint = skeleton.joint_start + idx;
      auto ac_joint = scene->joints[joint];

      core::Transform &joint_transform =
          scene->active_transform[transform_idx + ac_joint.id];
      joint_transform = scene->active_transform[frame_idx++];

      core::Transform &bind_transform =
          scene->bind_transform[transform_idx + ac_joint.id];
      bind_transform = scene->active_transform[transform_idx + ac_joint.id];

      core::Transform &active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];

      active_world_transform =
          scene->active_transform[transform_idx + ac_joint.id] *
          scene->active_world_transform[transform_idx + ac_joint.parent];
    }
  }
};
} // namespace anim

#endif // ANIMATIONLOADER_H_
