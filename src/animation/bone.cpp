#include "bone.hpp"

Joint::Joint(std::string bone_name, unsigned int bone_id,
             unsigned int parent_id) {
  // TODO: Create a memory copy constructor.
  name = bone_name;
  id = bone_id;
  parent = parent_id;
  transform = Transform();
}

void Joint::setWorldTransform(const Transform &parent) {
  worldTransform = this->transform * parent;
}

void Joint::setLocalTransform(Transform &parent) {
  glm::mat4 parentTransform = glm::inverse(parent.toMat4());
  glm::mat4 myTransform = this->worldTransform.toMat4();
  glm::mat4 local = parentTransform * myTransform;
  this->transform = Transform(local);
  // TODO: Fix this block later.

  //    this->transform.rotation =
  //        this->worldTransform.rotation * glm::conjugate(parent.rotation);
  //    this->transform.scale = this->worldTransform.scale / parent.scale;
  //    this->transform.position =
  //        (parent.position - this->worldTransform.position) *
  //        this->transform.scale;
}

void Joint::setTransform(const Transform &transform) {
  this->transform = transform;
}

void Joint::setParent(unsigned int parent_id) { parent = parent_id; }

// friend std::ostream &operator<<(std::ostream &os, const Joint &j);
unsigned int Joint::getParent() { return parent; }

void Joint::setBindTransforms() {
  this->bindTransform = this->transform;
  this->bindWorldTransform = this->worldTransform;
  this->inverseBindPose = glm::inverse(this->worldTransform.toMat4());
}

void Joint::bindUniforms(Shader &shader) {
  std::string name = "inversebindPose[" + std::to_string(id) + "]";
  shader.setMat4(name, inverseBindPose);

  std::string world_name = "worldPose[" + std::to_string(id) + "]";
  shader.setMat4(world_name, worldTransform.toMat4());
}

Skeleton::Skeleton() { joints = std::vector<Joint>{}; }
void Skeleton::log() {
  std::cout << "There are " << joints.size() << " joints.\n";
  // Also print all the joint name and its parent name is prettified format.
  for (auto &j : joints) {
    if (j.parent == -1) {
      std::cout << "Joint " << j.name << " (id: " << j.id
                << ") has no parent.\n";
    } else {
      std::cout << "Joint " << j.name << "(id: " << j.id << ") has parent "
                << joints[j.parent].name << ".\n";
    }

    // // Print it's local transform.
    // std::cout << "Joint " << j.name << "'s local transform is:\n"
    //           << glm::to_string(j.transform.toMat4()) << "\n";

    // // Also print it's world transform.
    // std::cout << "Joint " << j.name << "'s world transform is "
    //           << glm::to_string(j.worldTransform.toMat4()) << ".\n";
  }
}

void Skeleton::drawJoints(Shader &shader, BoneMesh &joint_model) {
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

void Skeleton::addJoint(std::string name, int child, int parent) {
  joints.emplace_back(name, child, parent);
}

void Skeleton::addJoint(std::string name, int parent) {
  int id = joints.size();
  joints.emplace_back(name, id, parent);
}

void Skeleton::setParent(int child, int parent) {
  // Find the index of parent joint
  int parent_id = findJointIdx(parent);
  // select the child joint with the given id and set its parent to parent_id
  for (auto &j : joints) {
    if (j.id == child) {
      j.setParent(parent_id);
      break;
    }
  }
}

void Skeleton::setTransforms(int joint_idx, glm::quat joint_orientation,
                             glm::vec3 joint_offset, glm::vec3 scale) {

  // Find the joint position for the given joint index.
  int joint_id = findJointIdx(joint_idx);
  auto active_joint = &joints[joint_id];
  active_joint->transform.position = joint_offset;
  active_joint->transform.rotation = joint_orientation;
  active_joint->transform.scale = scale;
}

void Skeleton::setTransforms(std::string joint_name, glm::mat4 transform) {
  int joint_id = findJointIdx(joint_name);
  auto active_joint = &joints[joint_id];
  auto trans = Transform(transform);
  active_joint->setTransform(trans);
}

void Skeleton::setTransforms(Frame frame) {
  for (int i = 0; i < size(); i++) {
    joints[i].setTransform(frame[i]);
  }
}

void Skeleton::setWorldTransforms(Frame frame) {
  for (int i = 0; i < size(); i++) {
    joints[i].setWorldTransform(frame[i]);
  }
}

void Skeleton::bindTransforms() {
  for (auto &j : joints) {
    j.setBindTransforms();
  }
}

void Skeleton::setLocalTransform() {
  for (int i = 0; i < joints.size(); i++) {
    auto joint = &joints[i];
    if (joint->parent == -1) {
      joint->transform = joint->worldTransform;
    } else {
      joint->setLocalTransform(joints[joint->parent].worldTransform);
    }
  }
}

void Skeleton::bindUniforms(Shader &shader) {
  for (auto &joint : joints)
    joint.bindUniforms(shader);
}

void Skeleton::setWorldTransforms(std::string joint_name, int jointIdx,
                                  glm::mat4 transform) {
  int joint_id = findJointIdx(joint_name);
  if (joint_id == -1) {
    return;
  }
  auto active_joint = &joints[joint_id];
  assert(active_joint->id == jointIdx);
  auto trans = Transform(transform);
  active_joint->setWorldTransform(trans);
}

void Skeleton::setWorldTransforms() {
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

Joint &Skeleton::get_joint(std::string name) {
  auto joint_id = findJointIdx(name);
  return (Joint &)joints[joint_id];
}

Joint &Skeleton::get_joint(int id) {
  auto joint_id = findJointIdx(id);
  return (Joint &)joints[joint_id];
}

unsigned int Skeleton::size() { return joints.size(); }

int Skeleton::findJointIdx(int id) {
  // TODO: Change the implementation to recursive defintion.
  for (int i = 0; i < joints.size(); i++) {
    if (joints[i].id == id) {
      return i;
    }
  }
  return -1; // This means the id doesn't exist which is never going to
             // happen.
}

int Skeleton::findJointIdx(std::string name) {
  // TODO: Change the implementation to recursive defintion.
  for (int i = 0; i < joints.size(); i++) {
    if (joints[i].name == name) {
      return i;
    }
  }
  return -1; // This means the id doesn't exist which is never going to
             // happen.
}
