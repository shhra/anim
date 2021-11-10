#include "transform.hpp"
// #include "glm/glm.hpp"
// #include "glm/gtx/quaternion.hpp"

Transform Transform::lookAt(glm::vec3 &target, glm::vec3 &f) {
  auto t = glm::normalize(target);
  if (f == t) {
    return *this;
  } else if (f == t * -1.0f) {
    glm::vec3 ortho = glm::vec3(1, 0, 0);
    if (fabsf(f.y) < fabsf(f.x)) {
      ortho = glm::vec3(0, 1, 0);
    }
    if (fabsf(f.z) < fabs(f.y) && fabs(f.z) < fabsf(f.x)) {
      ortho = glm::vec3(0, 0, 1);
    }
    glm::vec3 axis = glm::normalize(glm::cross(f, ortho));
    auto rotation = glm::angleAxis(glm::radians(0.0f), axis);
    return Transform(glm::vec3(0.0), rotation, glm::vec3(1.0));
  }
  glm::vec3 half = glm::normalize(f + t);
  glm::vec3 axis = glm::cross(f, half);
  auto rotation = glm::quat(glm::dot(f, half), axis.x, axis.y, axis.z);
  return Transform(glm::vec3(0.0), rotation, glm::vec3(1.0));
}

Transform Transform::fromMatrix(glm::mat4 &matrix) {
    // TODO complete this.
    return Transform();
}

inline Transform Transform::fromRotation(glm::quat &rotation) {
  return Transform(glm::vec3(0.0), rotation, glm::vec3(1.0));
}

inline Transform Transform::fromTranslation(glm::vec3 &translation) {
  return Transform(translation, glm::quat(1.0, 0.0, 0.0, 0.0), glm::vec3(1.0));
}

glm::mat4 Transform::toMat4() {
    // TODO: Complete this.
    return glm::mat4(1.0);
}

Transform Transform::operator*(const Transform &b) {
    auto rotation = this->rotation * b.rotation;
    auto scale = this->scale * b.scale;
    // Understand this operation and why is it done.
    auto position = this->rotation * b.position;
    position = this->scale * position;
    position += this->position;
    return Transform(position, rotation, scale);

}
