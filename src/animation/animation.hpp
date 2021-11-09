#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "bone.hpp"
#include "bone_mesh.hpp"
#include "frame.hpp"
#include "vector"

class Animation {
public:
  Animation() {
    frames = {};
    skeleton = Skeleton();
    bone = BoneMesh();
    index = 0;
  }

  void addJoints(std::vector<std::pair<std::string, int>> jointData) {
    for (auto &joint : jointData) {
      skeleton.addJoint(joint.first, joint.second);
    }
  }

  void addFrame(std::vector<glm::mat4> transforms) {
    Frame data = Frame();
    if (frames.size() == 0) {
      for (auto &transform : transforms) {
        data.addFrameData(transform);
      }
    } else {
      int current = frames.size();
      for (int i = 0; i < transforms.size(); i++) {
        auto previous = frames[current - 1].getJointTransform(i);
        data.addFrameData(previous * transforms[i]);
      }
    }
    frames.push_back(data);
  }

  void addFrame(std::vector<glm::quat> rotations,
                std::vector<glm::vec3> position) {

    Frame data = Frame();
    // Adds the frame using given position and rotations.
    auto translate = glm::vec3(0.f);
    if (frames.size() == 0) {
      for (int i = 0; i < rotations.size(); i++) {
        skeleton.setTransforms(i, glm::quat(1.0, 0.0, 0.0, 0.0), position[i]);
        translate += position[i];
        skeleton.setWorldPosition(i, translate);
        auto translate = glm::mat4(1);
        auto rotate = glm::toMat4(rotations[i]);
        auto transform = rotate * translate;
        data.addFrameData(transform);
      }
    } else {
      for (int i = 0; i < rotations.size(); i++) {
        auto transform = glm::toMat4(rotations[i]);
        transform = transform * frames[frames.size() - 1].getJointTransform(i);
        data.addFrameData(transform);
      }
    }

    frames.push_back(data);
  }

  void play(Shader &shader) {
    if (index >= frames.size()) {
      index = 0;
    }
    skeleton.setTransforms(frames[index]);
    // std::cout << "At index: " << index << "\n";
    skeleton.drawJoints(shader, bone);
    index++;
  }

  void stats() {
    skeleton.log();
    std::cout << "There are: " << frames.size() << " frames\n";
    for (auto &frame : frames) {
      // frame.log();
    }
    std::cout << "Processed.\n";
  }

private:
  Skeleton skeleton;
  std::vector<Frame> frames;
  BoneMesh bone;
  // High Risk: Data race. Better method needed.
  int index;
};

#endif // ANIMATION_H_
