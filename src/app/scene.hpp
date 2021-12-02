#ifndef SCENE_H_
#define SCENE_H_

#include "../animation/animation.hpp"
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

    anim = Animation();

    auto data = std::vector<std::pair<string, int>>{
        std::pair<string, int>("root", -1),
        std::pair<string, int>("first", 0),
        std::pair<string, int>("second", 1),
        std::pair<string, int>("third", 2),
    };

    anim.addJoints(data);

    auto rotations = std::vector<glm::quat>{
        glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))};

    auto positions = std::vector<glm::vec3>{
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2.0f, 2.0f, 2.0f),
        glm::vec3(1.0f, 1.0f, 1.0f), 0.5f * glm::vec3(1.0f, 1.0f, 1.0f)};

    for (int j = 1; j <= 360; j++) {
      auto anim_rotations = std::vector<glm::quat>{};
      auto r1 =
          glm::angleAxis(glm::radians(float(0)), glm::vec3(0.0f, 1.0f, 0.0f));
      auto r2 = glm::angleAxis(glm::radians(float(3)),
                               glm::normalize(glm::vec3(1.0)));
      auto r3 =
          glm::angleAxis(glm::radians(float(0)), glm::vec3(0.0f, 1.0f, 0.0f));
      auto r4 =
          glm::angleAxis(glm::radians(float(0)), glm::vec3(1.0f, 0.0f, 0.0f));
      anim_rotations.push_back(r1);
      anim_rotations.push_back(r2);
      anim_rotations.push_back(r3);
      if (j < 180) {
        anim.addFrame(anim_rotations, glm::vec3(0.00f));
      } else {
        anim.addFrame(anim_rotations, glm::vec3(-0.00f));
      }
    }

    anim.initRestFrame(rotations, positions);
    anim.setRetargeter();
    anim.bind();
    model.load();
    model.skeleton.bindTransforms();
    model.skeleton.log();
  }

  void render(float screen_width, float screen_height) {
    glm::mat4 projection = glm::perspective(
        glm::radians(cam.Zoom), screen_width / screen_height, 0.1f, 100.0f);
    glm::mat4 view = cam.GetViewMatrix();
    shader.setBool("is_skin", false);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    auto unit = glm::mat3(1.0f);
    grid.Draw(shader);
    anim.play(shader, &model.skeleton);
    model.skeleton.drawJoints(shader, bone);
    shader.setBool("is_skin", true);
    model.skeleton.bindUniforms(shader);
    model.Draw(shader);
  }

  void processInputs(float x, float y, bool pan, bool rotate) {
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
  Animation anim;
  BoneMesh bone = BoneMesh();
  Model model =
      // Model("/home/shailesh/Projects/Study/Visualization/assets/anim.gltf");
      Model("/home/shailesh/Projects/Study/Visualization/assets/RiggedFigure.gltf");
};
#endif // SCENE_H_
