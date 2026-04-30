#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "glad/glad.h"

#include "TextureManager.h"

TextureManager::TextureManager(std::vector<std::string> paths,
                               std::vector<int> indices, GLenum target)
    : m_target(target), m_paths(paths), m_indices(indices) {}

int TextureManager::init() {
  // initialize the texture objects
  glCreateTextures(m_target, m_indices.size(), m_ids);
  for (int &i : m_indices) {
    glCreateTextures(m_target, 1, &m_ids[i]);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(m_ids[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_ids[i], GL_TEXTURE_WRAP_T, GL_REPEAT);

    // extract texture data
    int tex_width;
    int tex_height;
    int tex_num_channels;
    int desired_channels;
    GLenum format;
    GLenum internal_format;

    std::string file_ext =
        m_paths[i].substr(m_paths[i].find(':'), m_paths[i].size() - 1);
    if (file_ext == "jpg" | file_ext == "jpeg") {
      desired_channels = 3;
      format = GL_RGB;
      internal_format = GL_RGB8;
    } else {
      desired_channels = 4;
      format = GL_RGBA;
      internal_format = GL_RGBA8;
    }

    unsigned char *data = stbi_load(m_paths[i].c_str(), &tex_width, &tex_height,
                                    &tex_num_channels, desired_channels);
    if (!data) {
      std::cerr << "failed to load texture " << m_paths[i] << '\n';
      return 0;
    }

    if (m_target == GL_TEXTURE_2D) {
      glTextureStorage2D(m_ids[i], 1, internal_format, tex_width, tex_height);
      glTextureSubImage2D(m_ids[i], 0, 0, 0, tex_width, tex_height, format,
                          GL_UNSIGNED_BYTE, data);
    }
  }

  return 1;
}

bool TextureManager::texture_exists(GLuint id) {
  if (!m_indices[id])
    return 0;
  else
    return 1;
}

void TextureManager::bind_texture(GLuint id) { glBindTextureUnit(0, id); }

void TextureManager::destroy() { glDeleteTextures(1, m_ids); }
