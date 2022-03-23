#include "transform.hpp"
// #include "glm/glm.hpp"
// #include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "iostream"

namespace core {

Transform::Transform(glm::mat4 &matrix) {
  // Extract position, scale and rotation from the given matrix.
  this->position = glm::vec3(matrix[3]);

  for (int i = 0; i < 3; i++) {
    this->scale[i] = glm::length(glm::vec3(matrix[i]));
  }
  const glm::mat3 rotationMat(glm::vec3(matrix[0] / this->scale[0]),
                              glm::vec3(matrix[1] / this->scale[1]),
                              glm::vec3(matrix[2] / this->scale[2]));
  this->rotation = glm::quat_cast(rotationMat);
}

inline Transform Transform::fromRotation(glm::quat &rotation) {
  return Transform(glm::vec3(0.0), rotation, glm::vec3(1.0));
}

inline Transform Transform::fromTranslation(glm::vec3 &translation) {
  return Transform(translation, glm::quat(1.0, 0.0, 0.0, 0.0), glm::vec3(1.0));
}

glm::mat4 Transform::toMat4() const {
  return glm::translate(glm::mat4(1.0), this->position) *
         glm::mat4_cast(this->rotation) *
         glm::scale(glm::mat4(1.0), this->scale);
}

Transform Transform::operator*(const Transform child) const {
  auto r = this->rotation * child.rotation;
  auto s = this->scale * child.scale;
  auto t = this->rotation * child.position;
  t = this->scale * t;
  t += this->position;
  return Transform(t, r, s);
}

} // namespace core
