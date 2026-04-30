#pragma once

#include <string>

#include "GLFW/glfw3.h"

class WindowManager {
public:
  WindowManager(int width, int height, std::string name);

  int init();

  GLFWwindow *get_window();

  void destroy_window();

private:
  GLFWwindow *m_window;

  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
  static void resize_callback(GLFWwindow *window, int width, int height);

  int m_width;
  int m_height;
  std::string m_name;
};
