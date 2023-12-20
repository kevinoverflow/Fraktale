#include "shader.h"

#include <iostream>

#include "cmrc/cmrc.hpp"

CMRC_DECLARE(Shaders);

void Shader::Init() {
  Destroy();
  program = glCreateProgram();
}

bool Shader::Load(GLenum shaderType, const char* shaderFileName) {
  GLuint shader = glCreateShader(shaderType);

  // Load the shader file from CMRC
  auto fs = cmrc::Shaders::get_filesystem();
  auto shaderFile = fs.open(shaderFileName);

  // Read the contents of the file
  auto shaderSource = std::string(shaderFile.begin(), shaderFile.end());

  // Compile the shader
  std::cout << "[INFO] " << shaderFileName << " wird kompiliert" << std::endl;
  const char* c_str = shaderSource.c_str();
  glShaderSource(shader, 1, &c_str, nullptr);
  glCompileShader(shader);

  // Check the shader
  GLint result = GL_FALSE;
  int infoLogLength;

  glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

  if (infoLogLength > 0) {
    char* errorMessage = (char*)malloc(infoLogLength + 1);
    glGetShaderInfoLog(shader, infoLogLength, nullptr, errorMessage);
    std::cerr << "[FEHLER] Shader Kompilierung fehlgeschlagen: " << errorMessage << std::endl;
    free(errorMessage);
    return false;
  }

  std::cout << "[INFO] Shader Kompilierung erfolgreich" << std::endl;

  glAttachShader(program, shader);

  return true;
}

bool Shader::Link() {
  std::cout << "[INFO] Shader verlinken" << std::endl;
  glLinkProgram(program);

  // Check the program
  GLint result;
  int infoLogLength;

  glGetProgramiv(program, GL_LINK_STATUS, &result);
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
  if (infoLogLength > 0) {
    char* errorMessage = (char*)malloc(infoLogLength + 1);
    glGetProgramInfoLog(program, infoLogLength, nullptr, errorMessage);
    std::cerr << "[FEHLER] Verlinkung fehlgeschlagen: " << errorMessage << std::endl;
    free(errorMessage);
    return false;
  }

  std::cout << "[INFO] Verlinkung erfolgreich" << std::endl;
  return true;
}

void Shader::Attach() { glUseProgram(program); }

void Shader::Detach() { glUseProgram(0); }

void Shader::Destroy() { glDeleteProgram(program); }

void Shader::SetUniform(const char* name, GLfloat v0) {
  GLint location = GetUniformLocation(name);
  glUniform1f(location, v0);
}

void Shader::SetUniform(const char* name, GLfloat v0, GLfloat v1) {
  GLint location = GetUniformLocation(name);
  glUniform2f(location, v0, v1);
}

void Shader::SetUniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2) {
  GLint location = GetUniformLocation(name);
  glUniform3f(location, v0, v1, v2);
}

void Shader::SetUniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
  GLint location = GetUniformLocation(name);
  glUniform4f(location, v0, v1, v2, v3);
}

GLint Shader::GetUniformLocation(const char* name) { return glGetUniformLocation(program, name); }