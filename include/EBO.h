#pragma once

#include <vector>

class EBO {
public:
  EBO(std::vector<unsigned int> indices);

  void destroy();

  const unsigned int &get_id() const;

private:
  unsigned int m_id;
};
