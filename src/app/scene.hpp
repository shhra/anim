#ifndef SCENE_H_
#define SCENE_H_

#include "../animation/bone.hpp"
#include "../animation/bone_mesh.hpp"
#include "../core/camera.hpp"
#include "../core/grid.hpp"
#include "../core/input.hpp"
#include "../core/shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

class Scene {
public:
  Scene() { cam = Camera(10); }

  Scene(const char *vert_shader, const char *frag_shader) {
    cam = Camera(10);
    shader = Shader(vert_shader, frag_shader);
    shader.use();
    grid = Grid(10, 1);
    init();
  }

  //! Initializes the scene resources.
  void init() {
    bone = BoneMesh();
    bones = Skeleton();
    bones.addJoint("start", -1);
    bones.addJoint("first", 0);
    bones.addJoint("second", 1);

    bones.setTransforms(
        0, glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::vec3(0.0));

    bones.setTransforms(
        1, glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::vec3(2.0));

    bones.setTransforms(
        2, glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::vec3(1.0, 1.0, 1.0));
    bones.setWorldTransforms();
  }

  void render(float screen_width, float screen_height) {
    glm::mat4 projection = glm::perspective(
        glm::radians(cam.Zoom), screen_width / screen_height, 0.1f, 100.0f);
    glm::mat4 view = cam.GetViewMatrix();
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    grid.Draw(shader);
    bones.drawJoints(shader, bone);
  }

  void processInputs(float x, float y, bool pan, bool rotate) {
    // This is inefficient but anyway.
    if (rotate) {
      cam.ProcessMouseMovement(x, y);
    }
    if (pan) {
      cam.ProcessPanMovement(x, y);
    }
  }

private:
  Camera cam;
  Shader shader;
  Grid grid;
  Skeleton bones;
  BoneMesh bone;
};
#endif // SCENE_H_
