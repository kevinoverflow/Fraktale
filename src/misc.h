#ifndef MISC_H
#define MISC_H

#include <complex>

#include "glm/glm.hpp"
#include "imgui_internal.h"

extern int width;
extern int height;

std::complex<double> mapToComplex(double offsetX, double offsetY) {
  double real = 1 + (2.0 * offsetX - width) / height;
  double imag = 1 + (2.0 * offsetY - height) / height;

  return std::complex<double>(real, imag);
}

enum Fractal { Mandelbrot = 0, Julia = 1, BurningShip = 2 };

enum ColorAlgorithm { Single = 0, Gradient = 1 };

namespace ImGui {
ImU32 CalculateColor(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c, const glm::vec3& d,
                     float t) {
  glm::vec3 color;
  color.x = a.x + b.x * cos(2 * 3.14159265359f * (c.x * t + d.x));
  color.y = a.y + b.y * cos(2 * 3.14159265359f * (c.y * t + d.y));
  color.z = a.z + b.z * cos(2 * 3.14159265359f * (c.z * t + d.z));
  color = glm::clamp(color, glm::vec3(0.0f), glm::vec3(1.0f));  // Clamp to the range [0, 1]
  return ImGui::ColorConvertFloat4ToU32(ImVec4(color.x, color.y, color.z, 1.0f));
}

void DrawGradientRect(const glm::vec3& a, const glm::vec3& b, const glm::vec3& c,
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
}

void VectorTable(glm::vec3* a, glm::vec3* b, glm::vec3* c, glm::vec3* d) {
  if (ImGui::BeginTable("VectorTable", 4)) {
    // Table headers
    ImGui::TableSetupColumn("Parameter", ImGuiTableColumnFlags_None);
    ImGui::TableSetupColumn("Rot", ImGuiTableColumnFlags_None);
    ImGui::TableSetupColumn("Grün", ImGuiTableColumnFlags_None);
    ImGui::TableSetupColumn("Blau", ImGuiTableColumnFlags_None);
    ImGui::TableHeadersRow();

    // Function to create a SliderFloat for a vector component with sensible defaults
    auto CreateSliderFloat = [](const char* label, float* value, float min, float max) {
      ImGui::TableSetColumnIndex(1);
      ImGui::SliderFloat(label, value, min, max);  // Assuming a range of 0.0f to 1.0f
    };

    // Function to create a SliderFloat for 'Y' component
    auto CreateSliderFloatY = [](const char* label, float* value, float min, float max) {
      ImGui::TableSetColumnIndex(2);
      ImGui::SliderFloat(label, value, min, max);  // Assuming a range of 0.0f to 1.0f
    };

    // Function to create a SliderFloat for 'Z' component
    auto CreateSliderFloatZ = [](const char* label, float* value, float min, float max) {
      ImGui::TableSetColumnIndex(3);
      ImGui::SliderFloat(label, value, min, max);  // Assuming a range of 0.0f to 1.0f
    };

    // Row for vector 'a'
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Y-Offset");                              // Display the vector name
    CreateSliderFloat("##a_x", &a->x, -3.141f, 3.141f);   // Slider for 'a.x'
    CreateSliderFloatY("##a_y", &a->y, -3.141f, 3.141f);  // Slider for 'a.y'
    CreateSliderFloatZ("##a_z", &a->z, -3.141f, 3.141f);  // Slider for 'a.z'

    // Repeat the above for vectors 'b', 'c', and 'd'

    // Row for vector 'b'
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Amplitude");                             // Display the vector name
    CreateSliderFloat("##b_x", &b->x, -3.141f, 3.141f);   // Slider for 'b.x'
    CreateSliderFloatY("##b_y", &b->y, -3.141f, 3.141f);  // Slider for 'b.y'
    CreateSliderFloatZ("##b_z", &b->z, -3.141f, 3.141f);  // Slider for 'b.z'

    // Row for vector 'c'
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Frequenz");                              // Display the vector name
    CreateSliderFloat("##c_x", &c->x, -3.141f, 3.141f);   // Slider for 'c.x'
    CreateSliderFloatY("##c_y", &c->y, -3.141f, 3.141f);  // Slider for 'c.y'
    CreateSliderFloatZ("##c_z", &c->z, -3.141f, 3.141f);  // Slider for 'c.z'

    // Row for vector 'd'
    ImGui::TableNextRow();
    ImGui::TableSetColumnIndex(0);
    ImGui::Text("Phase");                                 // Display the vector name
    CreateSliderFloat("##d_x", &d->x, -3.141f, 3.141f);   // Slider for 'd.x'
    CreateSliderFloatY("##d_y", &d->y, -3.141f, 3.141f);  // Slider for 'd.y'
    CreateSliderFloatZ("##d_z", &d->z, -3.141f, 3.141f);  // Slider for 'd.z'

    // End the table
    ImGui::EndTable();
  }
}

}  // namespace ImGui

void OsOpenInShell(const char* path) {
#ifdef _WIN32
  ::ShellExecuteA(NULL, "open", path, NULL, NULL, SW_SHOWDEFAULT);
#else
#if __APPLE__
  const char* open_executable = "open";
#else
  const char* open_executable = "xdg-open";
#endif
  char command[256];
  snprintf(command, 256, "%s \"%s\"", open_executable, path);
  system(command);
#endif
}

#endif  // MISC_H