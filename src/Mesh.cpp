#include <filesystem>
#include <iostream>

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"
#include "glad/glad.h"

#include "Mesh.h"

Mesh::Mesh(const char *filename) : m_program("model.vert", "model.frag") {
  m_model_path = static_cast<std::string>(std::filesystem::current_path()) +
                 "/res/" + filename;
}

int Mesh::init() {
  // import the scene
  Assimp::Importer importer;
  const aiScene *scene = importer.ReadFile(
      m_model_path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals |
                                aiProcess_FlipUVs |
                                aiProcess_JoinIdenticalVertices);

  if (!scene) {
    std::cerr << importer.GetErrorString();
    return 0;
  }

  m_meshes.resize(scene->mNumMeshes);

  int num_vertices = 0;
  int num_indices = 0;

  for (int i = 0; i < m_meshes.size(); ++i) {
    // add data for meshes
    const aiMesh *mesh = scene->mMeshes[i];
    m_meshes[i].mat_index = mesh->mMaterialIndex;
    m_meshes[i].num_indices = mesh->mNumFaces * 3;
    m_meshes[i].base_vertex = num_vertices;
    m_meshes[i].base_index = num_indices;

    num_vertices += mesh->mNumVertices;
    num_indices += m_meshes[i].num_indices;

    // update the vertex data
    for (int j = 0; j < mesh->mNumVertices; ++j) {
      const aiVector3D &pos = mesh->mVertices[j];
      const aiVector3D &normal = mesh->mNormals[j];
      const aiVector3D &tex_coord = mesh->HasTextureCoords(0)
                                        ? mesh->mTextureCoords[0][j]
                                        : aiVector3D(0.0f, 0.0f, 0.0f);

      m_vertices.push_back({{pos.x, pos.y, pos.z},
                            {tex_coord.x, tex_coord.y},
                            {normal.x, normal.y, normal.z}});
    }

    // update the indices
    for (int k = 0; k < mesh->mNumFaces; ++k) {
      const aiFace face = mesh->mFaces[k];
      for (int j = 0; j < 3; ++j) {
        m_indices.push_back(face.mIndices[j]);
      }
    }
  }

  std::vector<int> tex_indices;

  // find the texture paths
  for (int i = 0; i < scene->mNumMaterials; ++i) {
    const aiMaterial *mat = scene->mMaterials[i];

    if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 ||
        mat->GetTextureCount(aiTextureType_EMISSIVE) > 0) {
      aiString path;

      if (mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS ||
          mat->GetTexture(aiTextureType_EMISSIVE, 0, &path) == AI_SUCCESS) {
        std::string p{path.data};
        std::cout << "adding texture path " << p << '\n';
        if (p.substr(0, 2) == ".\\")
          p = p.substr(2, p.size() - 1);

        if (p[0] == '*') {
          m_tex_paths.push_back(p);
          tex_indices.push_back(i);
          continue;
        }

        std::string full_path =
            static_cast<std::string>(std::filesystem::current_path()) + "/" + p;

        tex_indices.push_back(i);
        m_tex_paths.push_back(full_path);
      }
    }
    if (mat->GetTextureCount(aiTextureType_BASE_COLOR) > 0)
      std::cout << "bruh\n";
  }

  // create the textures with the texture manager
  m_tex_manager = new TextureManager(m_tex_paths, tex_indices, GL_TEXTURE_2D);
  if (!m_tex_manager->init(scene)) {
    std::cerr << "failed to create textures\n";
    return 0;
  }

  // create the buffers
  m_vbo = new VBO(m_vertices);
  m_vao.attach_vertex_buffer(*m_vbo, 0, 0, sizeof(m_Vertex));

  m_ebo = new EBO(m_indices);
  m_vao.attach_element_buffer(*m_ebo);

  // add the vertex attributes
  m_vao.add_attribute(0, 0, 3, GL_FLOAT, GL_FALSE, offsetof(m_Vertex, pos));
  m_vao.add_attribute(1, 0, 2, GL_FLOAT, GL_FALSE,
                      offsetof(m_Vertex, tex_coord));
  m_vao.add_attribute(2, 0, 3, GL_FLOAT, GL_FALSE, offsetof(m_Vertex, normal));

  // initialize the shader program
  if (!m_program.init()) {
    std::cerr << "shader program initialization failed\n";
    return 0;
  }

  return 1;
}

void Mesh::set_uniform_matrix(const char *name, glm::mat4 &mat) {
  m_program.use();
  glUniformMatrix4fv(glGetUniformLocation(m_program.get_id(), name), 1,
                     GL_FALSE, glm::value_ptr(mat));
}

// draw each of the meshes
void Mesh::draw() {
  glFrontFace(GL_CCW);

  m_vao.bind();

  for (int i = 0; i < m_meshes.size(); ++i) {
    int mat_index = m_meshes[i].mat_index;
    // bind the texture, if it exists

    m_program.use();
    m_tex_manager->bind_texture(mat_index);

    glDrawElementsBaseVertex(GL_TRIANGLES, m_meshes[i].num_indices,
                             GL_UNSIGNED_INT,
                             (void *)(sizeof(GLuint) * m_meshes[i].base_index),
                             m_meshes[i].base_vertex);
  }
}

void Mesh::destroy() {
  m_vbo->destroy();
  m_ebo->destroy();
  m_vao.destroy();
  m_program.destroy();
}
