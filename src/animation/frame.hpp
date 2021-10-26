#ifndef FRAME_H_
#define FRAME_H_

#include "glm/glm.hpp"
#include <vector>

class Frame {
public:
  Frame() { worldTransforms = {}; }

  void addFrameData(glm::mat4 transform) { worldTransforms.push(transform); }

  void getJointTransfor(int id) { return worldTransforms[id]; }

private:
  std::vector<glm::mat4> worldTransforms;
};

#endif // FRAME_H_
