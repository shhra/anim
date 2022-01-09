#ifndef ANIMATIONLOADER_H_
#define ANIMATIONLOADER_H_

#include "bvhimporter.hpp"
#include "joint.hpp"
#include <memory>

namespace anim {

struct AnimDatabase {
  std::unordered_map<Animation *, int> animation_db;

  AnimDatabase() : animation_db({}) {}

  int *operator[](Animation *data) { return &animation_db[data]; }
};

struct AnimationLoader {

  static std::unique_ptr<Animation>
  loadMotionData(std::shared_ptr<core::Scene> &scene, BVHImporter &data,
                 AnimDatabase &db) {
    int start = scene->active_transform.size();
    int end = start + data.animation.end;
    auto animation = std::make_unique<Animation>();

    animation->start = start;
    animation->end = end;
    animation->frame_time = data.animation.frame_time;
    animation->frames = std::move(data.animation.frames);
    SkeletonTransformation::fillSkeletons(scene, data.skeleton);

    scene->active_transform.insert(scene->active_transform.begin() + start,
                                   data.transforms.begin(),
                                   data.transforms.end());

    if (db[animation.get()])
      *(db[animation.get()]) = scene->skeletons.size() - 1;

    return animation;
  }

  static void setFrame(std::shared_ptr<core::Scene> &scene,
                       std::unique_ptr<Animation> &anim, AnimDatabase &db,
                       int frame_id) {

    auto skeleton = scene->skeletons[*db[anim.get()]].get();
    auto frame = anim->frames[frame_id];

    int frame_idx = anim->start + frame.start;
    int transform_idx = skeleton->transform_start;

    auto root_transform = &scene->active_transform[transform_idx];
    *root_transform = scene->active_transform[frame_idx];
    auto root_world = &scene->active_world_transform[transform_idx];
    *root_world = scene->active_transform[transform_idx];

    frame_idx++;
    for (int joint = skeleton->joint_start + 1; joint < skeleton->size;
         joint++) {

      auto ac_joint = scene->joints[joint];

      auto joint_transform =
          &scene->active_transform[transform_idx + ac_joint.id];
      *joint_transform = scene->active_transform[frame_idx++];

      auto bind_transform = &scene->bind_transform[transform_idx + ac_joint.id];
      *bind_transform = scene->active_transform[transform_idx + ac_joint.id];

      auto active_world_transform =
          &scene->active_world_transform[transform_idx + ac_joint.id];

      *active_world_transform =
          scene->active_transform[transform_idx + ac_joint.id] *
          scene->active_world_transform[transform_idx + ac_joint.parent];
    }
  }
};
} // namespace anim

#endif // ANIMATIONLOADER_H_
