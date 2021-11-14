#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../core/transform.hpp"
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

  void initRestFrame(std::vector<glm::quat> rotations,
                     std::vector<glm::vec3> positions) {
    initFrame = Frame();
    for (int i = 0; i < skeleton.size(); i++) {
      skeleton.setTransforms(i, rotations[i], positions[i]);
      Transform t;
      if (i == 0)
        t = Transform(positions[i], rotations[i], glm::vec3(0.5));
      else
        t = Transform(positions[i], rotations[i], glm::vec3(1.0));

      initFrame.addFrameData(t);
    }
    activeFrame = initFrame;
  }

  void addFrame(std::vector<glm::quat> rotations, glm::vec3 root_position) {

    Frame data = Frame();
    Transform root = Transform(root_position, rotations[0], glm::vec3(1.0));
    data.addFrameData(root);
    for (int i = 1; i < rotations.size(); i++) {
      Transform t = Transform(glm::vec3(0.0f), rotations[i], glm::vec3(1.0));
      data.addFrameData(t);
    }
    frames.push_back(data);
  }

  void play(Shader &shader) {
    if (index >= frames.size()) {
      index = 0;
      activeFrame = initFrame;
    }
    skeleton.setTransforms(activeFrame);
    // skeleton.setWorldTransforms(activeFrame);
    skeleton.setWorldTransforms();
    skeleton.drawJoints(shader, bone);
    if (frames.size() > 0) {
      // Current frame * prev frame. This helps bring to current frame reference.
      auto resultFrame = frames[index] * activeFrame;

      activeFrame = resultFrame;
    }
    index++;
  }

private:
  Skeleton skeleton;
  std::vector<Frame> frames;
  // The inital frame for the animation.
  Frame initFrame;
  Frame activeFrame;
  BoneMesh bone;
  // High Risk: Data race. Better method needed.
  int index;
};

#endif // ANIMATION_H_
