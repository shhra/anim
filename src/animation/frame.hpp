#ifndef FRAME_H_
#define FRAME_H_

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>

class Frame {
public:
  Frame() { worldTransforms = {}; }

  void addFrameData(glm::mat4 transform) {
    worldTransforms.push_back(transform);
  }

  glm::mat4 getJointTransform(int id) { return worldTransforms[id]; }

  void log() {
    for (auto &transform : worldTransforms) {
      std::cout << glm::to_string(transform) << "\n";
    }
  }

private:
  std::vector<glm::mat4> worldTransforms;
};

#endif // FRAME_H_
