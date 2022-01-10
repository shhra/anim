#ifndef BVHIMPORTER_H_
#define BVHIMPORTER_H_

#include "animation.hpp"
#include "frame.hpp"
#include "bone.hpp"
#include <fstream>
#include <unordered_map>

namespace anim {

//! Handles importing of motion capture data stored in BVH format.
//! It will extract the bones as well as frames for the given animation.
class BVHImporter {
public:
  BVHImporter(std::string filename);

  core::Skeleton skeleton = core::Skeleton();

  // Stores all the transforms for the animation.
  std::vector<core::Transform> transforms;

  //! Data will be stored in frames.
  Animation animation = Animation();

  //! Time for each frame.
  float frame_time;

private:
  bool parseTree(std::ifstream &fs, int parent);

  bool parseNode(std::ifstream &fs, int parent, std::string name);

  bool parseJoint(std::ifstream &fs, int parent, std::string name);

  std::string parseChannel(std::ifstream &fs);

  bool parseMotion(std::ifstream &fs);

  //! Number of frames in the animation.
  int frame_len;

  void createFrame(std::string &line, bool first);


  //! Data gets stored in frames.
  std::vector<Frame> frames;

  //! A map that stores the channel info for given joint id.
  std::unordered_map<int, std::string> joint_channels;

  //! Skip end
  bool skipEnd(std::string line);

  std::vector<std::string> split(std::string &input);
};

}

#endif // BVHIMPORTER_H_
