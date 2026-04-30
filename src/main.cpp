#include "glad/glad.h"

#include "GLFW/glfw3.h"

#include "Mesh.h"
#include "WindowManager.h"

void init(WindowManager &window, Mesh &mesh);
void draw(Mesh &mesh);

bool open_window = true;

int main() {
  // window init
  glfwInit();
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

  // create the window
  WindowManager window_manager(640, 480, "model loading");
  Mesh mesh("res/fallout-1-terminal/source/source/fallout_1_terminal.glb");

  init(window_manager, mesh);

  while (!glfwWindowShouldClose(window_manager.get_window())) {
    draw(mesh);

    glfwSwapBuffers(window_manager.get_window());
    glfwPollEvents();
  }

  // clean up
  mesh.destroy();
  window_manager.destroy_window();
}

void init(WindowManager &window, Mesh &mesh) {
  // initialize the window
  if (!window.init()) {
    open_window = false;
    return;
  }
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
