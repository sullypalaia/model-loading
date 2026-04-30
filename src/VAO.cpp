#include "glad/glad.h"

#include "VAO.h"

VAO::VAO() { glCreateVertexArrays(1, &m_id); }

void VAO::destroy() { glDeleteVertexArrays(1, &m_id); }

void VAO::attach_element_buffer(const EBO &ebo) {
  glVertexArrayElementBuffer(m_id, ebo.get_id());
}

void VAO::add_attribute(GLuint index, GLuint binding_index, GLint size,
                        GLenum type, GLboolean normalized,
                        GLuint relative_offset) {
  glEnableVertexArrayAttrib(m_id, index);
  glVertexArrayAttribFormat(m_id, index, size, type, normalized,
                            relative_offset);
  glVertexArrayAttribBinding(m_id, index, binding_index);
}

void VAO::bind() { glBindVertexArray(m_id); }
