#ifndef VISUALIZATION_RETARGETER_HPP
#define VISUALIZATION_RETARGETER_HPP

#include "./bone.hpp"

// Animation retargetter is a class that can be used to retarget animation.
class AnimationRetargetter {
public:
  AnimationRetargetter() : source(nullptr){};

  AnimationRetargetter(Skeleton *source) : source(source) {}

  //! Retargetter requires the target skeleton in which this animation should be
  //! applied.
  void retarget(Skeleton *target);

private:
  Skeleton *source;
};

#endif // VISUALIZATION_RETARGETER_HPP
