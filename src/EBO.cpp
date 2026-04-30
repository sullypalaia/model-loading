#include "glad/glad.h"

#include "EBO.h"

EBO::EBO(std::vector<GLuint> indices) {
  glCreateBuffers(1, &m_id);
  glNamedBufferStorage(m_id, indices.size() * sizeof(GLuint), indices.data(),
                       0);
}

const GLuint &EBO::get_id() const { return m_id; }

void EBO::destroy() { glDeleteBuffers(1, &m_id); }
