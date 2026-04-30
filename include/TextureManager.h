#pragma once

#include <string>
#include <vector>

class TextureManager {
public:
  TextureManager(std::vector<std::string> paths, std::vector<int> indices,
                 unsigned int texture_type);
  int init();

  void bind_texture(unsigned int id);

  bool texture_exists(unsigned int id);

  void destroy();

private:
  unsigned int *m_ids;
  std::vector<std::string> m_paths;
  std::vector<int> m_indices;
  unsigned int m_target;

  inline static int tex_unit = 0;
};
