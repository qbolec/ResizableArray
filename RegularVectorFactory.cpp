#include "RegularVectorFactory.h"
#include "RegularVector.h"

AVector * RegularVectorFactory::create(){
  return new RegularVector();
}
