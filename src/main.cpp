// DONE 1: Create a window
// TODO 2: Load a plane mesh

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/camera.hpp"
#include "core/grid.hpp"
#include "core/models.hpp"
#include "core/shader.hpp"
#include "core/window.hpp"

// #include "animation/animation.hpp"
#include "memory"

#include <utility>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(10);



//   // tell stb_image.h to flip loaded texture's on the y-axis (before loading
//   // model).
//   stbi_set_flip_vertically_on_load(true);

//   // configure global opengl state
//   // -----------------------------
//   glEnable(GL_DEPTH_TEST);

//   // build and compile shaders
//   // -------------------------
//   Shader ourShader("/home/shailesh/Projects/Study/Visualization/src/vis.vert",
//                    "/home/shailesh/Projects/Study/Visualization/src/vis.frag");

//   // load models
//   // -----------
//   Model ourModel("/home/shailesh/Projects/Study/Visualization/assets/bone.obj");

//   // draw in wireframe
//   // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//   // auto bones = Skeleton();

//   // bones.addJoint("start", -1);
//   // bones.addJoint("first", 0);
//   // bones.addJoint("second", 1);

//   // BoneMesh bone = BoneMesh();

//   // Animation anim = Animation();

//   std::vector<std::pair<std::string, int>> data = {
//       std::make_pair("start", -1),
//       std::make_pair("first", 0),
//       std::make_pair("second", 1),
//       std::make_pair("third", 2),
//   };

//   // anim.addJoints(data);

//   std::vector<std::vector<glm::quat>> rotations;
//   for (int i = 0; i < 360; i++) {
//     std::vector<glm::quat> rotation;
//     rotation.push_back(
//         glm::angleAxis(glm::radians(float(0)), glm::vec3(0.f, 1.f, 0.f)));
//     rotation.push_back(
//         glm::angleAxis(glm::radians(float(0)), glm::vec3(0.f, 1.f, 0.f)));
//     rotation.push_back(
//         glm::angleAxis(glm::radians(float(1)), glm::vec3(1.f, 0.f, 0.f)));
//     // rotation.push_back(glm::quat(1.0, 0.0, 0.0, 0.0));
//     // rotation.push_back(glm::quat(1.0, 0.0, 0.0, 0.0));
//     // rotation.push_back(glm::quat(1.0, 0.0, 0.0, 0.0));
//     rotation.push_back(glm::quat(1.0, 0.0, 0.0, 0.0));
//     rotations.push_back(rotation);
//   }

//   std::vector<glm::vec3> positions;
//   positions.push_back(glm::vec3(0.0));
//   positions.push_back(glm::vec3(1.0));
//   positions.push_back(glm::vec3(0.5));
//   positions.push_back(glm::vec3(0.2));

//   // for(auto &rotation: rotations) {
//   //   anim.addFrame(rotation, positions);
//   // }

//   // anim.stats();

//   Grid grid(10, 1);
//   // render loop
//   // -----------
//   while (!glfwWindowShouldClose(window)) {
//     // per-frame time logic
//     // --------------------
//     float currentFrame = glfwGetTime();
//     deltaTime = currentFrame - lastFrame;
//     lastFrame = currentFrame;

//     // input
//     // -----
//     processInput(window);

//     // render
//     // ------
//     glClearColor(0.69f, 0.839f, 0.961f, 1.0f);
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//     // don't forget to enable shader before setting uniforms
//     ourShader.use();

//     // bone.Draw(ourShader);

//     // bones.drawJoints(ourShader, bone);
//     // anim.play(ourShader);

//     // view/projection transformations
//     glm::mat4 projection =
//         glm::perspective(glm::radians(camera.Zoom),
//                          (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//     glm::mat4 view = camera.GetViewMatrix();
//     ourShader.setMat4("projection", projection);
//     ourShader.setMat4("view", view);

//     grid.Draw(ourShader);

//     // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved
//     // etc.)
//     // -------------------------------------------------------------------------------
//     glfwSwapBuffers(window);
//     glfwPollEvents();
//   }

//   // glfw: terminate, clearing all previously allocated GLFW resources.
//   // ------------------------------------------------------------------
//   glfwTerminate();
//   return 0;
// }
//
int main() {

  Window window = Window(800, 600);
  window.initWindow();
  window.createContext();

  while(!glfwWindowShouldClose(window.getContext())){
    window.handleInput();
    window.render();
    window.pollevents();
  }

  glfwTerminate();

  return 0;

}
