#include "bvhimporter.hpp"
#include <fstream>
#include <glm/fwd.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <ostream>
#include <stack>
#include <string>

#define Line(filestream)                                                       \
  std::string line;                                                            \
  std::getline(filestream, line)

std::vector<std::string> BVHImporter::split(std::string &input) {
  std::vector<std::string> result = {};
  std::size_t pos;
  while ((pos = input.find(" ")) != std::string::npos) {
    auto sub = input.substr(0, pos);
    result.push_back(sub);
    input.erase(0, pos + 1);
  }
  int length = input.length();
  // Attach the last word.
  result.push_back(input.substr(0, length - 1));
  return result;
}

BVHImporter::BVHImporter(std::string filename) {
  // Open the file.
  std::ifstream file_stream;
  file_stream.open(filename);
  Line(file_stream);
  if (line.length() == 0) {
    std::cerr << "This file is a empty file fail to process it." << std::endl;
  }
  bool hierarchy_processed = false;
  if (line.find("HIERARCHY") != std::string::npos) {
    // Start parsing the skeleton data.
    hierarchy_processed = parseTree(file_stream, -1);
  }
  std::cout << line << std::endl;
  if (hierarchy_processed) {
    std::cout << "Parsing motion data" << std::endl;
    // Start parsing the motion data.
    parseMotion(file_stream);
  }
}

bool BVHImporter::parseTree(std::ifstream &fs, int parent) {
  // Read the first line.
  Line(fs);
  if (line.find("ROOT") != std::string::npos) {
    auto name = split(line)[1];
    std::cout << "Name of the root: " << name << std::endl;
    std::getline(fs, line); // Skip "{"
    return parseNode(fs, parent, name);
  } else {
    return false;
  }
}

bool BVHImporter::parseNode(std::ifstream &fs, int parent, std::string name) {
  // Read node data.
  std::stack<int> active_stack = {};
  active_stack.push(parent);
  while (!active_stack.empty()) {
    parent = active_stack.top();
    // Parse the joint data over here.
    std::cout << "Creating a joint with name: " << name << " and parent "
              << parent << std::endl;
    parseJoint(fs, parent, name);
    Line(fs);
    active_stack.push(animation.skeleton.size() - 1);
    if (skipEnd(line)) {
      for (int i = 0; i < 3; i++) {
        Line(fs);
      }
      bool pop = true;
      while (pop) {
        Line(fs);
        if (line.find("MOTION") != std::string::npos) {
          return true;
        }

        if (line.find("}") != std::string::npos) {
          active_stack.pop();
        } else {
          name = split(line)[1];
          pop = false;
        }
      }
    } else {
      name = split(line)[1];
    }
    { Line(fs); }
  }
  return true;
}

bool BVHImporter::parseJoint(std::ifstream &fs, int parent, std::string name) {
  // Read the offset line.
  Line(fs);
  std::cout << "Offset data: ";
  auto result = split(line);

  float x = std::stof(result[1].c_str());
  float y = std::stof(result[2].c_str());
  float z = std::stof(result[3].c_str());

  auto position = glm::vec3(x, y, z);
  std::cout << glm::to_string(position) << std::endl;
  // TODO: Make this function that always returns id.
  animation.skeleton.addJoint(name, parent);
  auto id = animation.skeleton.get_joint(name).id;

  // Fill in the joint offset.
  animation.skeleton.setTransforms(id, glm::quat(1.0, 0.0, 0.0, 0.0), position);

  // Read the channel data.
  auto channel = parseChannel(fs);

  // TODO: Handle error here for empty string.
  // TODO: Be sure that data is written only once.
  joint_channels[animation.skeleton.get_joint(name).id] = channel;

  return true;
}

std::string BVHImporter::parseChannel(std::ifstream &fs) {
  Line(fs);
  auto channel_info = split(line);

  std::string result = "";
  int length = channel_info.size();
  if (length == 9) { // THIS IS A HACK SPECIFIC TO MY DATASET
    length -= 1;
  }
  for (int i = 2; i <= length - 1; ++i) {
    result += channel_info[i][0];
  }

  std::cout << "Channel info: " << result << std::endl;
  return result;
}

bool BVHImporter::skipEnd(std::string line) {
  if (line.find("End") != std::string::npos) {
    return true;
  }
  return false;
}

bool BVHImporter::parseMotion(std::ifstream &fs) {
  // Read the frame length data
  {
    Line(fs);
    frame_len = std::stoi(split(line)[1]);
  }
  // Read the frame time.
  {
    Line(fs);
    frame_time = std::stof(split(line)[2]);
  }

  for (int i = 0; !fs.eof(); i++) {
    Line(fs);
    // Parse the motion data here.
    if (line.length() > 0) {
      bool first = i == 0 ? true : false;
      frames.push_back(createFrame(line, first));
    }
  }
  return true;
}

Frame BVHImporter::createFrame(std::string &frame_data, bool first) {
  auto data = split(frame_data);
  Frame frame = Frame();
  int idx = 0;

  for (int i = 0; i < animation.skeleton.size(); i++) {
    auto &joint = animation.skeleton.get_joint(i);

    glm::vec3 position = glm::vec3(0.0f);
    glm::quat orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    Transform transform;
    // Get the joint position
    if (joint_channels[i] == "XYZZYX") {
      glm::vec3 scale = glm::vec3(1.0f);
      position.x = std::stof(data[idx++]);
      position.y = std::stof(data[idx++]);
      position.z = std::stof(data[idx++]);
      if (first) {
        position += joint.transform.position;
      }

      // print this position value.
      std::cout << "Position: " << glm::to_string(position) << std::endl;

      scale = 1.0f * scale;

      // Get the joint orientation from the data. The data is using euler angle.
      // Convert it to quaternion using appropriate axis.
      float z = glm::radians(std::stof(data[idx++]));
      float y = glm::radians(std::stof(data[idx++]));
      float x = glm::radians(std::stof(data[idx++]));

      orientation *= glm::angleAxis(z, glm::vec3(0.0f, 0.0f, 1.0f));
      orientation *= glm::angleAxis(y, glm::vec3(0.0f, 1.0f, 0.0f));
      orientation *= glm::angleAxis(x, glm::vec3(1.0f, 0.0f, 0.0f));
      transform = Transform(position, orientation, scale);
    } else if (joint_channels[i] == "ZYX") {
      float z = glm::radians(std::stof(data[idx++]));
      float y = glm::radians(std::stof(data[idx++]));
      float x = glm::radians(std::stof(data[idx++]));

      orientation *= glm::angleAxis(z, glm::vec3(0.0f, 0.0f, 1.0f));
      orientation *= glm::angleAxis(y, glm::vec3(0.0f, 1.0f, 0.0f));
      orientation *= glm::angleAxis(x, glm::vec3(1.0f, 0.0f, 0.0f));
      transform =
          Transform(joint.transform.position, orientation, glm::vec3(1.0f));
    }

    frame.addFrameData(transform);
  }
  animation.addFrame(frame);
  return frame;
}
