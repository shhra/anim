// DONE 1: Create a window
// TODO 2: Load a plane mesh

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core/camera.hpp"
#include "core/grid.hpp"
#include "core/models.hpp"
#include "core/shader.hpp"
#include "core/window.hpp"

#include "app/scene.hpp"

// #include "animation/animation.hpp"
#include "memory"

#include <utility>

#include <iostream>

int main() {

  Window window = Window(800, 600);

  // glm::quat q1 = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  // auto out = q1 * glm::vec3(1.0, 0.0, 0.0);

  // std::cout << glm::to_string(out) << std::endl;


  window.initWindow();
  window.createContext();
  Scene scene("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
              "/home/shailesh/Projects/Study/Visualization/src/vis.frag");

  window.postContext(&scene);
  while(!glfwWindowShouldClose(window.getContext())){
    window.handleInput();
    window.render(scene);
    window.pollevents(scene);
  }

  glfwTerminate();

  return 0;

}
