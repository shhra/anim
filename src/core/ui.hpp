#ifndef UI_H_
#define UI_H_

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <string>

class Ui {
public:
  Ui(GLFWwindow *context) {
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplGlfw_InitForOpenGL(context, false);
  }

  void loadFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
  }

  void setup() {
    bool show_debug = true;
    ImGui::Begin("Debug Window", &show_debug);
  }

  void addString(std::string data) { ImGui::Text("%s", data.c_str()); }

  void draw() {
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  void terminate() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
  }
};

#endif // UI_H_
