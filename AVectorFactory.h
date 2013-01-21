#ifndef A_VECTOR_FACTORY
#define A_VECTOR_FACTORY

#include "AVector.h"

class AVectorFactory{
public:
  virtual AVector * create()=0;
};

#endif
