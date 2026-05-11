#pragma once

#include <string>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "EBO.h"
#include "ShaderProgram.h"
#include "TextureManager.h"
#include "VAO.h"
#include "VBO.h"

class Mesh {
public:
  Mesh(const char *filename);

  int init();

  void set_uniform_matrix(const char *name, glm::mat4 &mat);

  void draw();

  void destroy();

private:
  struct m_Vertex {
    float pos[3];
    float tex_coord[2];
    float normal[3];
  };

  struct m_Mesh {
    int num_indices;
    int base_vertex;
    int base_index;
    int mat_index;
  };

  std::vector<m_Mesh> m_meshes;

  std::vector<m_Vertex> m_vertices;
  std::vector<unsigned int> m_indices;

  std::string m_model_path;

  std::vector<std::string> m_tex_paths;

  TextureManager *m_tex_manager;

  VAO m_vao;
  EBO *m_ebo;
  VBO<m_Vertex> *m_vbo;
  ShaderProgram m_program;
};
