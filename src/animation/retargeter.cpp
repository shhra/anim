#include "retargeter.hpp"
#include <glm/gtx/string_cast.hpp>

void AnimationRetargetter::retarget(Skeleton *target) {
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

    // When inverting the rotation it changes to origin, and then adding the
    // parent world rotation brings it to the current target space.
    auto tar_src_diff = glm::conjugate(src_bone->bindWorldTransform.rotation) *
                        (tar_bone->bindWorldTransform.rotation);

    // Calculate the rotation difference
    // Adding the local rotation to the parent world rotation brings the
    // rotation of this joint to the world space.
    auto src_world_rotation = src_a * src_bone->transform.rotation;

    // Handle artifacts
    if (glm::dot(src_world_rotation, src_bone->bindWorldTransform.rotation) < 0) {
      src_world_rotation = src_world_rotation * (-tar_src_diff);
    } else {
      src_world_rotation = src_world_rotation * tar_src_diff;
    }
    // Finally it adds the different to the target's parent rotation to
    // calculate for the local rotation!
    tar_bone->transform.rotation = tar_a * src_world_rotation;
  };
  // Handle rotation.

  // TODO: Handle more erros here.
  for (auto &data : data_pair) {
    Joint &src = source->get_joint(data.first);
    Joint &tar = target->get_joint(data.second);
    // std::cout << "Mapping " << src.name  << " to " << tar.name << std::endl;
    rotate(&src, &tar);
  }

  auto handle_zeros = [](glm::vec3 &v) -> glm::vec3 {
    auto vec = v;
    if (std::abs(vec[0]) <= 1e-2)
      vec[0] = 0.0f;
    if (std::abs(vec[1]) <= 1e-2)
      vec[1] = 0.0f;
    if (std::abs(vec[2]) <= 1e-2)
      vec[2] = 0.0f;
    return vec;
  };

  auto div = [](glm::vec3 &a, glm::vec3 &b) -> glm::vec3 {
    auto data = a;
    data[0] = (b[0] != 0.0f) ? a.x / b.x : 0.0f;
    data[1] = (b[1] != 0.0f) ? a.y / b.y : 0.0f;
    data[2] = (b[2] != 0.0f) ? a.z / b.z : 0.0f;
    return data;
  };

  Joint &src_bone = source->get_joint("Hips");
  Joint &tar_bone = target->get_joint("mixamorig:Hips");
  auto a = handle_zeros(tar_bone.worldTransform.position);
  auto b = handle_zeros(src_bone.worldTransform.position);
  auto scale = div(a, b);
//  scale = glm::vec3(1.0f);

//   tar_bone.worldTransform.position = (src_bone.worldTransform.position -
//                                       src_bone.bindWorldTransform.position) *
//                                          scale +
//                                      tar_bone.bindWorldTransform.position;
//    tar_bone.transform = tar_bone.worldTransform;
  target->setWorldTransforms();
  // target->setLocalTransform();
}

void AnimationRetargetter::createMap(std::vector<std::string> &source,
                                     std::vector<std::string> &target) {

  // TODO: Insert assertion.
  if (source.size() != target.size()) {
    std::cerr << "Make sure the source and target are same." << std::endl;
  }

  for (int i = 0; i < source.size(); i++) {
    data_pair.push_back(
        std::pair<std::string, std::string>(source[i], target[i]));
  }
}
