#ifndef INPUT_H_
#define INPUT_H_

#include "camera.hpp"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>

namespace core {

class Input {
  float last_cursorX, last_cursorY;
  bool first_mouse = true;
  float yoffset = 0.0f;
  bool fast = false;

public:
  Input(std::shared_ptr<CameraManager> &cam_manager, float x, float y)
      : cam_manager(cam_manager), last_cursorX(x), last_cursorY(y) {}

  std::pair<float, float> getCoords() {
    return std::pair<float, float>(last_cursorX, last_cursorY);
  }

  void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    bool left_action = false;
    bool right_action = false;
    // first_mouse = true;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      left_action = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
               GLFW_RELEASE) {
      left_action = false;
      // return;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      right_action = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
               GLFW_RELEASE) {
      right_action = false;
      // return;
    }

    if (!left_action && !right_action) {
      last_cursorX = xpos;
      last_cursorY = ypos;
      cam_manager->setInputs(xpos, ypos, left_action, right_action);
      return;
    }
    if (first_mouse) {
      last_cursorX = xpos;
      last_cursorY = ypos;
      first_mouse = false;
    }
    float xoffset = xpos - last_cursorX;
    float yoffset = last_cursorY - ypos;

    last_cursorX = xpos;
    last_cursorY = ypos;

    if (left_action && fast) {
      cam_manager->processInputs(xoffset, yoffset, false, true);
    }
    if (right_action && fast) {
      cam_manager->processInputs(xoffset, yoffset, true, false);
    }
  }

  void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) {
      fast = true;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_RELEASE) {
      fast = false;
    }
  }

  void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    float y = yoffset * 0.3;
    if (fast)
      y *= 10.0;
    cam_manager->processInputs(xoffset, y, false, false, true);
  }

private:
  std::shared_ptr<CameraManager> cam_manager;
};
} // namespace core
#endif // INPUT_H_
