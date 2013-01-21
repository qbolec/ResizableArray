#ifndef WATERLOO_VECTOR
#define WATERLOO_VECTOR

#include "AVector.h"

class WaterlooVector : public AVector{
  struct BufferInfo{
    size_t size;
    size_t occupancy;
    BufferInfo(size_t size,size_t occupancy);
    bool isFull();
    bool isEmpty();
    bool isTooLarge();
  };
  size_t n;
  size_t s;
  //size_t d;
  bool hasSpare;
  BufferInfo lastDataBlock;
  BufferInfo lastSuperBlock;
  BufferInfo indexBlock;
  int * * index;

  void shrink();
  void grow();
  unsigned int MSB(unsigned long long int x);

public:
  WaterlooVector();
  void push(int e);
  int & at(size_t idx);
  size_t size();
  size_t allocatedSize();
  int pop();
  ~WaterlooVector();
};

#endif
