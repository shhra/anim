#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../core/scene.hpp"
#include "../core/transform.hpp"
#include "bone.hpp"
#include "bone_mesh.hpp"
#include "bvhimporter.hpp"
#include "frame.hpp"
#include "vector"
#include <memory>
#include <unistd.h>

namespace core {
class Animation {
public:
  // TODO: Instead of creating an skeleton object inside animation class, I
  // think it will be better to pass in the skeleton id. This way animation will
  // act on data and it will decouple from skeleton.
  Skeleton skeleton;

  Animation() {
    frames = {};
    this->skeleton = Skeleton();
    bone = anim::BoneMesh();
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

  void addFrame(Frame &frame) { frames.push_back(frame); }

  void setInit() {
    initFrame = frames[0];
    activeFrame = initFrame;
    // TODO: Make sure this init is called for motion parsing only.
    skeleton.setRoot(0);
    skeleton.setTransforms(activeFrame);
    skeleton.setWorldTransforms();
    skeleton.bindTransforms();
  }

  // void setRetargeter() { animRetarget = AnimationRetargetter(&this->skeleton); }

  // void setMap(std::vector<std::string> &source,
  //             std::vector<std::string> &target) {
  //   animRetarget.createMap(source, target);
  // }

  void bind() { skeleton.bindTransforms(); }

  void play(Shader &shader, Skeleton *target = nullptr) {
    if (index >= frames.size()) {
      index = 0;
      activeFrame = initFrame;
    }
    skeleton.setTransforms(activeFrame);
    skeleton.setWorldTransforms();

    // Use this skeleton to retarget another skeleton.
    // if (target != nullptr)
    //   animRetarget.retarget(target);

    //    skeleton.drawJoints(shader, bone);
    if (frames.size() > 0 && index > 0) {
      // Current frame * prev frame. This helps bring to current frame
      // reference.
      auto resultFrame = frames[index] * activeFrame;
      activeFrame = resultFrame;
      // TODO: Fix is how bvh take the animation
      // activeFrame = frames[index];
    }
    index++;
  }

private:
  // Remove "frames from animation too." And create allow animation to process
  // frame data instead.
  // Therefore these frames are stored outside of animation.
  std::vector<Frame> frames;
  // The inital frame for the animation.
  Frame initFrame;
  Frame activeFrame;   // It's okay to keep it inside.
  anim::BoneMesh bone; // Can be passed a bonemesh pointer.
  // High Risk: Data race. Better method needed.
  int index;

  // Make retargetter take in the animation instead of it being
  // inside animation it being inside animation
  // AnimationRetargetter animRetarget;
};
} // namespace core

namespace anim {

struct Frame {
  int start;
  int end;
  Frame(int start, int end) : start(start), end(end) {}
};

struct Animation {
  int start; // Transforms.
  int end;   // Transforms.
  float frame_time;
  std::vector<Frame> frames;
  Animation() : start(0), end(0), frames({}), frame_time(0.0f) {}
};


} // namespace anim

#endif // ANIMATION_H_
