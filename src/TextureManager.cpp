#include <memory>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <iostream>

#include "glad/glad.h"

#include "TextureManager.h"

TextureManager::TextureManager(std::vector<std::string> paths,
                               std::vector<int> indices, GLenum target)
    : m_target(target), m_paths(paths), m_indices(indices) {}

int TextureManager::init(const aiScene *scene) {
  // create the textures
  for (int i = 0; i < m_paths.size(); ++i) {
    std::string path = m_paths[i];
    int mat_index = m_indices[i];
    GLuint id;
    glCreateTextures(m_target, 1, &id);
    glTextureParameteri(id, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(id, GL_TEXTURE_WRAP_T, GL_REPEAT);

    if (m_paths[i][0] == '*') {
      std::cout << "loading embedded texture: " << path << std::endl;

      const aiTexture *tex = scene->GetEmbeddedTexture(path.c_str());
      if (!tex) {
        std::cerr << "failed to load embedded texture " << path << '\n';
        return 0;
      }

      if (tex->mHeight == 0) {
        int tex_width;
        int tex_height;
        int tex_num_channels;
        int desired_channels;

        unsigned char *data = stbi_load_from_memory(
            reinterpret_cast<const unsigned char *>(tex->pcData), tex->mWidth,
            &tex_width, &tex_height, &tex_num_channels, 4);

        glTextureStorage2D(id, 1, GL_RGBA8, tex_width, tex_height);
        glTextureSubImage2D(id, 0, 0, 0, tex_width, tex_height, GL_RGBA,
                            GL_UNSIGNED_BYTE, data);
      } else {
        glTextureStorage2D(id, 1, GL_RGBA8, tex->mWidth, tex->mHeight);
        glTextureSubImage2D(id, 0, 0, 0, tex->mWidth, tex->mHeight, GL_RGBA,
                            GL_UNSIGNED_BYTE, tex->pcData);
      }

      m_textures[mat_index] = id;
      continue;
    }

    // extract texture data
    int tex_width;
    int tex_height;
    int tex_num_channels;
    int desired_channels;
    GLenum format;
    GLenum internal_format;

    std::cout << "loading texture: " << path << std::endl;
    std::string file_ext = path.substr(path.rfind('.') + 1, path.size() - 1);

    if (file_ext == "jpg" || file_ext == "jpeg") {
      desired_channels = 3;
      format = GL_RGB;
      internal_format = GL_RGB8;
    } else {
      desired_channels = 4;
      format = GL_RGBA;
      internal_format = GL_RGBA8;
    }

    unsigned char *data = stbi_load(path.c_str(), &tex_width, &tex_height,
                                    &tex_num_channels, desired_channels);
    if (!data) {
      std::cerr << "failed to load texture " << path << '\n';
      return 0;
    }

    if (m_target == GL_TEXTURE_2D) {
      glTextureStorage2D(id, 1, internal_format, tex_width, tex_height);
      glTextureSubImage2D(id, 0, 0, 0, tex_width, tex_height, format,
                          GL_UNSIGNED_BYTE, data);
    }

    m_textures[mat_index] = id;
  }

  return 1;
}

bool TextureManager::texture_exists(int mat_index) {
  return m_textures.find(mat_index) != m_textures.end();
}

void TextureManager::bind_texture(int mat_index) {
  glBindTextureUnit(0, m_textures[mat_index]);
}

void TextureManager::destroy() {
  for (auto &id : m_textures) {
    glDeleteTextures(1, &id.second);
  }
  m_textures.clear();
}
