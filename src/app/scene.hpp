#ifndef SCENE_H_
#define SCENE_H_

#include "../animation/animation.hpp"
#include "../animation/loader.hpp"
#include "../animation/retargeter.hpp"
#include "../animation/bone.hpp"
#include "../animation/bone_mesh.hpp"
#include "../animation/bvhimporter.hpp"
#include "../core/camera.hpp"
#include "../core/grid.hpp"
#include "../core/models.hpp"
#include "../core/shader.hpp"
#include "../core/ui.hpp"
#include "../core/renderer.hpp"
#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"
#include <chrono>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/dual_quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <string>
#include <thread>

namespace app {

struct SceneCreator {
  SceneCreator() = default;
  virtual ~SceneCreator() = default;

  virtual void initialize() = 0;
  virtual void handleUpdates(int i) = 0;
  virtual std::unique_ptr<core::Camera> &getCam() = 0;
  virtual void render(Shader& shader) = 0;

};

} // namespace app

#endif // SCENE_H_
