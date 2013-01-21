#ifndef REGULAR_VECTOR
#define REGULAR_VECTOR

#include "AVector.h"
#include <vector>

class RegularVector : public AVector{
private:
  std::vector<int> v;
public:
  void push(int e);
  int & at(size_t idx);
  size_t size();
  size_t allocatedSize();
  int pop();
};

#endif
