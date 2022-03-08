#ifndef FILES_H_
#define FILES_H_

#include "imgui.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace core {
class Files {
public:
  Files(const char *name) {
    this->name = name;
    if (!std::filesystem::exists("../assets")) {
      std::filesystem::create_directory("../assets");
    }
    root = std::filesystem::absolute(std::filesystem::path("../assets"));
  }

  static void load(std::filesystem::path &path, std::string ext,
                   std::filesystem::path *data_file, bool* show) {
    auto root = std::filesystem::path(path);
    *data_file = root;
    // Iterate through this directory and load the animations.
    for (auto const &file : std::filesystem::directory_iterator(root)) {
      if ((file.path().stem().string().find(".") == std::string::npos) &&
          ((file.path().extension().string() == ext) ||
           (file.is_directory())) &&
          ImGui::Selectable(file.path().filename().c_str(),
                            *data_file == file.path(),
                            ImGuiSelectableFlags_AllowDoubleClick)) {
        if (file.is_directory()) {
          root = file.path();
        } else if (file.is_regular_file()) {
          if (ImGui::IsMouseDoubleClicked(0)) {
            *data_file = file.path();
            *show = false;
          }
        }
      }
    }
  }

  const std::vector<std::filesystem::path> &loaded() const { return files; }

  void navigationUI(const char *ext, bool *load) {
    ImGui::Begin(this->name.c_str());
    ImGui::Text("Navigate to desired %s location.", name.c_str());
    ImGui::Separator();
    ImGui::Text("Current Path: %s", root.filename().c_str());
    ImGui::Text("Selected File: %s", selected.filename().c_str());
    ImGui::SameLine();
    if (ImGui::Button("Load")) {
      show_navigation = true;
    }
    ImGui::Separator();
    if (ImGui::CollapsingHeader("Browse Files", &show_navigation,
                                ImGuiTreeNodeFlags_DefaultOpen)) {
      if (ImGui::Button("parent")) {
        root = root.parent_path();
      }
      for (auto const &file : std::filesystem::directory_iterator(root)) {
        if ((file.path().stem().string().find(".") == std::string::npos) &&
            ((file.path().extension().string() == ext) ||
             (file.is_directory())) &&
            ImGui::Selectable(file.path().filename().c_str(),
                              selected == file.path(),
                              ImGuiSelectableFlags_AllowDoubleClick)) {
          if (file.is_directory()) {
            root = file.path();
          } else if (file.is_regular_file()) {
            if (ImGui::IsMouseDoubleClicked(0)) {
              selected = file.path();
              show_navigation = false;
              *load = true;
            }
          }
        }
      }
    }
    ImGui::End();
  }

  const char *selected_file() { return selected.c_str(); }

private:
  std::string name;
  bool show_navigation = false;
  std::filesystem::path root;
  std::filesystem::path selected = std::filesystem::current_path();
  std::vector<std::filesystem::path> files;
};
} // namespace core
#endif // FILES_H_
