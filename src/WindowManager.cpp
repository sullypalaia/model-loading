#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "WindowManager.h"

WindowManager::WindowManager(int width, int height, std::string name)
    : m_width(width), m_height(height), m_name(name) {}

int WindowManager::init() {
  m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
  if (!m_window) {
    std::cerr << "failed to create window\n";
    return 0;
  }

  glfwMakeContextCurrent(m_window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glfwSetWindowUserPointer(m_window, this);

  glfwSetKeyCallback(m_window, key_callback);
  glfwSetFramebufferSizeCallback(m_window, resize_callback);

  return 1;
}

GLFWwindow *WindowManager::get_window() { return m_window; }

void WindowManager::destroy_window() { glfwDestroyWindow(m_window); }

void WindowManager::key_callback(GLFWwindow *window, int key, int scancode,
                                 int action, int mods) {
  WindowManager *window_ptr =
      static_cast<WindowManager *>(glfwGetWindowUserPointer(window));
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    std::cout << "closing window...\n";
    glfwSetWindowShouldClose(window_ptr->m_window, true);
  }
}

void WindowManager::resize_callback(GLFWwindow *window, int width, int height) {
  WindowManager *window_ptr =
      static_cast<WindowManager *>(glfwGetWindowUserPointer(window));
  window_ptr->m_update_size(width, height);
}

void WindowManager::m_update_size(int width, int height) {
  glViewport(0, 0, width, height);
  m_width = width;
  m_height = height;
}
