#ifndef WATERLOO_VECTOR_FACTORY
#define WATERLOO_VECTOR_FACTORY

#include "AVectorFactory.h"

class WaterlooVectorFactory : public AVectorFactory{
  AVector * create();
};

#endif
