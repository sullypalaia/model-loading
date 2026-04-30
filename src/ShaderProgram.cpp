#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "glad/glad.h"

#include "ShaderProgram.h"

using namespace std::string_literals;

ShaderProgram::ShaderProgram(std::string vert_filename,
                             std::string frag_filename) {
  std::string cwd{std::filesystem::current_path()};

  m_vert_path = cwd + "/shaders/" + vert_filename;
  if (!frag_filename.empty())
    *m_frag_path = cwd + "/shaders/" + frag_filename;
}

int ShaderProgram::init() {
  m_id = glCreateProgram();
  GLuint vert_shader = glCreateShader(GL_VERTEX_SHADER);
  GLuint frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

  std::ifstream vert_in(m_vert_path);
  if (!vert_in) {
    std::cerr << "failed to open " << m_vert_path << '\n';
    return 0;
  }
  std::string vert_s{std::istreambuf_iterator<char>(vert_in),
                     std::istreambuf_iterator<char>()};
  const GLchar *vert_cstr{vert_s.c_str()};

  glShaderSource(vert_shader, 1, &vert_cstr, NULL);
  glCompileShader(vert_shader);

  GLint vert_comp_status;
  glGetShaderiv(vert_shader, GL_COMPILE_STATUS, &vert_comp_status);

  if (!vert_comp_status) {
    std::cerr << "compilation of " << m_vert_path << " failed\n";

    GLint vert_log_length;
    glGetShaderiv(vert_shader, GL_INFO_LOG_LENGTH, &vert_log_length);
    GLchar *vert_log = (GLchar *)malloc(vert_log_length * sizeof(GLchar));
    glGetShaderInfoLog(vert_shader, vert_log_length, NULL, vert_log);

    std::cerr << vert_log;
    return 0;
  }

  glAttachShader(m_id, vert_shader);
  glDeleteShader(vert_shader);

  if (m_frag_path) {
    std::ifstream frag_in(*m_frag_path);
    if (!frag_in) {
      std::cerr << "failed to open " << m_frag_path << '\n';
      return 0;
    }
    std::string frag_s{std::istreambuf_iterator<char>(frag_in),
                       std::istreambuf_iterator<char>()};
    const GLchar *frag_cstr{frag_s.c_str()};

    glShaderSource(frag_shader, 1, &frag_cstr, NULL);
    glCompileShader(frag_shader);

    GLint frag_comp_status;
    glGetShaderiv(frag_shader, GL_COMPILE_STATUS, &frag_comp_status);

    if (!frag_comp_status) {
      std::cerr << "compilation of " << m_frag_path << " failed\n";

      GLint frag_log_length;
      glGetShaderiv(frag_shader, GL_INFO_LOG_LENGTH, &frag_log_length);
      GLchar *frag_log = (GLchar *)malloc(frag_log_length * sizeof(GLchar));
      glGetShaderInfoLog(frag_shader, frag_log_length, NULL, frag_log);

      std::cerr << frag_log;
      return 0;
    }

    glAttachShader(m_id, frag_shader);
    glDeleteShader(frag_shader);
  }

  glLinkProgram(m_id);

  GLint link_status;
  glGetProgramiv(m_id, GL_LINK_STATUS, &link_status);

  if (!link_status) {
    std::cerr << "failed to link program\n";

    GLint program_log_length;
    glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &program_log_length);
    GLchar *program_log = (GLchar *)malloc(program_log_length * sizeof(GLchar));
    glGetProgramInfoLog(m_id, program_log_length, NULL, program_log);

    std::cerr << program_log;
    return 0;
  }

  return 1;
}

void ShaderProgram::relink() { glLinkProgram(m_id); }

void ShaderProgram::add_varyings(GLsizei count, const GLchar *const *varyings,
                                 GLenum format) {
  glTransformFeedbackVaryings(m_id, count, varyings, format);
}

void ShaderProgram::use() { glUseProgram(m_id); }

void ShaderProgram::destroy() { glDeleteProgram(m_id); }
