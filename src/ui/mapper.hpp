#ifndef MAPPER_H
#define MAPPER_H

#include "../animation/bvhimporter.hpp"
#include "../core/files.hpp"
#include "../core/models.hpp"
#include "imgui.h"
#include "imgui_internal.h"
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
    if (!target_loaded && !src_loaded) {
      loadBoneData(target, motion_data);
    }

    // Iterate through model skeleton and create a two UI that writes the
    // Relationship to a tuple.

    // Iterate throuh the tuple and store it as a map

    // If new map ask the user to save it.
    if (load_new) {
      // Create a new window.
      showMapper();
    }
  }

private:
  std::vector<std::string> target_bone_names = {};
  std::map<std::string, std::string> src_target_map = {};
  bool load_new = false;
  bool show_browser = false;
  bool target_loaded = false;
  bool src_loaded = false;

  void loadBoneData(std::unique_ptr<core::Model> &target,
                    std::unique_ptr<anim::BVHImporter> &motion_data) {
    for (auto bone : target->skeleton.getJoints()) {
      target_bone_names.push_back(bone.name);
    }
    for (auto bone : motion_data->skeleton.getJoints()) {
      src_target_map[bone.name] = "";
    }
    target_loaded = true;
    src_loaded = true;
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

  void showMapper() {
    ImGui::OpenPopup("Bone Mapper");
    if (ImGui::BeginPopupModal("Bone Mapper", nullptr,
                               ImGuiWindowFlags_MenuBar)) {
      if (ImGui::Button("Close")) {
        load_new = false;
        ImGui::CloseCurrentPopup();
      }
      ImGui::SameLine();
      if (ImGui::Button("Print")) {
        for (auto &data : src_target_map) {
          std::cout << data.first << " maps to " << data.second << std::endl;
        }
      }
      ImGui::Separator();
      for (auto &data : src_target_map) {
        // Begin dropdown and select the value;
        showDropDown(data.first, &data.second);
        ImGui::NewLine();
      }
      ImGui::EndPopup();
    }
  }

  void showDropDown(std::string name, std::string *item) {
    static int item_idx = 0;
    if (ImGui::BeginCombo(name.c_str(), (*item).c_str())) {
      for (int n = 0; n < target_bone_names.size(); n++) {
        const bool is_selected = (item_idx == n);
        if (ImGui::Selectable(target_bone_names[n].c_str(), is_selected))
          item_idx = n;
        if (is_selected) {
          ImGui::SetItemDefaultFocus();
        }
      }
      *item = target_bone_names[item_idx];
      ImGui::EndCombo();
    }
  }
};
}; // namespace ui

#endif /* MAPPER_H */
