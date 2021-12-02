#include "retargeter.hpp"

void AnimationRetargetter::retarget(Skeleton *target) {
  // Joint &src_bone = source->get_joint("root");
  // Joint &tar_bone = target->get_joint("start");

  auto rotate = [&](Joint *src_bone, Joint *tar_bone) {
    // Source's parent rotation.
    auto src_a =
        (src_bone->parent == -1)
            ? src_bone->bindWorldTransform.rotation
            : source->get_joint(src_bone->parent).bindWorldTransform.rotation;

    // Target's parent rotation.
    auto tar_a =
        (tar_bone->parent == -1)
            ? tar_bone->bindWorldTransform.rotation
            : target->get_joint(tar_bone->parent).bindWorldTransform.rotation;
    tar_a = glm::inverse(tar_a);

    // Handle rotation change.
    auto tar_src_diff = glm::inverse(src_bone->bindWorldTransform.rotation) *
                        (tar_bone->bindWorldTransform.rotation);

    // Calculate the rotation difference
    auto src_local_diff = src_a * src_bone->transform.rotation;

    auto diff = src_local_diff;
    // Handle artifacts
    if (glm::dot(src_bone->bindWorldTransform.rotation, src_local_diff) < 0) {
      diff = diff * (-tar_src_diff);
    } else {
      diff = diff * tar_src_diff;
    }

    diff = tar_a * diff;
    tar_bone->transform.rotation = diff;
  };
  // Handle rotation.

  Joint &src_bone = source->get_joint("root");
  // Joint &tar_bone = target->get_joint("arm_joint_1");
  Joint &tar_bone = target->get_joint("start");
  rotate(&src_bone, &tar_bone);

  Joint &sb2 = source->get_joint("first");
  Joint &tb2 = target->get_joint("first");
  // Joint &tb2 = target->get_joint("arm_joint_L_2");
  rotate(&sb2, &tb2);

  Joint &sb3 = source->get_joint("second");
  Joint &tb3 = target->get_joint("second");
  // Joint &tb3 = target->get_joint("arm_joint_L_3");
  rotate(&sb3, &tb3);

  // Joint &sb4 = source->get_joint("third");
  // Joint &tb4 = target->get_joint("third");
  // rotate(&sb4, &tb4);

  bool hip = true;
  if (hip) {
    auto scale = tar_bone.worldTransform.scale / src_bone.worldTransform.scale;
    tar_bone.worldTransform.position = (src_bone.worldTransform.position -
                                        src_bone.bindWorldTransform.position) *
                                           scale +
                                       tar_bone.bindWorldTransform.position;
    tar_bone.transform = tar_bone.worldTransform;
  }

  target->setWorldTransforms();
}
