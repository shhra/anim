#include "transform.hpp"
// #include "glm/glm.hpp"
// #include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include "iostream"

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

Transform::Transform(glm::mat4 &matrix) {
  // Extract position, scale and rotation from the given matrix.
  this->position = matrix[3];

  for(int i= 0; i < 3; i++) {
    this->scale[i] = glm::length(glm::vec3(matrix[i]));
  }
  const glm::mat3 rotationMat(glm::vec3(matrix[0] / scale[0]),
                                     glm::vec3(matrix[1] / scale[1]),
                                     glm::vec3(matrix[2] / scale[2]));
  this->rotation = glm::quat_cast(rotationMat);
}

inline Transform Transform::fromRotation(glm::quat &rotation) {
  return Transform(glm::vec3(0.0), rotation, glm::vec3(1.0));
}

inline Transform Transform::fromTranslation(glm::vec3 &translation) {
  return Transform(translation, glm::quat(1.0, 0.0, 0.0, 0.0), glm::vec3(1.0));
}

glm::mat4 Transform::toMat4() {
  return glm::translate(glm::mat4(1.0), this->position) *
         glm::mat4_cast(this->rotation) *
         glm::scale(glm::mat4(1.0), this->scale);
}

Transform Transform::operator*(const Transform parent) {
  auto r = parent.rotation * this->rotation;
  auto s = this->scale * parent.scale;
  // Understand this operation and why is it done.
  auto t = parent.rotation * this->position;
  t = parent.scale * t;
  t += parent.position;

  // std::cout << "MULT:::: "<< glm::to_string(r) << "\n";

  return Transform(t, r, s);
}
