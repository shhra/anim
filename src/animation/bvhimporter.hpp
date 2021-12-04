#ifndef BVHIMPORTER_H_
#define BVHIMPORTER_H_

#include "./bone.hpp"
#include "./frame.hpp"

//! Handles importing of motion capture data stored in BVH format.
//! It will extract the bones as well as frames for the given animation.
class BVHImporter {
public:
  BVHImporter(std::string filename);

private:
  bool parseTree(std::ifstream &fs, int parent);

  bool parseNode(std::ifstream &fs, int parent, std::string name);

  bool parseMotion();

  //! Data gets stored in frames.
  std::vector<Frame> frames;

  //! Data will be stored in frames.
  Skeleton skeleton;

  //! TODO: This is just temp for now.
  std::vector<int> joints;


  //! Skip end
  bool skipEnd(std::string line);

  std::vector<std::string> split(std::string &input);
};

#endif // BVHIMPORTER_H_
