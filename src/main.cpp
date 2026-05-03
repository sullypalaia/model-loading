#include "glad/glad.h"
#include <iostream>

#include "GLFW/glfw3.h"

#include "Mesh.h"
#include "WindowManager.h"

void APIENTRY error_callback(GLenum source, GLenum type, GLuint id,
                             GLenum severity, GLsizei length,
                             const GLchar *message, const void *userParam) {
  if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
    return;

  std::cerr << "opengl error " << type << ", severity " << severity
            << ", message: " << message << '\n';
}

void init(Mesh &mesh);
void draw(Mesh &mesh);

bool open_window = true;

int main() {
  // window init
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);

  // create the window
  WindowManager window_manager(640, 480, "model loading");
  if (!window_manager.init()) {
    std::cerr << "window initialization failed\n";
    return -1;
  }

  // set up debugging
  glEnable(GL_DEBUG_OUTPUT);
  glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
  glDebugMessageCallback(error_callback, nullptr);

  Mesh mesh("fallout-1-terminal/source/fallout_1_terminal.glb");

  init(mesh);

  while (!glfwWindowShouldClose(window_manager.get_window())) {
    draw(mesh);

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean up
  mesh.destroy();
  window_manager.destroy_window();
}

void init(Mesh &mesh) {
  if (!mesh.init()) {
    open_window = false;
    return;
  }
}

void draw(Mesh &mesh) {
  constexpr GLfloat clear_color[]{0.2f, 0.8f, 0.8f, 1.0f};
  glClearBufferfv(GL_COLOR, 0, clear_color);

  mesh.draw();
}
