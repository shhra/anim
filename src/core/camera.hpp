#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <random>
#include <vector>

namespace core {

// Defines several possible options for camera movement. Used as abstraction to
// stay away from window-system specific input methods
enum Camera_Movement { FORWARD, BACKWARD, LEFT, RIGHT };

// Default camera values
static const float YAW = -40.0f;
static const float PITCH = -45.0f;
static const float SPEED = 2.5f;
static const float SENSITIVITY = 0.1f;
static const float ZOOM = 45.0f;

// An abstract camera class that processes input and calculates the
// corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera {
public:
  // camera Attributes
  float Radius;
  glm::vec3 Front;
  glm::vec3 Up;
  glm::vec3 Right;
  glm::vec3 WorldUp;
  glm::vec3 Origin;
  // euler Angles
  float Yaw;
  float Pitch;
  // camera options
  float MouseSensitivity;
  float Zoom;

  // Camera update values.
  int new_x, new_y;
  bool is_left_pressed, is_right_pressed;

  // constructor with vectors
  Camera(float radius, glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
         float yaw = YAW, float pitch = PITCH)
      : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MouseSensitivity(SENSITIVITY),
        Zoom(ZOOM) {
    Radius = radius;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    Origin = glm::vec3(0.0f);
    updateCameraVectors();
  }

  // returns the view matrix calculated using Euler Angles and the LookAt Matrix
  glm::mat4 GetViewMatrix() {
    return glm::lookAt(Origin - Front * Radius, Origin, Up);
  }

  // processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction.
  void ProcessMouseMovement(float xoffset, float yoffset,
                            GLboolean constrainPitch = true) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch) {
      if (Pitch > 89.0f)
        Pitch = 89.0f;
      if (Pitch < -89.0f)
        Pitch = -89.0f;
    }

    // update Front, Right and Up Vectors using the updated Euler angles
    updateCameraVectors();
  }

  void ProcessPanMovement(float xoffset, float yoffset) {
    xoffset *= 0.001 * Radius;
    yoffset *= 0.001 * Radius;

    Origin += -xoffset * Right - yoffset * Up;

    updateCameraVectors();
  }

  // processes input received from a mouse scroll-wheel event. Only requires
  // input on the vertical wheel-axis
  void ProcessMouseScroll(float yoffset) {
    auto gaze = glm::normalize(glm::vec3(Radius)) * yoffset;
    Radius =
        (yoffset < 0) ? Radius + glm::length(gaze) : Radius - glm::length(gaze);
    updateCameraVectors();
  }

private:
  void updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
  }
};

struct CameraManager {
  CameraManager(std::shared_ptr<Camera> &cam) : cam(cam) {}

  void setInputs(int x, int y, bool left, bool right) {
    this->cam->new_x = x;
    this->cam->new_y = y;
    this->cam->is_left_pressed = left;
    this->cam->is_right_pressed = left;
  };

  void processInputs(float x, float y, bool pan, bool rotate,
                     bool zoom = false) {
    if (rotate) {
      this->cam->ProcessMouseMovement(x, y);
    }
    if (pan) {
      this->cam->ProcessPanMovement(x, y);
    }
    if (zoom) {
      this->cam->ProcessMouseScroll(y);
    }
  }

private:
  std::shared_ptr<Camera> cam;
};
} // namespace core

#endif // CAMERA_H_
