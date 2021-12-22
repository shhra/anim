#ifndef SCENE_H_
#define SCENE_H_

#include "../animation/animation.hpp"
#include "../animation/bone.hpp"
#include "../animation/bone_mesh.hpp"
#include "../animation/bvhimporter.hpp"
#include "../core/camera.hpp"
#include "../core/grid.hpp"
#include "../core/input.hpp"
#include "../core/shader.hpp"
#include "../core/ui.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <chrono>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <thread>

class Scene {
public:
  Scene() { cam = Camera(10); }

  Scene(const char *vert_shader, const char *frag_shader) {
    cam = Camera(5);
    shader = Shader(vert_shader, frag_shader);
    shader.use();
    grid = Grid(10, 1);
    init();
  }

  //! Initializes the scene resources.
  void init() {

    anim = Animation();

    auto data = std::vector<std::pair<std::string, int>>{
        std::pair<std::string, int>("root", -1),
        std::pair<std::string, int>("first", 0),
        std::pair<std::string, int>("second", 1),
        std::pair<std::string, int>("third", 2),
    };

    anim.addJoints(data);

    auto rotations = std::vector<glm::quat>{
        glm::angleAxis(glm::radians(45.0f),
                       glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f))),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        glm::angleAxis(glm::radians(0.0f), glm::vec3(1.0f, 0.0f, 0.0f))};

    auto positions = std::vector<glm::vec3>{
        glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 2.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f), 0.5f * glm::vec3(0.0f, 1.0f, 0.0f)};

    for (int j = 1; j <= 1; j++) {
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
        anim.addFrame(anim_rotations, glm::vec3(0.01f));
      } else {
        anim.addFrame(anim_rotations, glm::vec3(-0.01f));
      }
    }

    anim.initRestFrame(rotations, positions);
    anim.setRetargeter();
    anim.bind();
    // this->data.animation.setInit();
    // this->data.animation.setRetargeter();
    // this->data.animation.skeleton.log();

    // model.load();
    // model.skeleton.bindTransforms();
    // model.skeleton.log();

    std::vector<std::string> source = {
        "Hips",         "LeftUpLeg",    "LeftLeg",  "LeftFoot",
        "LeftToeBase",  "RightUpLeg",   "RightLeg", "RightFoot",
        "RightToeBase", "LowerBack",    "Spine",    "Spine1",
        "Neck",         "Neck1",        "Head",     "LeftShoulder",
        "LeftArm",      "LeftForeArm",  "LeftHand", "RightShoulder",
        "RightArm",     "RightForeArm", "RightHand"};

    std::vector<std::string> target = {
        "mixamorig:Hips",         "mixamorig:LeftUpLeg",
        "mixamorig:LeftLeg",      "mixamorig:LeftFoot",
        "mixamorig:LeftToeBase",  "mixamorig:RightUpLeg",
        "mixamorig:RightLeg",     "mixamorig:RightFoot",
        "mixamorig:RightToeBase", "mixamorig:LowerBack",
        "mixamorig:Spine",        "mixamorig:Spine1",
        "mixamorig:Spine2",       "mixamorig:Neck",
        "mixamorig:Head",         "mixamorig:LeftShoulder",
        "mixamorig:LeftArm",      "mixamorig:LeftForeArm",
        "mixamorig:LeftHand",     "mixamorig:RightShoulder",
        "mixamorig:RightArm",     "mixamorig:RightForeArm",
        "mixamorig:RightHand"};
    // this->data.animation.setMap(source, target);
  }

  void render(float screen_width, float screen_height, Ui &ui) {
    glm::mat4 projection = glm::perspective(
        glm::radians(cam.Zoom), screen_width / screen_height, 0.1f, 100.0f);
    glm::mat4 view = cam.GetViewMatrix();
    shader.setBool("is_skin", false);
    shader.setMat4("projection", projection);
    shader.setMat4("view", view);
    auto unit = glm::mat3(1.0f);

    grid.Draw(shader);
    // data.animation.play(shader, &model.skeleton);
    // anim.play(shader, &model.skeleton);
    anim.play(shader, nullptr);

    // Cast a ray here.
    glm::vec3 ray = raycast();

    // Print the ray.
    std::cout << "Ray: " << ray.x << " " << ray.y << " " << ray.z << std::endl;

    // TODO: Select the objects here.

    anim.skeleton.drawJoints(shader, bone);
    // data.animation.skeleton.drawJoints(shader, bone);
    // model.skeleton.drawJoints(shader, bone);
    shader.setBool("is_skin", true);
    // model.skeleton.bindUniforms(shader);
    // model.Draw(shader);

    // auto joint = model.skeleton.get_joint(0);
    // std::string name = joint.name + " rotation: (" +
    //                    std::to_string(joint.worldTransform.rotation.w) +
    //                    std::to_string(joint.worldTransform.rotation.x) +
    //                    std::to_string(joint.worldTransform.rotation.y) +
    //                    std::to_string(joint.worldTransform.rotation.z) + ")";
    // ui.addString(name);
  }

  void processInputs(float x, float y, bool pan, bool rotate,
                     bool zoom = false) {
    if (rotate) {
      cam.ProcessMouseMovement(x, y);
    }
    if (pan) {
      cam.ProcessPanMovement(x, y);
    }
    if (zoom) {
      cam.ProcessMouseScroll(y);
    }
  }

  glm::vec3 raycast() {
    // Print the mouse position in normalized device coordinates
    float x = (this->x / (float)1280) * 2.0f - 1.0f;
    float y = (this->y / (float)720) * 2.0f - 1.0f;

    // Homogenous ray origin
    glm::vec4 r_near = glm::vec4(x, y, -1.f, 1.f);
    glm::mat4 projection = glm::perspective(glm::radians(cam.Zoom),
                                            1280.0f / 720.0f, 0.1f, 100.0f);

    // Camera space
    glm::vec4 r_cam = glm::inverse(projection) * r_near;
    r_cam = glm::vec4(r_cam.x, r_cam.y, -1.0f, 0.0f);

    glm::mat4 view = cam.GetViewMatrix();
    glm::vec3 r_world = glm::vec3(glm::inverse(view) * r_cam);
    return glm::normalize(r_world);
  }

  void setInputs(float x, float y, bool left, bool right) {
    this->x = x;
    this->y = x;
    this->left = left;
    this->right = right;
  }

private:
  Camera cam;
  Shader shader;
  Grid grid;
  Animation anim;
  BoneMesh bone = BoneMesh();
  // Model model =
  // Model("/home/shailesh/Projects/Study/Visualization/assets/vegeta.gltf");
  // Model("/home/shailesh/Projects/Study/Visualization/assets/anim.gltf");
  // Model("/home/shailesh/Projects/Study/Visualization/assets/RiggedFigure.gltf");
  //
  // BVHImporter data =
  //     BVHImporter("/home/shailesh/Projects/Study/PFNN/pfnn/data/animations/"
  //                 "LocomotionFlat02_000.bvh");
  float x, y;
  bool left, right;
};
#endif // SCENE_H_
