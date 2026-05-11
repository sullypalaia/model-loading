#include <cmath>
#include <iostream>

#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "WindowManager.h"

WindowManager::WindowManager(int width, int height, std::string name,
                             double sensitivity, float speed)
    : m_width(width), m_height(height), m_name(name),
      m_sensitivity(sensitivity), m_speed(speed) {}

int WindowManager::init() {
  m_window = glfwCreateWindow(m_width, m_height, m_name.c_str(), NULL, NULL);
  if (!m_window) {
    std::cerr << "failed to create window\n";
    return 0;
  }

  glfwMakeContextCurrent(m_window);
  gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

  glViewport(0, 0, m_width, m_height);

  glfwSetWindowUserPointer(m_window, this);

  glfwSetKeyCallback(m_window, key_callback);
  glfwSetFramebufferSizeCallback(m_window, resize_callback);
  glfwSetCursorPosCallback(m_window, cursor_pos_callback);

  glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  m_last_time = glfwGetTime();

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

void WindowManager::cursor_pos_callback(GLFWwindow *window, double x,
                                        double y) {
  WindowManager *window_ptr =
      static_cast<WindowManager *>(glfwGetWindowUserPointer(window));

  if (window_ptr->m_last_x == std::numeric_limits<double>::max()) {
    window_ptr->m_last_x = x;
    window_ptr->m_last_y = y;
  }

  double dt = glfwGetTime() - window_ptr->m_last_time;

  window_ptr->m_yaw += (x - window_ptr->m_last_x) * window_ptr->m_sensitivity;
  window_ptr->m_pitch += (window_ptr->m_last_y - y) * window_ptr->m_sensitivity;

  window_ptr->m_last_x = x;
  window_ptr->m_last_y = y;

  if (window_ptr->m_yaw >= 360.0)
    window_ptr->m_yaw -= 360.0;
  if (window_ptr->m_yaw <= -360.0)
    window_ptr->m_yaw = -360.0 - window_ptr->m_yaw;

  if (window_ptr->m_pitch > 89.0)
    window_ptr->m_pitch = 89.0;
  else if (window_ptr->m_pitch < -89.0)
    window_ptr->m_pitch = -89.0;
}

void WindowManager::m_update_size(int width, int height) {
  glViewport(0, 0, width, height);
  m_width = width;
  m_height = height;
}

void WindowManager::m_update() {
  float dt = glfwGetTime() - m_last_time;
  m_last_time = glfwGetTime();

  if (glfwGetKey(m_window, GLFW_KEY_W) == GLFW_PRESS)
    m_pos += m_front * m_speed * dt;
  if (glfwGetKey(m_window, GLFW_KEY_A) == GLFW_PRESS)
    m_pos -= glm::cross(m_front, m_up) * m_speed * dt;
  if (glfwGetKey(m_window, GLFW_KEY_D) == GLFW_PRESS)
    m_pos += glm::cross(m_front, m_up) * m_speed * dt;
  if (glfwGetKey(m_window, GLFW_KEY_S) == GLFW_PRESS)
    m_pos -= m_front * m_speed * dt;
  if (glfwGetKey(m_window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    m_pos -= m_up * m_speed * dt;
  if (glfwGetKey(m_window, GLFW_KEY_SPACE) == GLFW_PRESS)
    m_pos += m_up * m_speed * dt;

  m_front = glm::vec3(
      std::cos(glm::radians(m_pitch)) * std::cos(glm::radians(m_yaw)),
      std::sin(glm::radians(m_pitch)),
      std::cos(glm::radians(m_pitch)) * std::sin(glm::radians(m_yaw)));

  m_view_mat = glm::lookAt(m_pos, m_pos + m_front, m_up);
}
