#ifndef UI_H_
#define UI_H_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "memory.h"
#include "scene.hpp"
#include "transform.hpp"
#include <string>

struct Ui {
  static void Init(GLFWwindow *context) {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(context, false);
  }

  static void loadFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  static void setup() {
    bool show_debug = true;
    ImGui::Begin("Debug Window", &show_debug);
  }

  static void addString(std::string data) { ImGui::Text("%s", data.c_str()); }

  // Create UI for loading transform data
  static void addTransform() {}

  static void draw() {
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  static void terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }

  static void addSkeletonTree(anim::Skeleton &skeleton,
                              std::vector<anim::Joint> &joints,
                              int *selected_transform) {
    // This skeleton will add the skeleton tree.
    std::string header_name = "Skeleton: " + std::to_string(skeleton.id);
    if (ImGui::CollapsingHeader(header_name.c_str())) {
      // Get the joint vector.
      for (int i = 0; i < skeleton.size; i++) {
        int id = i + skeleton.joint_start;
        int transform_id = i + skeleton.transform_start;
        if (ImGui::Selectable(joints[id].name.c_str(),
                              selected_transform == &transform_id)) {
          *selected_transform = transform_id;
        }
      }
    }
  }

  static void HelpMarker(const char *desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered()) {
      ImGui::BeginTooltip();
      ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
      ImGui::TextUnformatted(desc);
      ImGui::PopTextWrapPos();
      ImGui::EndTooltip();
    }
  }
};

#endif // UI_H_
