#ifndef SHADER_H
#define SHADER_H

#include "GL/glew.h"

class Shader {
 public:
  void Init();
  bool Load(GLenum shaderType, const char* shaderFileName);
  bool Link();
  void Attach();
  void Detach();
  void Destroy();
  void SetUniform(const char* name, GLfloat v0);
  void SetUniform(const char* name, GLfloat v0, GLfloat v1);
  void SetUniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2);
  void SetUniform(const char* name, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);

 private:
  GLuint program;
  GLint GetUniformLocation(const char* name);
};

#endif  // SHADER_H