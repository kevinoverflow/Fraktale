#include <complex>

#include "dearimgui.h"
#include "glm/glm.hpp"
#include "imgui.h"
#include "misc.h"
#include "shader.h"
#include "window.h"

/* ------ Variablen ------ */
static const char* programName = "Fraktale";
int width = 1280;
int height = 720;
float scale = 1.0f;

int crosshair = 1;

GLFWwindow* window = nullptr;

// OpenGL Buffer
static GLuint VAO, VBO, EBO;
static GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,  // A
    1.0f,  -1.0f, 0.0f,  // B
    1.0f,  1.0f,  0.0f,  // C
    -1.0f, 1.0f,  0.0f,  // D
};
static GLint indices[] = {0, 1, 2, 2, 3, 0};

static Shader* shader = new Shader();

// Mauseingabe
double offsetX, offsetY;
double zoom = 0.75;

// Aktuelles Fraktal
static int fractal = Fractal::Mandelbrot;

static int iterations = 50;

static float juliaReal = -0.4f;
static float juliaImag = 0.8f;

// Farbe
static int colorAlgorithm = ColorAlgorithm::Single;

// Farbe - Einfarbig
static float color[3] = {1.0f, 1.0f, 1.0f};

// Farbe - Farbverlauf (Parameter für die Farbverlaufsfunktion)
/*ImGui::Gradient gradients[] = {
    {
        "1",
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(0.5f, 0.5f, 0.5f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.00f, 0.1, 0.2f)
    },
    {
        "2",
        glm::vec3(1.0, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f),
        glm::vec3(0.00f, 0.0, 0.0f)
    },
};*/

ImGui::Gradient gradients[] = {
    {"Palette 1", glm::vec3(0.0f, 0.5f, 0.5f), glm::vec3(0.0f, 0.5f, 0.5f),
     glm::vec3(0.0f, 0.5f, 0.33f), glm::vec3(0.0f, 0.5f, 0.66f)},
    {"Palette 2", glm::vec3(0.938f, 0.328f, 0.718f), glm::vec3(0.659f, 0.438f, 0.328f),
     glm::vec3(0.388f, 0.388f, 0.296f), glm::vec3(2.538f, 2.478f, 0.168f)},
    {"Palette 3", glm::vec3(0.66f, 0.56f, 0.68f), glm::vec3(0.718f, 0.438f, 0.72f),
     glm::vec3(0.52f, 0.8f, 0.52f), glm::vec3(-0.43f, -0.397f, -0.083f)},
    {"Palette 4", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(0.8f, 0.8f, 0.5f), glm::vec3(0.0f, 0.2f, 0.5f)},
    {"Palette 5", glm::vec3(0.821f, 0.328f, 0.242f), glm::vec3(0.659f, 0.481f, 0.896f),
     glm::vec3(0.612f, 0.34f, 0.296f), glm::vec3(2.82f, 3.026f, -0.273f)},
    {"Palette 6", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.33f, 0.67f)},
    {"Palette 7", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.1f, 0.2f)},
    {"Palette 8", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.3f, 0.2f, 0.2f)},
    {"Palette 9", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.8f, 0.9f, 0.3f)},
    {"Palette 10", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(1.0f, 0.7f, 0.4f), glm::vec3(0.0f, 0.15f, 0.2f)},
    {"Palette 11", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(2.0f, 1.0f, 0.0f), glm::vec3(0.5f, 0.2f, 0.25f)},
    {"Palette 12", glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.5f, 0.5f, 0.5f),
     glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.25f, 0.25f)}};

ImGui::Gradient currentGradient = {"Palette 7", glm::vec3(0.5f, 0.5f, 0.5f),
                                   glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(1.0f, 1.0f, 1.0f),
                                   glm::vec3(0.00f, 0.1, 0.2f)};

