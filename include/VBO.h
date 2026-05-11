#pragma once

#include "glad/glad.h"
#include <vector>

template <typename T> class VBO {
public:
  VBO(std::vector<T> data) {
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, data.size() * sizeof(T), data.data(), 0);
  }

  void destroy() const { glDeleteBuffers(1, &m_id); }

  const unsigned int &get_id() const { return m_id; };

private:
  unsigned int m_id;
};
