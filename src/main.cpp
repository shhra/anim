#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_RADIANS
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
