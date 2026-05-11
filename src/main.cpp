#include "glad/glad.h"
#include <iostream>

#include "GLFW/glfw3.h"

#include "Mesh.h"
#include "UBO.h"
#include "WindowManager.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

struct Transform {
  glm::mat4 projection;
  glm::mat4 view;
};

void error_callback(GLenum source, GLenum type, GLuint id, GLenum severity,
                    GLsizei length, const GLchar *message,
                    const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;

  std::cerr << "opengl error " << type << ", severity " << severity
            << ", message: " << message << '\n';
}

void init(Mesh &mesh);
void draw(Mesh &mesh, WindowManager &window, UBO<Transform> &ubo);

bool open_window = true;

int main() {
  // window init
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // create the window
  WindowManager window_manager(1920, 1080, "model loading", 0.1, 1.0f);
  if (!window_manager.init()) {
    std::cerr << "window initialization failed\n";
    return -1;
  }

  // set up debugging
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(error_callback, nullptr);

  // Mesh mesh("fallout-1-terminal/source/fallout_1_terminal.glb");
  Mesh mesh("croissant.glb");
  UBO<Transform> ubo;

  init(mesh);

  while (!glfwWindowShouldClose(window_manager.get_window())) {
    draw(mesh, window_manager, ubo);

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean up
  mesh.destroy();
  ubo.destroy();
  window_manager.destroy_window();
}

void init(Mesh &mesh) {
  // enable stuff
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  if (!mesh.init()) {
    open_window = false;
    return;
  }

  // transform the model
  float scale_factor = 10.0;

  glm::mat4 model(1.0f);
  model = glm::rotate(model, glm::radians(180.0f), glm::vec3(1.0f, 0.0f, 1.0f));
  model = glm::scale(model, glm::vec3(scale_factor));

  mesh.set_uniform_matrix("model", model);
}

void draw(Mesh &mesh, WindowManager &window, UBO<Transform> &ubo) {
  constexpr GLfloat clear_color[]{0.2f, 0.8f, 0.8f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, clear_color);

  constexpr GLfloat clear_depth{1.0f};
  glClearBufferfv(GL_DEPTH, 0, &clear_depth);

  // create the uniform buffer
  glm::mat4 proj = glm::perspective(glm::radians(45.0f),
                                    static_cast<float>(window.m_width) /
                                        static_cast<float>(window.m_height),
                                    0.1f, 100.0f);

  window.m_update();

  ubo.bind();
  ubo.update(0, sizeof(glm::mat4), glm::value_ptr(proj));
  ubo.update(sizeof(glm::mat4), sizeof(glm::mat4),
             glm::value_ptr(window.m_view_mat));

  mesh.draw();
}
