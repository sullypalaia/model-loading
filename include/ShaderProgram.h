#pragma once

#include <string>

class ShaderProgram {
public:
  ShaderProgram(std::string vert_filename, std::string frag_filename = "");

  int init();

  void relink();

  void destroy();

  void add_varyings(int count, const char *const *varyings,
                    unsigned int format);

  void use();

private:
  unsigned int m_id;

  std::string m_vert_path;
  std::string m_frag_path;
};
