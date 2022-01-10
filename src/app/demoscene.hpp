#ifndef DEMOSCENE_H_
#define DEMOSCENE_H_

#include "scene.hpp"
#include <memory>

// TODO: Make this demoscene read a config file!
namespace app {

struct DemoScene : public SceneCreator {
public:
  DemoScene()
      : cam(std::move(std::make_unique<core::Camera>(20))),
        scene(std::move(std::make_unique<core::Scene>())) {
    initialize();
  }

  virtual void initialize() {
    loadData();
    fillScene();
  }

  virtual void handleUpdates(int i) {
    if (i >= motion_animation->frames.size()) {
      i = 0;
    }
    anim::AnimationLoader::setFrame(scene, motion_animation, anim_db, i);
    anim::AnimationRetargetter::retarget(scene, scene->skeletons[1],
                                         scene->skeletons[0], map_db);
    anim::SkeletonTransformation::updateTransforms(scene, 0);
    anim::SkeletonTransformation::updateTransforms(scene, 1);
  }

  std::unique_ptr<core::Camera> &getCam() { return this->cam; }

  virtual void render(Shader &shader) { renderer.render(shader); }

private:
  anim::AnimationRetargetter::MapDatabase map_db;
  anim::AnimDatabase anim_db;
  anim::BVHImporter motion_data;
  std::unique_ptr<anim::Animation> motion_animation;
  std::unique_ptr<core::Scene> scene = nullptr;
  std::unique_ptr<core::Camera> cam = nullptr;
  core::Model model = core::Model(
      "/home/shailesh/Projects/Study/Visualization/assets/vegeta.gltf");
  core::Renderer renderer = core::Renderer(cam, scene);

  void loadData() {
    std::vector<std::string> source = {
        "Hips",         "LeftUpLeg",  "LeftLeg",       "LeftFoot",
        "LeftToeBase",  "RightUpLeg", "RightLeg",      "RightFoot",
        "RightToeBase", "Spine",      "Spine1",        "Neck",
        "Neck1",        "Head",       "LeftShoulder",  "LeftArm",
        "LeftForeArm",  "LeftHand",   "RightShoulder", "RightArm",
        "RightForeArm", "RightHand"};

    std::vector<std::string> target = {
        "mixamorig:Hips",          "mixamorig:LeftUpLeg",
        "mixamorig:LeftLeg",       "mixamorig:LeftFoot",
        "mixamorig:LeftToeBase",   "mixamorig:RightUpLeg",
        "mixamorig:RightLeg",      "mixamorig:RightFoot",
        "mixamorig:RightToeBase",  "mixamorig:Spine",
        "mixamorig:Spine1",        "mixamorig:Spine2",
        "mixamorig:Neck",          "mixamorig:Head",
        "mixamorig:LeftShoulder",  "mixamorig:LeftArm",
        "mixamorig:LeftForeArm",   "mixamorig:LeftHand",
        "mixamorig:RightShoulder", "mixamorig:RightArm",
        "mixamorig:RightForeArm",  "mixamorig:RightHand"};

    map_db = anim::AnimationRetargetter::createMap(source, target);
    anim_db = anim::AnimDatabase();

    model.load();
    model.skeleton.bindTransforms(); // << this will be removed.
    motion_data = anim::BVHImporter(
        "/home/shailesh/Projects/Study/PFNN/pfnn/data/animations/"
        "LocomotionFlat02_000.bvh");
  }

  void fillScene() {
    for (auto &mesh : model.meshes) {
      std::unique_ptr<core::Mesh> m = std::make_unique<core::Mesh>(mesh);
      core::SceneManager::addMesh(scene, m);
    }

    anim::SkeletonTransformation::fillSkeletons(scene, model.skeleton);
    core::SceneManager::addgrid(scene, Grid(20, 1));
    motion_animation = std::move(
        anim::AnimationLoader::loadMotionData(scene, motion_data, anim_db));
    anim::AnimationLoader::initialize(scene, motion_animation, anim_db);
  }
};
} // namespace app

#endif // DEMOSCENE_H_
