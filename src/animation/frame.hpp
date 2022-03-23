#ifndef FRAME_H_
#define FRAME_H_

#include "../core/transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <iostream>
#include <vector>

namespace core {
class Frame {
public:
  Frame() { transforms = {}; }

  void addFrameData(Transform &transform) {
    // if (transforms.size() == 0) {
    transforms.push_back(transform);
    // } else {
    //   auto result = transform * transforms[transforms.size() - 1];
    //   transforms.push_back(result);
    // }
  }

  Transform operator[](int id) { return transforms[id]; }

  void setValue(int id, Transform transform) { transforms[id] = transform; }

  Frame operator*(const Frame &previous) {
    Frame f = previous;
    f.setValue(0, f[0]);
    for (int i = 1; i < transforms.size(); i++) {
      // This transform is the active frame. Therefore, it has to be parent for
      // the previous frame.
      auto res = this->transforms[i] * f[i];
      f.setValue(i, res);
    }
    return f;
  }

  void log(int i) {
    std::cout << "\n\n FRAME " << i << "====\n";
    for (auto &transform : transforms) {
      std::cout << "Rotation:  " << glm::to_string(transform.rotation) << "\n";
    }
  }

private:
  std::vector<Transform> transforms;
};
} // namespace core

namespace anim {

} // namespace anim

#endif // FRAME_H_
