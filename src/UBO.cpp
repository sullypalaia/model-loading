#include "glad/glad.h"

#include "UBO.h"

template <typename T> UBO<T>::UBO(std::vector<T> data) {
  glCreateBuffers(1, &m_id);
}

template <typename T> const GLuint &UBO<T>::get_id() const { return m_id; }

template <typename T> void UBO<T>::destroy() const {
  glDeleteBuffers(1, &m_id);
}
