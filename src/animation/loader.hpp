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
  /** Loads the motion data into the scene.
   *
   * To load a motion data do the following things.
   * 1. Initialize an animation object.
   * 2. Load the relevant transforms.
   **/
  static std::unique_ptr<Animation>
  loadMotionData(std::unique_ptr<core::Scene> &scene, BVHImporter &data,
                 AnimDatabase &db) {
    /// 1. Initialize an animation object.
    ///    i. We need to know the start offset of the this animation data.
    ///    To find it, check the size of the frame_transforms.
    ///    ii. To define the end offset, fetch this information from the motion
    ///    data.
    int start = scene->frame_transforms.size();
    int end = start + data.animation.end;
    auto animation = std::make_unique<Animation>();

    animation->start = start;
    animation->end = end;
    animation->frame_time = data.animation.frame_time;
    animation->frames = std::move(data.animation.frames);

    /// 2. Load the relevant transforms.
    ///    i. Copy the frame data into the frame transforms.
    SkeletonTransformation::fillSkeletons(scene, data.skeleton, false);

    scene->frame_transforms.insert(scene->frame_transforms.begin() + start,
                                   data.transforms.begin(),
                                   data.transforms.end());

    if (db[animation.get()])
      *(db[animation.get()]) = scene->skeletons.size() - 1;

    return animation;
  }

  /** Initialize the rest pose for given skeleton using the motion data.
   *
   * Since the motion data may not have the proper initialization of the rest
   * pose, it is required to manually initialize the rest pose.
   *
   * To achieve this:
   *   1. Select the required skeleton for given animation from motion capture.
   *   2. Find the initial frame index.
   *   3. Apply the animation.
   **/
  static void initialize(std::unique_ptr<core::Scene> &scene,
                         std::unique_ptr<Animation> &anim, AnimDatabase &db) {

    /// 1. Select the required skeleton for given animation.
    ///    i. Use the animation database and the animation pointer to select the
    ///    required skeleton.
    auto skeleton = scene->skeletons[*db[anim.get()]];

    /// 2. Find the initial frame index
    ///    i. Select the 0th frame as the initial frame.
    ///    ii. Use the frame offset and animation start offset to find the
    ///    frame index.
    auto frame = anim->frames[0];
    int frame_idx = anim->start + frame.start;
    /// 3. Apply the animation.
    ///    i. Using the frame index, select the appropriate joint transform from
    ///    the `frame_transform`.
    ///    ii. Use the skeleton data to find the active transform for given
    ///    joint.
    int transform_idx = skeleton.transform_start;
    core::Transform &root_transform = scene->active_transform[transform_idx];

    /// Note: Frame index allows accessing the joint tranforms for the
    /// given frame. Each frame is designed in such a way that increasing the
    /// offset allows easy access of the joint data for given frame.
    root_transform = scene->frame_transforms[frame_idx];
    core::Transform &root_world = scene->active_world_transform[transform_idx];
    root_world = scene->frame_transforms[frame_idx];

    frame_idx++;
    for (int idx = 1; idx < skeleton.size; idx++) {

      int joint = skeleton.joint_start + idx;
      auto ac_joint = scene->joints[joint];

      core::Transform &joint_transform =
          scene->active_transform[transform_idx + ac_joint.id];
      joint_transform = scene->frame_transforms[frame_idx++];

      core::Transform &bind_transform =
          scene->bind_transform[transform_idx + ac_joint.id];
      bind_transform = scene->active_transform[transform_idx + ac_joint.id];

      core::Transform &active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];

      active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.parent] *
          scene->active_transform[transform_idx + ac_joint.id];

      core::Transform &bind_world_transform =
          scene->bind_world_transform[transform_idx + ac_joint.id];

      bind_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];
    }
  }

  /** Uses the frame id to set the skeleton pose.
   *
   * The core logic of the setting the animation is similar to the
   * initialization. However, the main difference is selecting the frame for
   * given id.
   *
   * The reason for code separation is because of setting the bind transform.
   * The initial frame requires the find transform.
   **/
  static void setFrame(std::unique_ptr<core::Scene> &scene,
                       std::unique_ptr<Animation> &anim, AnimDatabase &db,
                       int frame_id) {

    auto skeleton = scene->skeletons[*db[anim.get()]];
    auto frame = anim->frames[frame_id];

    int frame_idx = anim->start + frame.start;
    int transform_idx = skeleton.transform_start;

    // TODO: Common part can be refractored.
    core::Transform &root_transform = scene->active_transform[transform_idx];
    root_transform = scene->frame_transforms[frame_idx];
    core::Transform &root_world = scene->active_world_transform[transform_idx];
    root_world = scene->frame_transforms[frame_idx];

    frame_idx++;
    for (int idx = 1; idx < skeleton.size; idx++) {

      int joint = skeleton.joint_start + idx;
      auto ac_joint = scene->joints[joint];

      core::Transform &joint_transform =
          scene->active_transform[transform_idx + ac_joint.id];
      joint_transform = scene->frame_transforms[frame_idx++];

      core::Transform &bind_transform =
          scene->bind_transform[transform_idx + ac_joint.id];
      bind_transform = scene->active_transform[transform_idx + ac_joint.id];

      core::Transform &active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.id];

      active_world_transform =
          scene->active_world_transform[transform_idx + ac_joint.parent] *
          scene->active_transform[transform_idx + ac_joint.id];
    }
  }
};
} // namespace anim

#endif // ANIMATIONLOADER_H_
