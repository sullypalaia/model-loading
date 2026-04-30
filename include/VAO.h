#pragma once

#include "EBO.h"
#include "VBO.h"

class VAO {
public:
  VAO();

  void destroy();

  template <typename T>
  void attach_vertex_buffer(const VBO<T> &vbo, unsigned int binding_index,
                            std::ptrdiff_t offset, std::ptrdiff_t stride) {
    glVertexArrayVertexBuffer(m_id, binding_index, vbo.get_id(), offset,
                              stride);
  }

  void attach_element_buffer(const EBO &ebo);

  void add_attribute(unsigned int index, unsigned int binding_index, int size,
                     unsigned int type, unsigned char normalized,
                     unsigned int relative_offset);

  void bind();

private:
  unsigned int m_id;
};
