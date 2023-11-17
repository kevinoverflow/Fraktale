#include <complex>
#include <iostream>
#include <vector>

#include "dearimgui.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "misc.h"
#include "shader.h"
#include "window.h"

static char* programName = "Fraktale";
int width = 1280;
int height = 720;
float scale = 1.0f;

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
double zoom = 0.75;

static int fractal = Fractal::Mandelbrot;

static int iterations = 50;

static float juliaReal = -0.4f;
static float juliaImag = 0.8f;

static int colorAlgorithm = ColorAlgorithm::Single;

static float color[3] = {1.0f, 1.0f, 1.0f};

static glm::vec3 a = glm::vec3(0.5f, 0.5f, 0.5f);
static glm::vec3 b = glm::vec3(0.5f, 0.5f, 0.5f);
static glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 d = glm::vec3(0.00f, 0.1, 0.2f);

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

  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  int wwidth;
  glfwGetWindowSize(window, &wwidth, nullptr);
  scale = (float)width / (float)wwidth;

  while (!window_should_close()) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);

    shader->Attach();
    shader->SetUniform("u_resolution", (GLfloat)width, (GLfloat)height);
    shader->SetUniform("u_iterations", (GLfloat)iterations);
    shader->SetUniform("u_offset", (GLfloat)offsetX, (GLfloat)offsetY);
    shader->SetUniform("u_zoom", (GLfloat)zoom);
    shader->SetUniform("u_fractal", (GLfloat)fractal);
    shader->SetUniform("u_color_scheme", (GLfloat)colorAlgorithm);
    shader->SetUniform("u_julia_c", (GLfloat)juliaReal, (GLfloat)juliaImag);
    shader->SetUniform("u_color", (GLfloat)color[0], (GLfloat)color[1], (GLfloat)color[2]);
    shader->SetUniform("u_a", a.x, a.y, a.z);
    shader->SetUniform("u_b", b.x, b.y, b.z);
    shader->SetUniform("u_c", c.x, c.y, c.z);
    shader->SetUniform("u_d", d.x, d.y, d.z);

    imgui_update([]() {
      ImGui::Begin("Einstellungen");

      ImGui::SeparatorText("Position");
      ImGui::Text("Mittelpunkt %.2f%+.2fi", mapToComplex(offsetX, offsetY).real(),
                  mapToComplex(offsetX, offsetY).imag());

      ImGui::Text("Zoom: %.2f", zoom);
      ImGui::Button("Zurücksetzen");
      if (ImGui::IsItemClicked()) {
        offsetX = 0.0;
        offsetY = 0.0;
        zoom = 0.75;
      }

      ImGui::SeparatorText("Render");
      const char* fractalTypes[] = {"Mandelbrot", "Julia", "Burning Ship"};
      ImGui::Combo("Fraktal", &fractal, fractalTypes, IM_ARRAYSIZE(fractalTypes));

      if (fractal == Fractal::Julia) {
        ImGui::SliderFloat("Realteil", &juliaReal, -2.0f, 2.0f);
        ImGui::SliderFloat("Imaginärteil", &juliaImag, -2.0f, 2.0f);
      }

      ImGui::SliderInt("Iterationen", &iterations, 1, 256);

      ImGui::SeparatorText("Farbe");
      ImGui::RadioButton("Einfarbig", &colorAlgorithm, ColorAlgorithm::Single);
      ImGui::SameLine();
      ImGui::RadioButton("Farbverlauf", &colorAlgorithm, ColorAlgorithm::Gradient);

      if (colorAlgorithm == ColorAlgorithm::Single) {
        ImGui::ColorPicker3("Farbe", color, 0);
      }
      if (colorAlgorithm == ColorAlgorithm::Gradient) {
        ImGui::DrawGradientRect(a, b, c, d, 200, 30, 100);
        ImGui::Dummy(ImVec2(0.0f, 30.0f));
        ImGui::VectorTable(&a, &b, &c, &d);
      }

      ImGui::SeparatorText("Info");
      ImGui::Text("FPS: %.2f", ImGui::GetIO().Framerate);
      ImGui::Text("Auflösung: %dx%d@%.2fx", width, height, scale);

      // platform info
      ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));
      ImGui::Text("OpenGL Version: %s", glGetString(GL_VERSION));

      ImGui::SeparatorText("Über");
      ImGui::TextWrapped(
          "Fraktale ist ein interaktives Programm, um Fraktale zu visualisieren und zu "
          "erkunden.");
      ImGui::Separator();
      ImGui::Text("Entwickelt von: Kevin Hoàng");
      ImGui::Text("Version: 1.0.0");

      ImGui::Button("Quellcode");
      if (ImGui::IsItemClicked()) {
        OsOpenInShell("https://www.github.com/kevinoverflow/Fraktale");
      }

      ImGui::End();
    });

    window_update();
  }

  imgui_destroy();
  window_destroy();

  return 0;
}