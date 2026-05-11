#pragma once

#include <climits>
#include <string>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "GLFW/glfw3.h"

class WindowManager {
public:
  WindowManager(int width, int height, std::string name, double sensitivity,
                float speed);

  int init();

  GLFWwindow *get_window();

  void destroy_window();

  void m_update();

  int m_width;
  int m_height;

  glm::mat4 m_view_mat;

private:
  GLFWwindow *m_window;

  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void resize_callback(GLFWwindow *window, int width, int height);

  static void cursor_pos_callback(GLFWwindow *window, double x, double y);

  void m_update_size(int width, int height);

  std::string m_name;

  double m_yaw = -90.0;
  double m_pitch;

  double m_last_x = std::numeric_limits<double>::max();
  double m_last_y = std::numeric_limits<double>::max();

  double m_sensitivity;

  glm::vec3 m_front{0.0f, 0.0f, -1.0f};
  glm::vec3 m_up{0.0f, 1.0f, 0.0f};
  glm::vec3 m_pos{0.0f, 0.0f, 3.0f};

  float m_speed;

  float m_last_time;
  float m_dt = 0;
};
