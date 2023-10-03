#ifndef CALLBACKS_H
#define CALLBACKS_H

#include <iostream>

#include "GLFW/glfw3.h"
#include "imgui.h"

extern int width, height;

static bool window_drag_active = 0;
static double cursorX, cursorY;

extern double offsetX, offsetY;
extern double zoom;

void glfw_error_callback(int error, const char* description) {
  std::cerr << "[FEHLER] GLFW Fehler: " << error << ", " << description << std::endl;
}

void glfw_framebuffer_size_callback(GLFWwindow* window, int fwidth, int fheight) {
  width = fwidth;
  height = fheight;
  glViewport(0, 0, width, height);
}

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int mods) {
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  if (io.WantCaptureMouse) return;

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
    window_drag_active = true;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    cursorX = x;
    cursorY = y;
  }

  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
    window_drag_active = false;
  }
}

void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  if (io.WantCaptureMouse) return;

    if (window_drag_active) {
        // Calculate the difference in cursor position
        double dx = xpos - cursorX;
        double dy = ypos - cursorY;

        // Scale the offset by the current zoom factor
        offsetX -= dx / zoom;
        offsetY += dy / zoom; // Note the subtraction to invert the y-axis

        // Update cursor position
        cursorX = xpos;
        cursorY = ypos;
  }
}

void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  if (io.WantCaptureMouse) return;

    if (yoffset > 0) {
    zoom *= 1.1;
  } else {
    zoom /= 1.1;
  }
}

#endif  // CALLBACKS_H