#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

struct Transform {
  glm::vec3 forward;
  glm::vec3 up;
  glm::quat worldRotation;
  glm::vec3 worldPosition;
  glm::quat rotation;
  glm::vec3 position;
  float scale;

  Transform() {
    forward = glm::vec3(0.0f, 0.0f, 1.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    position = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scale = 1.f;
  }

  glm::vec3 getForward() {
    auto forward4 = glm::vec4(forward.x, forward.y, forward.z, 1.0);
    auto rotated = glm::toMat4(rotation) * forward4;
    return  glm::normalize(glm::vec3(rotated));

    // return glm::normalize(glm::rotate(rotation, forward));
  }

    glm::vec3 getUp() {
        auto up4 = glm::vec4(up.x, up.y, up.z, 1.0);
        auto rotated = glm::toMat4(rotation) * up4;
        // return glm::normalize(glm::vec3(rotated));
        return  glm::normalize(glm::vec3(rotated));
    }

  glm::quat fromTo(glm::vec3 towards) {

    auto t = glm::normalize(towards);
    auto f = glm::vec3(0.f, 0.f, 1.0f);
    if (f == t) {
      return rotation;
    } else if (f == t * -1.0f) {
      glm::vec3 ortho = glm::vec3(1, 0, 0);
      if (fabsf(f.y) < fabsf(f.x)) {
        ortho = glm::vec3(0, 1, 0);
      }
      if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
        ortho = glm::vec3(0, 0, 1);
      }
      glm::vec3 axis = glm::normalize(glm::cross(f, ortho));
      return glm::angleAxis(glm::radians(0.0f), axis);
    }
    glm::vec3 half = glm::normalize(f + t);
    glm::vec3 axis = glm::cross(f, half);
    return glm::quat(glm::dot(f, half), axis.x, axis.y, axis.z);
  }

  glm::vec3 getGlobalPosition(glm::mat4 world) {
    // auto result = world * glm::vec4(0.f, 0.f, 0.f, 1.0);

    // return  glm::vec3(result / result.w);
    return glm::vec3(world[3][0], world[3][1], world[3][2]);

  }
};

#endif // TRANSFORM_H_
