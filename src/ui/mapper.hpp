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
    promptUser();
    // Creates an UI that allows mapping two Skeletons
    loadBoneData(motion_data);

    // Iterate through model skeleton and create a two UI that writes the
    // Relationship to a tuple.

    // Iterate throuh the tuple and store it as a map

    // If new map ask the user to save it.
  }

private:
  const std::vector<std::string> BoneNames = {};

  const std::map<std::string, std::string> SkeletonMap = {};
  bool load_new = false;
  bool show_browser = false;

  void loadBoneData(std::unique_ptr<anim::BVHImporter> &motion_data) {
    // Iterate through the skeleton data and write the motion data into
    // Bone Maps
  }

  void promptUser() {
    // Currently the file is checked from assets directory.
    std::filesystem::path asset_dir =
        "./assets"; // TODO: Make sure this directory exists
    std::filesystem::path file = std::filesystem::current_path();
    ImGui::Text("Mapper: ");
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
      show_browser = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("New")) {
      load_new = true;
    }

    if (ImGui::CollapsingHeader("Load previous", &show_browser,
                                ImGuiTreeNodeFlags_DefaultOpen)) {
      core::Files::load(asset_dir, ".txt", &file, &show_browser);
    }
  }
};

}; // namespace ui

#endif /* MAPPER_H */
