#ifndef FRAME_H_
#define FRAME_H_

#include "../core/transform.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/string_cast.hpp"
#include <vector>

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
    for (int i = 0; i < transforms.size(); i++) {
      // This transform is the active frame. Therefore it has to be parent for the
      // previous frame. '*' operator requires passing parent on RHS.
      auto res = f[i] * this->transforms[i];
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

#endif // FRAME_H_
