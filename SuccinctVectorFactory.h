#ifndef SUCCINCT_VECTOR_FACTORY_H
#define SUCCINCT_VECTOR_FACTORY_H

#include "SuccinctVector.h"
#include "AVectorFactory.h"
template<int k>
class SuccinctVectorFactory : public AVectorFactory{
  AVector * create(){
    return new SuccinctVector<k>();
  }
};
#endif
