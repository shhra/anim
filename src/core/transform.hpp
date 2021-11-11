#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

struct Transform {
  //! Defines the rotation of the object
  glm::quat rotation;
  //! Defines the position of the object
  glm::vec3 position;
  //! Defines the scale of the entity.
  glm::vec3 scale;

  Transform() {
    position = glm::vec3(0.0f);
    rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    scale = glm::vec3(1.f);
  }

  Transform(glm::vec3 T, glm::quat R, glm::vec3 S)
      : position(T), rotation(R), scale(S) {}

  //! Extracts the translation, rotation and scale from the matrix.
  Transform fromMatrix(glm::mat4 &matrix);

  //! Given the forward vector and towards direction, calculate the transform
  //! and return the rotated transform. This transformation mutates the state.
  Transform lookAt(glm::vec3 &target, glm::vec3 &forward);

  //! Create Transform just using rotation. This sets translation to 0 and scale
  //! to 1.
  inline Transform fromRotation(glm::quat &rotation);

  //! Create Transfrom from translation only. This sets rotation to unit
  //! quaternion. Scale is set to 1.
  inline Transform fromTranslation(glm::vec3 &translation);

  //! Add the rotation to the transform
  inline void rotate(glm::quat &rotation) { this->rotation *= rotation; }

  //! Let the unit vector in the local x direction
  inline glm::vec3 localX() {
    return this->rotation * glm::vec3(1.0, 0.0, 0.0);
  }

  //! Let the unit vector in the local x direction
  inline glm::vec3 localY() {
    return this->rotation * glm::vec3(0.0, 1.0, 0.0);
  }

  //! Let the unit vector in the local x direction
  inline glm::vec3 localZ() {
    return this->rotation * glm::vec3(0.0, 0.0, 1.0);
  }

  //! Get the required transformation matrix from translation, rotation and
  //! scale.
  glm::mat4 toMat4();

  //! This multiplies with the another transform b and return a new transform.
  Transform operator*(const Transform b);

};

#endif // TRANSFORM_H_
