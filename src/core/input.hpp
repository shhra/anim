#ifndef INPUT_H_
#define INPUT_H_

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>

class Input {
  float last_cursorX, last_cursorY;
  bool first_mouse = true;
  float yoffset = 0.0f;

public:
  Input(float x, float y) : last_cursorX(x), last_cursorY(y) {}

  std::pair<float, float> getCoords() {
    return std::pair<float, float>(last_cursorX, last_cursorY);
  }

  void cursor_callback(GLFWwindow *window, double xpos, double ypos) {
    bool left_action = false;
    bool right_action = false;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
      left_action = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) ==
               GLFW_RELEASE) {
      left_action = false;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
      right_action = true;
    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) ==
               GLFW_RELEASE) {
      right_action = false;
    }

    if (left_action) {
      // std::cout << "Left: (" << xpos << ", " << ypos << ")\n";
      if (first_mouse) {
        last_cursorX = xpos;
        last_cursorY = ypos;
        first_mouse = false;
      }
      float xoffset = xpos - last_cursorX;
      float yoffset = last_cursorY - ypos;

      last_cursorX = xpos;
      last_cursorY = ypos;
    }

    if (right_action) {
      // std::cout << "Right: (" << xpos << ", " << ypos << ")\n";
      if (first_mouse) {
        last_cursorX = xpos;
        last_cursorY = ypos;
        first_mouse = false;
      }
      float xoffset = xpos - last_cursorX;
      float yoffset = last_cursorY - ypos;

      last_cursorX = xpos;
      last_cursorY = ypos;
    }
  }

  void keyboard_callback(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
  }

  void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
  }

  void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    yoffset = yoffset;
  }

private:
};

#endif // INPUT_H_
