#include <iostream>

#include "dearimgui.h"
#include "window.h"

GLFWwindow* window = nullptr;

int main(int argc, char** argv) {
  std::cout << "Hello, World!" << std::endl;

  if (window_init(800, 600, "Hello, World!") != 0) {
    return 1;
  }
  imgui_init();

  while (!window_should_close()) {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    imgui_update([]() {
      ImGui::Begin("Hello, World!");
      ImGui::Text("Hello, World!");
      ImGui::End();
    });
    window_update();
  }

  imgui_destroy();
  window_destroy();

  return 0;
}