#ifndef ENHANCED_H
#define ENHANCED_H
#include "../core/files.hpp"
#include "../core/ui.hpp"
#include "../ui/mapper.hpp"
#include "imgui.h"
#include "scene.hpp"
#include <cstddef>
#include <memory>

namespace app {
struct Enhanced : public Scene {
  Enhanced()
      : cam(std::move(std::make_unique<core::Camera>(20))),
        scene(std::move(std::make_unique<core::Scene>())) {
    this->renderer = std::make_unique<core::Renderer>(this->cam, this->scene);
    initialize();
  }

  virtual void initialize() {
    emptyScene();
    load_animation = false;
    load_model = false;
  }

  std::unique_ptr<core::Camera> &getCam() { return this->cam; }

  virtual void handleUpdates(int *i) {
    if (motion_animation) {
      if (*i >= motion_animation->frames.size()) {
        *i = 0;
      }
      anim::AnimationLoader::setFrame(scene, motion_animation, anim_db, *i);
      if (!map_db.empty()) {
        anim::AnimationRetargetter::retarget(scene, scene->skeletons[1],
                                             scene->skeletons[0], map_db);
        anim::SkeletonTransformation::updateTransforms(scene, 0); // HACK! HACK!
        anim::SkeletonTransformation::updateTransforms(
            // scene, *anim_db[motion_animation.get()]);
            scene, 1);
      }
    }
  }

  virtual void render(Shader &shader) { renderer->render(shader, true); }

  virtual void addUI() {
    animation.navigationUI(".bvh", &load_animation);
    models.navigationUI(".gltf", &load_model);
    if (load_animation) {
      motion_importer =
          std::make_unique<anim::BVHImporter>(animation.selected_file());
      if (fillAnimation())
        load_animation = false;
    }
    if (load_model) {
      model = std::make_unique<core::Model>(models.selected_file());
      model->load();
      model->skeleton.bindTransforms();
      if (fillMesh())
        load_model = false;
    }
    if (model && motion_importer) {
      mapper.mapData(model, motion_importer);
    }

    if (mapper.is_loaded()) {
      std::vector<std::string> source = {};
      std::vector<std::string> target = {};
      for (auto &data : mapper.get_map()) {
        if (data.second == "-")
          continue;
        source.push_back(data.first);
        target.push_back(data.second);
      }
      map_db = anim::AnimationRetargetter::createMap(source, target);
      std::cout << map_db.size() << std::endl;
      mapper.unload();
    }
  }

private:
  std::unique_ptr<core::Scene> scene = nullptr;
  std::unique_ptr<core::Camera> cam = nullptr;
  std::unique_ptr<core::Renderer> renderer = nullptr;
  ui::BoneMapper mapper;

  std::unique_ptr<core::Model> model = nullptr;
  std::unique_ptr<anim::BVHImporter> motion_importer = nullptr;
  std::unique_ptr<anim::Animation> motion_animation = nullptr;

  anim::AnimDatabase anim_db;
  anim::AnimationRetargetter::MapDatabase map_db;

  bool load_animation;
  bool load_model;

  core::Files animation = core::Files("animation");
  core::Files models = core::Files("models");

  void emptyScene() {
    anim_db = anim::AnimDatabase();
    core::SceneManager::addgrid(scene, Grid(20, 1));
  }

  bool fillMesh() {
    if (model) {
      core::SceneManager::flushMesh(scene);
      for (auto &mesh : model->meshes) {
        std::unique_ptr<core::Mesh> m = std::make_unique<core::Mesh>(mesh);
        core::SceneManager::addMesh(scene, m);
      }
      anim::SkeletonTransformation::fillSkeletons(scene, model->skeleton, true);
    }
    return true;
  }

  bool fillAnimation() {
    if (motion_importer) {
      motion_animation = std::move(anim::AnimationLoader::loadMotionData(
          scene, *motion_importer.get(), anim_db));
      anim::AnimationLoader::initialize(scene, motion_animation, anim_db);
    }
    return true;
  }
};
} // namespace app

#endif /* ENHANCED_H */
