#ifndef INVERSE_KINEMATICS_H_
#define INVERSE_KINEMATICS_H_

#include "../core/scene.hpp"
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>

namespace anim {

struct InverseKinematics {

  static void applyIK(std::unique_ptr<core::Scene> &scene,
                      anim::Skeleton &skeleton, core::Transform &target) {

    // Fetch the tip of the skeketon
    core::Transform &tip =
        scene->active_transform[skeleton.transform_start + skeleton.size - 1];

    tip.rotation = target.rotation;

    // Get skeleton chain.
    int iter = 0;
    while (iter < 25) {
      for (int i = skeleton.size - 1; i >= 0; i--) {
        int idx = skeleton.transform_start + i;
        glm::vec3 ui =
            scene->active_world_transform[idx].position - tip.position;
        ui = glm::normalize(ui);
        glm::vec3 vi =
            scene->active_world_transform[idx].position - target.position;
        vi = glm::normalize(vi);
        glm::vec3 wi = glm::cross(ui, vi);
        wi = glm::normalize(wi);

        float dot = glm::dot(ui, vi);
        float angle = glm::acos(dot);

        // Handle target rotation
        glm::quat r = target.rotation * glm::inverse(tip.rotation);

        glm::vec3 forward = r * glm::vec3(0.0, 0.0, -1.0);
        glm::vec3 up = r * glm::vec3(0.0, 1.0, 0.0);

        glm::quat rotation = glm::angleAxis(angle, wi);
        forward += rotation * glm::vec3(0.0, 0.0, -1.0);
        up += rotation * glm::vec3(0.0, 1.0, 0.0);

        auto &current_transform = scene->active_world_transform[idx];
        // Update the world transform here.
        core::Transform t = core::Transform();
        up = glm::normalize(0.5f * up);
        forward = glm::normalize(0.5f * forward);
        t = t.lookAt(up, forward);
        current_transform.rotation = t.rotation;
      }
      if (glm::length(tip.position - target.position) < 1e-3) {
        break;
      }
      iter += 1;
    }
  }
};
} // namespace anim
#endif // INVERSE_KINEMATICS_H_
