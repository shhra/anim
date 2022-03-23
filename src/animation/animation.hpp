#ifndef ANIMATION_H_
#define ANIMATION_H_

#include "../core/scene.hpp"
#include "../core/transform.hpp"
#include "bone.hpp"
#include "bone_mesh.hpp"
#include "frame.hpp"
#include "vector"
#include <memory>
#include <unistd.h>

namespace anim {

struct Frame {
  int start;
  int end;
  Frame(int start, int end) : start(start), end(end) {}
};

struct Animation {
  int start; // Frame offset to access the start of animation in `frame_transforms`
  int end;   // Specifies where the frame ends.
  float frame_time;
  std::vector<Frame> frames;
  Animation() : start(0), end(0), frames({}), frame_time(0.0f) {}
};


} // namespace anim

#endif // ANIMATION_H_
