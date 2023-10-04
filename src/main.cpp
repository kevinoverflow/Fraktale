#include <iostream>
#include <vector>

#include "dearimgui.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "imgui_internal.h"
#include "shader.h"
#include "types.h"
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

static int iterations = 50;
static float color[3] = {1.0f, 1.0f, 1.0f};

// turn values into glm vectors
static glm::vec3 a = glm::vec3(0.5f, 0.5f, 0.5f);
static glm::vec3 b = glm::vec3(0.5f, 0.5f, 0.5f);
static glm::vec3 c = glm::vec3(1.0f, 1.0f, 1.0f);
static glm::vec3 d = glm::vec3(0.00f, 0.1, 0.2f);

// static std::vector<glm::vec3> values = {a, b, c, d};
static float values[4][3] = {{a.x, a.y, a.z}, {b.x, b.y, b.z}, {c.x, c.y, c.z}, {d.x, d.y, d.z}};

// float array for imgui with the same values as the glm vectors

ImU32 CalculateColor(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d,
                     float t) {
  glm::vec3 color;
  color.x = a.x + b.x * cos(2 * 3.14159265359f * (c.x * t + d.x));
  color.y = a.y + b.y * cos(2 * 3.14159265359f * (c.y * t + d.y));
  color.z = a.z + b.z * cos(2 * 3.14159265359f * (c.z * t + d.z));
  color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));  // Clamp to the range [0, 1]
  return ImGui::ColorConvertFloat4ToU32(ImVec4(color.x, color.y, color.z, 1.0f));
}

void DrawGradientRectWithFunction(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
                                  const glm::vec3& d, float gradientWidth, float gradientHeight,
                                  int numSamples) {
  ImGuiWindow* window = ImGui::GetCurrentWindow();
  if (window->SkipItems) return;

  ImVec2 pos = window->DC.CursorPos;
  ImVec2 size(gradientWidth, gradientHeight);

  // Calculate the step size for the gradient
  float stepSize = gradientWidth / static_cast<float>(numSamples);

  // Draw the gradient by rendering horizontal lines with different colors
  for (int i = 0; i < numSamples; ++i) {
    float t = static_cast<float>(i) / static_cast<float>(numSamples - 1);
    ImU32 color = CalculateColor(a, b, c, d, t);
    ImVec2 lineStart(pos.x + i * stepSize, pos.y);
    ImVec2 lineEnd(pos.x + (i + 1) * stepSize, pos.y + gradientHeight);
    window->DrawList->AddRectFilledMultiColor(lineStart, lineEnd, color, color, color, color);
  }

  if (ImGui::IsItemHovered())
    ImGui::SetTooltip(
        "Gradient Function: color(t) = (%.2f, %.2f, %.2f) + (%.2f, %.2f, %.2f) * cos[2π(%.2f, "
        "%.2f, %.2f) * t + (%.2f, %.2f, %.2f)]",
        a.x, a.y, a.z, b.x, b.y, b.z, c.x, c.y, c.z, d.x, d.y, d.z);
}

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
    shader->SetUniform("u_color", (GLfloat)color[0], (GLfloat)color[1], (GLfloat)color[2]);

    shader->SetUniform("u_a", a.x, a.y, a.z);
    shader->SetUniform("u_b", b.x, b.y, b.z);
    shader->SetUniform("u_c", c.x, c.y, c.z);
    shader->SetUniform("u_d", d.x, d.y, d.z);

    imgui_update([]() {
      ImGui::ShowDemoWindow();

      ImGui::Begin("Einstellungen");
      ImGui::SeparatorText("Render");
      ImGui::SliderInt("Iterationen", &iterations, 1, 256);
      ImGui::SeparatorText("Farbe");

      static int v = ColorAlgorithm::Single;
      ImGui::RadioButton("Einfarbig", &v, ColorAlgorithm::Single);
      ImGui::SameLine();
      ImGui::RadioButton("Farbverlauf", &v, ColorAlgorithm::Gradient);

      if (v == ColorAlgorithm::Single) {
        ImGui::ColorPicker3("Farbe", color, 0);
      }
      if (v == ColorAlgorithm::Gradient) {
        // Draw the gradient function with the following coefficients: [[0.731 1.098 0.192]
        // [0.358 1.090 0.657] [1.077 0.360 0.328] [0.965 2.265 0.837]]
        DrawGradientRectWithFunction(a, b, c, d, 200, 30, 100);
        /*DrawGradientRectWithFunction(
            glm::vec3(0.731f, 1.098f, 0.192f), glm::vec3(0.358f, 1.090f, 0.657f),
            glm::vec3(1.077f, 0.360f, 0.328f), glm::vec3(0.965f, 2.265f, 0.837f), 200, 30, 100);
        */
        ImGui::Dummy(ImVec2(0.0f, 30.0f));
        /*DrawGradientRectWithFunction(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
                                     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, 0.0f), 200,
                                     200, 100);*/

        // Create labels for the vectors

        // Begin a table with 4 columns
        if (ImGui::BeginTable("VectorTable", 4)) {
          // Table headers
          ImGui::TableSetupColumn("Vector Name", ImGuiTableColumnFlags_None);
          ImGui::TableSetupColumn("X", ImGuiTableColumnFlags_None);
          ImGui::TableSetupColumn("Y", ImGuiTableColumnFlags_None);
          ImGui::TableSetupColumn("Z", ImGuiTableColumnFlags_None);
          ImGui::TableHeadersRow();

          // Row for vector 'a'
          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("a");
          ImGui::TableSetColumnIndex(1);
          ImGui::InputFloat("##a_x", &a.x);
          ImGui::TableSetColumnIndex(2);
          ImGui::InputFloat("##a_y", &a.y);
          ImGui::TableSetColumnIndex(3);
          ImGui::InputFloat("##a_z", &a.z);

          // Repeat the above for vectors 'b', 'c', and 'd'

          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("b");
          ImGui::TableSetColumnIndex(1);
          ImGui::InputFloat("##b_x", &b.x);
          ImGui::TableSetColumnIndex(2);
          ImGui::InputFloat("##b_y", &b.y);
          ImGui::TableSetColumnIndex(3);
          ImGui::InputFloat("##b_z", &b.z);

          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("c");
          ImGui::TableSetColumnIndex(1);
          ImGui::InputFloat("##c_x", &c.x);
          ImGui::TableSetColumnIndex(2);
          ImGui::InputFloat("##c_y", &c.y);
          ImGui::TableSetColumnIndex(3);
          ImGui::InputFloat("##c_z", &c.z);

          ImGui::TableNextRow();
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("d");
          ImGui::TableSetColumnIndex(1);
          ImGui::InputFloat("##d_x", &d.x);
          ImGui::TableSetColumnIndex(2);
          ImGui::InputFloat("##d_y", &d.y);
          ImGui::TableSetColumnIndex(3);
          ImGui::InputFloat("##d_z", &d.z);

          // End the table
          ImGui::EndTable();
        }
      }
      ImGui::End();
    });

    window_update();
  }

  imgui_destroy();
  window_destroy();

  return 0;
}