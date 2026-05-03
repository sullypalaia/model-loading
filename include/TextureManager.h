#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "assimp/scene.h"

class TextureManager {
public:
  TextureManager(std::vector<std::string> paths, std::vector<int> indices,
                 unsigned int texture_type);
  int init(const aiScene *scene);

  void bind_texture(int mat_index);

  bool texture_exists(int mat_index);

  void destroy();

private:
  std::unordered_map<int, unsigned int> m_textures;
  std::vector<std::string> m_paths;
  std::vector<int> m_indices;
  unsigned int m_target;
};
