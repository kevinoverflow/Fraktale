#include <iostream>

#include "dearimgui.h"
#include "window.h"
#include "shader.h"

static char* programName = "Fraktale";
int width = 1280;
int height = 720;

GLFWwindow* window = nullptr;

static GLuint VAO, VBO, EBO;
static GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,  // A
    1.0f,  -1.0f, 0.0f,  // B
    1.0f,  1.0f,  0.0f,  // C
    -1.0f, 1.0f,  0.0f,  // D
};
static GLint indices[] = {0, 1, 2, 2, 3, 0};

static Shader* shader = new Shader();

double offsetX, offsetY;
double zoom = 1;

int main(int argc, char** argv) {
  if (window_init(width, height, programName) != 0) {
    return 1;
  }
  imgui_init();

  // Vertex Array
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Buffer
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

  // Element Buffer
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Shader
  shader->Init();
  shader->Load(GL_VERTEX_SHADER, "VertexShader.vert");
  shader->Load(GL_FRAGMENT_SHADER, "FragmentShader.frag");
  shader->Link();

  while (!window_should_close()) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);

    shader->Attach();
    shader->SetUniform("u_resolution", (GLfloat)width, (GLfloat)height);
    shader->SetUniform("u_iterations", (GLfloat)50);
    shader->SetUniform("u_offset", (GLfloat)offsetX, (GLfloat)offsetY);
    shader->SetUniform("u_zoom", (GLfloat)zoom);

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