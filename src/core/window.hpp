#ifndef WINDOW_H_
#define WINDOW_H_

#include "camera.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "ui.hpp"
#include <memory>

namespace core {

// Responsible for handling all window related business.
class Window {
public:
  Window(unsigned int width, unsigned int height)
      : screen_width(width), screen_height(height) {
    deltaTime = 0.0f;
    lastFrame = 0.0f;
  }

  void initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    window = glfwCreateWindow(screen_width, screen_height, "explorer", nullptr,
                              nullptr);
    if (window == nullptr) {
      std::cout << "Failed to create GLFW window" << std::endl;
      glfwTerminate();
    }
    // glEnable(GL_DEPTH_TEST);
  }
  void createContext() {
    glfwMakeContextCurrent(window);
    // glfwSetFramebufferSizeCallback(window, inputs.mo);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "Failed to initialize GLAD" << std::endl;
      glfwTerminate();
    }

    glEnable(GL_DEPTH_TEST);
  }

  // void postContext(Scene *scene) { input.get()->setScene(scene); }

  GLFWwindow *getContext() { return this->window; }

  void clear() {
    glClearColor(0.69f, 0.839f, 0.961f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void swap() {
    glfwSwapBuffers(window);
  }

  void pollevents() {
    // Call the other functions here.
    glfwPollEvents();
  }

  void handleInput(Input *input) { input->keyboard_callback(window); }

  void terminate() { glfwTerminate(); }

  void registerCallbacks(Input *input) {
    glfwSetWindowUserPointer(window, input);
#define registerCallback(functionName)                                         \
  [](GLFWwindow *window, auto... args) {                                       \
    auto pointer = static_cast<Input *>(glfwGetWindowUserPointer(window));     \
    if (pointer)                                                               \
      pointer->functionName(window, args...);                                  \
  }
    glfwSetFramebufferSizeCallback(window,
                                   registerCallback(framebuffer_size_callback));
    glfwSetCursorPosCallback(window, registerCallback(cursor_callback));
    glfwSetScrollCallback(window, registerCallback(scroll_callback));
    // #undef registerCallback
  }

private:
  GLFWwindow *window;
  unsigned int screen_width, screen_height;
  float deltaTime;
  float lastFrame;
};

} // namespace core

#endif // WINDOW_H_
