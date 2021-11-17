#ifndef BONE_H
#define BONE_H

#include "../core/models.hpp"
#include "../core/shader.hpp"
#include "../core/transform.hpp"
#include "./bone_mesh.hpp"
#include "./frame.hpp"

#include "glm/gtx/string_cast.hpp"
#include "iostream"
#include "memory"
#include "string.h"

struct Joint {
  // TODO: Create a memory copy constructor.
  Joint(std::string bone_name, unsigned int bone_id, unsigned int parent_id) {
    name = bone_name;
    id = bone_id;
    parent = parent_id;
    transform = Transform();
  }

  void setWorldTransform(const Transform &parent) {
    worldTransform = this->transform * parent;
  }

  void setTransform(const Transform &transform) { this->transform = transform; }

  // friend std::ostream &operator<<(std::ostream &os, const Joint &j);
  unsigned int getParent() { return parent; }

  unsigned int id;
  unsigned int parent;
  std::string name;
  Transform transform;
  Transform worldTransform;
};

struct Skeleton {
  Skeleton() { joints = std::vector<Joint>{}; }

  void log() { std::cout << "There are " << joints.size() << " joints.\n"; }

  void drawJoints(Shader &shader, BoneMesh &joint_model) {
    for (int i = 1; i < joints.size(); i++) {
      auto parent = joints[joints[i].parent];
      auto towards = joints[i].worldTransform.position;
      auto parentP = parent.worldTransform.position;
      auto zAxis = glm::vec3(0.0, 0.0, 1.0);
      auto lookdir = (towards - parentP);
      auto rotate =
          glm::mat4_cast(parent.transform.lookAt(lookdir, zAxis).rotation);

      auto scaleFactor = joints[i].worldTransform.scale *
                         glm::length(joints[i].transform.position);
      auto translate = glm::translate(glm::mat4(1.f), parentP);
      auto scale = glm::scale(glm::mat4(1.f), scaleFactor);

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
  }

  void setTransforms(Frame frame) {
    for (int i = 0; i < size(); i++) {
      joints[i].setTransform(frame[i]);
    }
  }
  void setWorldTransforms() {
    for (int i = 0; i < joints.size(); i++) {
      auto joint = &joints[i];
      if (joint->parent == -1) {
        joint->worldTransform = joint->transform;
      } else {
        joint->setWorldTransform(joints[joint->parent].worldTransform);
      }
      // std::cout << *joint;
    }
  }

  void setWorldTransforms(Frame frame) {
    for (int i = 0; i < size(); i++) {
      joints[i].setWorldTransform(frame[i]);
    }
  }

  unsigned int size() { return joints.size(); }

private:
  std::vector<Joint> joints;
};

#endif /* BONE_H*/
