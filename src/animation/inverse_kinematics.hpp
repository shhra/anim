#ifndef INVERSE_KINEMATICS_H_
#define INVERSE_KINEMATICS_H_

#include "../core/scene.hpp"
#include "joint.hpp"
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>

namespace anim {

struct InverseKinematics {

  static bool applyIK(std::unique_ptr<core::Scene> &scene,
                      anim::Skeleton &skeleton, core::Transform &target) {

    // Get skeleton chain.
    int iter = 0;
    while (iter < 13) {
      // Fetch the tip of the skeleton
      core::Transform &tip =
          scene->active_world_transform[skeleton.transform_start +
                                        skeleton.size - 1];
      for (int i = skeleton.size - 2; i >= 0; i--) {
        int idx = skeleton.transform_start + i;
        glm::vec3 ui =
            tip.position - scene->active_world_transform[idx].position;
        glm::vec3 vi =
            target.position - scene->active_world_transform[idx].position;

        glm::quat rotation = core::Transform().rotation;
        // if (glm::length(vi) > 1e-3) {
        rotation = core::Transform::lookAt(vi, ui).rotation;
        // }
        const auto current_transform = scene->active_world_transform[idx];
        core::Transform &current_local_transform = scene->active_transform[idx];

        auto world_rotation = current_transform.rotation * rotation;
        auto local_rotation =
            world_rotation * glm::inverse(current_transform.rotation);
        current_local_transform.rotation =
            local_rotation * current_local_transform.rotation;
      }
      if (glm::length(target.position - tip.position) < 1e-1) {
        anim::SkeletonTransformation::updateWorldTransforms(scene, skeleton.id);
        return true;
      }
      iter += 1;
      anim::SkeletonTransformation::updateWorldTransforms(scene, skeleton.id);
    }
    return false;
  }
};
} // namespace anim
#endif // INVERSE_KINEMATICS_H_
