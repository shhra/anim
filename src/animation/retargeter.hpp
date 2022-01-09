#ifndef VISUALIZATION_RETARGETER_HPP
#define VISUALIZATION_RETARGETER_HPP

#include "./bone.hpp"
#include <string>


namespace core {
// Animation retargetter is a class that can be used to retarget animation.
class AnimationRetargetter {
public:
  AnimationRetargetter() : source(nullptr){};

  AnimationRetargetter(Skeleton *source) : source(source) {}

  //! Retargetter requires the target skeleton in which this animation should be
  //! applied.
  void retarget(Skeleton *target);

  //! Sets the Retargetting map for two joints.
  void createMap(std::vector<std::string> &source,
                 std::vector<std::string> &target);

private:
  Skeleton *source;
  using RetargetMap = std::vector<std::pair<std::string, std::string>>;
  RetargetMap data_pair;
};
}

#endif // VISUALIZATION_RETARGETER_HPP
