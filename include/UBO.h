#pragma once

#include <vector>

template <typename T> class UBO {
public:
  UBO(std::vector<T> data);

  void destroy() const;

  const unsigned int &get_id() const;

private:
  unsigned int m_id;
};
