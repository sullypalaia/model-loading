#pragma once

#include "glad/glad.h"

template <typename T> class UBO {
public:
  UBO() {
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, sizeof(T), nullptr, GL_DYNAMIC_STORAGE_BIT);
  }

  void bind() const { glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_id); }

  void update(const int offset, const int size, const void *data) const {
    glNamedBufferSubData(m_id, offset, size, data);
  }

  void destroy() const { glDeleteBuffers(1, &m_id); }

private:
  unsigned int m_id;
};
