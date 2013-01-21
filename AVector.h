#ifndef A_VECTOR_H
#define A_VECTOR_H

#include <cstddef>

class AVector{
public:
  virtual void push(int e)=0;
  virtual int & at(size_t idx)=0;
  virtual size_t size()=0;
  virtual size_t allocatedSize()=0;
  virtual int pop()=0;
  virtual ~AVector(){}
};

#endif
