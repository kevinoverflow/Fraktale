#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#define GLFW_INCLUDE_NONE
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "callbacks.h"

extern GLFWwindow* window;

int window_init(int width, int height, const char* title) {
  glfwSetErrorCallback(glfw_error_callback);

  if (!glfwInit()) {
    std::cerr << "[FEHLER] GLFW konnte nicht initialisiert werden" << std::endl;
    return 1;
  }

  std::cout << "[INFO] GLFW initialisiert" << std::endl;

#ifdef __APPLE__
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  window = glfwCreateWindow(width, height, title, nullptr, nullptr);
  if (!window) {
    std::cerr << "[FEHLER] GLFW Fenster konnte nicht erstellt werden" << std::endl;
    return 1;
  }

  glfwSetFramebufferSizeCallback(window, glfw_framebuffer_size_callback);
  glfwSetMouseButtonCallback(window, glfw_mouse_callback);
  glfwSetCursorPosCallback(window, glfw_cursor_pos_callback);
  glfwSetScrollCallback(window, glfw_scroll_callback);

  glfwMakeContextCurrent(window);
  glfwSwapInterval(1);

  std::cout << "[INFO] OpenGL " << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MAJOR) << "."
            << glfwGetWindowAttrib(window, GLFW_CONTEXT_VERSION_MINOR) << std::endl;

  glewExperimental = GL_TRUE;
  if (GLEW_OK != glewInit()) {
    std::cerr << "[FEHLER] GLEW konnte nicht initialisiert werden" << std::endl;
    glfwTerminate();
    return 1;
  }

  return 0;
}

void window_update() {
  glfwSwapBuffers(window);
  glfwPollEvents();
}

int window_should_close() { return glfwWindowShouldClose(window); }

void window_destroy() {
  glfwDestroyWindow(window);
  glfwTerminate();
}

#endif  // WINDOW_H
