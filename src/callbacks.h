#ifndef CALLBACKS_H
#define CALLBACKS_H

#include "GLFW/glfw3.h"

void glfw_error_callback(int error, const char* description) {
  std::cerr << "[FEHLER] GLFW Fehler: " << error << ", " << description << std::endl;
}

void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height) {}

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {}

#endif  // CALLBACKS_H