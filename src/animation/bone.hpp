#ifndef BONE_H
#define BONE_H

#include "../core/shader.hpp"
#include "../core/transform.hpp"
#include "./bone_mesh.hpp"
#include "./frame.hpp"

#include "glm/gtx/string_cast.hpp"
#include "iostream"
#include "memory"
#include "string.h"
#include <cstddef>
#include <unordered_map>

namespace core {

// A joint presents a bone in the skeleton. Each joint can be considered as
// a node in the skeleton. Joint can also help in skinning of the mesh.
// Joints are required to drive the skeletal animation of any given character
// mesh.
struct Joint {
  Joint(std::string bone_name, std::size_t bone_id, std::size_t parent_id);

  //! Simply copies the transform.
  void setTransform(const Transform &transform);

  //! Calculates the local transform. It requires parents world transform.
  void setLocalTransform(Transform &parent);

  //! Calculates the world transform. It requires parents world transform.
  void setWorldTransform(const Transform &parent);

  //! It sets the default value as the bind pose.
  void setBindTransforms();

  //! It is necessary for skinning. This sets the uniforms for the inverse bind
  //! transforms as well as current world transform.
  void bindUniforms(Shader &shader);

  void setParent(std::size_t parent_id);

  //! This id is used to identify the joints when creating the skeleton from
  //! glTF file.
  std::size_t id;

  //! Each joint has references to the parent. Instead of allowing the parents
  //! to have children, here we refere to the parent.
  std::size_t parent;

  //! Joint name is required for the retargeting the animation. It also plays
  //! important role in identifying the nodes in glTF file.
  std::string name;

  //! The transform and worldTransform of the joint change during the animation
  //! cycle. These represent the pose transformation.
  Transform transform;

  //! See definition for transform
  Transform worldTransform;

  //! The bind transforms are default transformations of the joint. Mostly
  //! These transforms are either T-posed or A-posed to represent the resting
  //! pose of the joint.
  Transform bindTransform;

  //! See definition for bindTransform.
  Transform bindWorldTransform;

  //! The inverse bind transforms allow the users to map joints into the skin
  //! space of the space.
  //! Since worldTransform * inverseBindTransform = Identiy.
  //! If the worldTransform is moved, it can be used to find where the skin mesh
  //! will move.
  glm::mat4 inverseBindPose;
};

//! A skeleton is a tree that stores the all the bones in that form the skeleton
//! of the character. It provides necessary functions that allows the users to
//! set different transforms which will be used in animation.
struct Skeleton {
  Skeleton();
  //! Write the logs for the skeleton. It prints only necessary information.
  //! TODO: create a process to log necessary or desired variables.
  void log();

  //! These requires a bone besh to draw the skeleton. The bone is a small mesh
  //! that can represent a joint. It draws a bone from parent position to it's
  //! current position
  void drawJoints(Shader &shader, anim::BoneMesh &joint_model);

  //! This one doesn't require the child id. This can be used when one is
  //! creating the Skeleton from something like BVH.
  void addJoint(std::string name, int parent);

  //! This version requires child id. This is done to align the the glTF joint
  //! convention.
  void addJoint(std::string name, int child, int parent);

  void setParent(int child, int parent);

  //! It takes quaternion, position and scale to and sets the value to joint
  //! local transform given by joint_idx
  void setTransforms(int joint_idx, glm::quat joint_orientation,
                     glm::vec3 joint_offset, glm::vec3 scale = glm::vec3(1.0));

  //! This version just creates local tranform from the glm::mat4.
  void setTransforms(std::string joint_name, glm::mat4 transform);

  //! Unlike other versrion, this transformation uses animation frame to
  //! set the local transform.
  void setTransforms(Frame frame);

  //! This is optional method and rarely used. Only when you have computed the
  //! world transformation for the given frame, then it safe to use this.
  //! Most of the time prefer `setTransforms(Frame frame)` followeb by
  //! `setWorldTransforms()`
  void setWorldTransforms(Frame frame);

  //! It takes the joint_name and jointIdx to set the transform for the given
  //! joint. It is required when parsing transforms from external format such as
  //! glTF
  //! TODO: Remove joint_name.
  void setWorldTransforms(std::string joint_name, int jointIdx,
                          glm::mat4 transform);

  //! It calculates the world transforms from already set local transform.
  void setWorldTransforms();

  //! Call this function once the skeleton is loaded. It will use the default
  //! transforms to calcuate the bind transforms.
  void bindTransforms();

  //! It calculates the local transforms from initialized world transforms.
  void setLocalTransform();

  //! Required for skinning.
  void bindUniforms(Shader &shader);

  //! Use joint name to fetch the joint. Necessary in animation retargetting.
  Joint &get_joint(std::string name);

  //! Use joint id to fetch the joint.
  Joint &get_joint(int id);

  std::size_t size();

  //! Sets the root id
  void setRoot(std::size_t id) { this->root_id = id; }

  const std::vector<Joint> &getJoints() const { return joints; }

private:
  // TODO: Joints can be decouples from skeletons too.
  // Instead of using joints in this way, we can refer the joint by joint id.
  // These joints can be stored independently from skeleton.
  // As a result all the "transform" operation now changes into a joint
  // operation.
  // Decoupling the joints will make it easy to apply joint operations.
  std::vector<Joint> joints;

  //! Find the index of the joint with given id.
  std::size_t findJointIdx(std::size_t id);

  //! Find the index of the joint with given name.
  std::size_t findJointIdx(std::string name);

  //! Root id
  std::size_t root_id;

  //! Holds this skeleton tree.
  std::unordered_map<size_t, std::vector<size_t>> tree = {};
};
} // namespace core
#endif /* BONE_H*/
