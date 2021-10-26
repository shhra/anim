#ifndef BONE_H
#define BONE_H

#include "../core/models.hpp"
#include "../core/shader.hpp"
#include "../core/transform.hpp"
#include "./bone_mesh.hpp"

#include "glm/gtx/string_cast.hpp"
#include "iostream"
#include "memory"
#include "string.h"

struct Joint {
  Joint(std::string bone_name, unsigned int bone_id, unsigned int parent_id) {
    name = bone_name;
    id = bone_id;
    parent = parent_id;
    transform = Transform();
    worldTransform = glm::mat4(1.f);
  }

  glm::mat4 getLocalToParent() {
    glm::mat4 localTransform = glm::mat4(1.f);
    localTransform = glm::translate(localTransform, transform.position);
    localTransform *= glm::toMat4(transform.rotation);
    if (parent != -1) {
      localTransform =
          glm::scale(localTransform, glm::vec3(1.f));
    }
    // TODO: Add this scaling factor if necessary.
    return localTransform;
  }

  friend std::ostream &operator<<(std::ostream &os, const Joint &j);
  unsigned int getParent() { return parent; }

  unsigned int id;
  unsigned int parent;
  glm::mat4 worldTransform;
  Transform transform;
  std::string name;
};

std::ostream &operator<<(std::ostream &os, Joint &j) {
  os << "\n\n========================================\n";
  os << "ID: " << j.id << "\n";
  os << "Parent ID: " << j.parent << "\n";
  os << "World Transform: " << glm::to_string(j.worldTransform) << "\n";
  os << "Position: " << glm::to_string(j.transform.position) << "\n";
  os << "Rotation: " << glm::to_string(j.transform.rotation) << "\n";
  os << "World Position: " << glm::to_string(j.transform.worldPosition) << "\n";
  os << "World Rotation: " << glm::to_string(j.transform.worldRotation) << "\n";
  os << "========================================\n";
  return os;
}

struct Skeleton {
  Skeleton() { joints = std::vector<Joint>{}; }

  void drawJoints(Shader &shader, BoneMesh &joint_model) {
    // Assumes that root is at 0th position
    for (int i = 1; i < joints.size(); i++) {
      // auto world = joints[i].worldTransform;

      // Get the parent joint information.
      auto parent = joints[joints[i].parent];
      // auto towards =
      //     joints[i].transform.getGlobalPosition(joints[i].worldTransform);

      auto towards = joints[i].transform.worldPosition;
      // Find the direction from parents forward towards the that point.
      // auto rotate = glm::mat4_cast(glm::quatLookAt(towards, up));
      auto rotate = glm::mat4_cast(parent.transform.fromTo(towards));

      auto translate =
          glm::translate(glm::mat4(1.f), parent.transform.worldPosition);

      auto scale = glm::scale(
          glm::mat4(1.f), glm::vec3(glm::length(joints[i].transform.position)));

      auto world = translate * rotate * scale;
      // auto world = translate * rotate;

      shader.setMat4("model", world);

      joint_model.Draw(shader);
    }
  }

  void addJoint(std::string name, int parent) {
    int id = joints.size();
    Joint j = Joint(name, id, parent);
    joints.push_back(j);
  }

  void setTransforms(int joint_idx, glm::quat joint_orientation,
                     glm::vec3 joint_offset) {
    auto active_joint = &joints[joint_idx];

    active_joint->transform.position = joint_offset;
    active_joint->transform.rotation = joint_orientation;

    auto local = active_joint->getLocalToParent();
    if (active_joint->getParent() == -1) {
      active_joint->worldTransform = local;
      active_joint->transform.worldRotation = active_joint->transform.rotation;
      active_joint->transform.worldPosition = active_joint->transform.position;
      std::cout << "Rotation: "
                << glm::to_string(active_joint->transform.getUp()) << "\n";
    } else {
      Joint parent = joints[active_joint->getParent()];
      glm::mat4 pWorld = parent.worldTransform;
      glm::mat4 world = pWorld * local;
      active_joint->worldTransform = world;
      active_joint->transform.worldRotation =
          parent.transform.worldRotation * active_joint->transform.rotation;

      active_joint->transform.worldPosition =
          glm::vec3(world[3][0], world[3][1], world[3][2]);
    }

    std::cout << *active_joint;
  }

private:
  std::vector<Joint> joints;
};

#endif /* BONE_H*/