/* ------ main ------ */
int main() {
  // GLFW Fenster erstellen
  if (window_init(width, height, programName) != 0) {
    return 1;
  }
  // Dear ImGui initialisieren
  imgui_init();

  // Vertex Array erstellen
  glGenVertexArrays(1, &VAO);
  glBindVertexArray(VAO);

  // Vertex Buffer erstellen
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

  // Element Buffer erstellen
  glGenBuffers(1, &EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Shader laden
  shader->Init();
  shader->Load(GL_VERTEX_SHADER, "VertexShader.vert");
  shader->Load(GL_FRAGMENT_SHADER, "FragmentShader.frag");
  shader->Link();

  // Fenstergröße anpassen
  glfwGetFramebufferSize(window, &width, &height);
  glViewport(0, 0, width, height);

  // Skalierungsfaktor (DPI) anpassen
  int wwidth;
  glfwGetWindowSize(window, &wwidth, nullptr);
  scale = (float)width / (float)wwidth;

  /* ------ Hauptschleife ------ */
  while (!window_should_close()) {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // OpenGL Vertices zeichnen
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    glDisableVertexAttribArray(0);

    // Shader aktualisieren und Uniforms setzen
    shader->Attach();
    shader->SetUniform("u_resolution", (GLfloat)width, (GLfloat)height);
    shader->SetUniform("u_iterations", (GLfloat)iterations);
    shader->SetUniform("u_offset", (GLfloat)offsetX, (GLfloat)offsetY);
    shader->SetUniform("u_zoom", (GLfloat)zoom);
    shader->SetUniform("u_crosshair", (GLfloat)crosshair);
    shader->SetUniform("u_fractal", (GLfloat)fractal);
    shader->SetUniform("u_color_scheme", (GLfloat)colorAlgorithm);
    shader->SetUniform("u_julia_c", (GLfloat)juliaReal, (GLfloat)juliaImag);
    shader->SetUniform("u_color", (GLfloat)color[0], (GLfloat)color[1], (GLfloat)color[2]);
    shader->SetUniform("u_a", currentGradient.a.x, currentGradient.a.y, currentGradient.a.z);
    shader->SetUniform("u_b", currentGradient.b.x, currentGradient.b.y, currentGradient.b.z);
    shader->SetUniform("u_c", currentGradient.c.x, currentGradient.c.y, currentGradient.c.z);
    shader->SetUniform("u_d", currentGradient.d.x, currentGradient.d.y, currentGradient.d.z);

    // Dear ImGui UI zeichnen
    imgugi_update([]() {
      ImGui::Begin("Einstellungen");

      ImGui::SeparatorText("Position");
      ImGui::Text("Mittelpunkt %.2f%+.2fi", mapToComplex(offsetX, offsetY).real(),
                  mapToComplex(offsetX, offsetY).imag());
      ImGui::Text("Zoom: %.2f", zoom);
      ImGui::Checkbox("Fadenkreuz", (bool*)&crosshair);
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
        if (ImGui::BeginCombo("##GradientSelector", currentGradient.id)) {
          for (int i = 0; i < IM_ARRAYSIZE(gradients); i++) {
            bool isSelected = (currentGradient.id == gradients[i].id);

            // Push style variables to adjust item width and height
            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                                ImVec2(10, 10));  // Adjust spacing between items
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                                ImVec2(200, 50));  // Adjust padding inside each item frame

            if (ImGui::Selectable(gradients[i].id, isSelected)) {
              currentGradient = gradients[i];
            }

            if (isSelected) {
              ImGui::SetItemDefaultFocus();  // Set focus to the selected item
            }

            // Render gradient rect as combo box item
            ImGuiStyle& style = ImGui::GetStyle();

            float h = ImGui::GetTextLineHeightWithSpacing() - style.FramePadding.y;
            ImGui::DrawGradientRect(gradients[i], 200, h, 200);

            // Pop style variables
            ImGui::PopStyleVar(2);  // Pop both ItemSpacing and FramePadding
          }
          ImGui::EndCombo();
        }

        // Render the gradient rect view when the combo box is closed
        if (!ImGui::IsPopupOpen("##GradientSelector")) {
          ImGui::DrawGradientRect(currentGradient, 200, 30, 200);
        }

        ImGui::Dummy(ImVec2(0.0f, 30.0f));
        ImGui::VectorTable(&currentGradient);
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

    // Fenster aktualisieren
    window_update();
  }

  // Ressourcen freigeben
  imgui_destroy();
  window_destroy();

  return 0;
}
