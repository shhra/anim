#ifndef MAPPER_H
#define MAPPER_H

#include "../animation/bvhimporter.hpp"
#include "../core/files.hpp"
#include "../core/models.hpp"
#include "imgui.h"
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace ui {
struct BoneMapper {
  void saveBoneData(char *filename) {
    // Write the mapping for given mapper into the model.
  }

  void mapData(std::unique_ptr<core::Model> &target,
               std::unique_ptr<anim::BVHImporter> &motion_data) {
    // Promp the use to create a new map or load an old map.
    if (!promptUser()) {
      // Creates an UI that allows mapping two Skeletons
      loadBoneData(motion_data);
    }

    // Iterate through model skeleton and create a two UI that writes the
    // Relationship to a tuple.

    // Iterate throuh the tuple and store it as a map

    // If new map ask the user to save it.
  }

private:
  const std::vector<std::string> BoneNames = {};

  const std::map<std::string, std::string> SkeletonMap = {};

  void loadBoneData(std::unique_ptr<anim::BVHImporter> &motion_data) {
    // Iterate through the skeleton data and write the motion data into
    // Bone Maps
  }

  bool promptUser() {
    // Currently the file is checked from assets directory.
    std::filesystem::path asset_dir = "./assets";
    core::Files data = core::Files("data");
    ImGui::Text("Do you want to load mapper?");
    ImGui::Separator();
    bool load = false;
    if (ImGui::Button("Load previous")) {
      // Select file and load it.
      load = true;
      data.navigationUI(".txt", &load);
      load = true;
    }
    return load;
  }
};

}; // namespace ui

#endif /* MAPPER_H */
