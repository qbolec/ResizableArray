#include "WaterlooVectorFactory.h"
#include "WaterlooVector.h"

AVector * WaterlooVectorFactory::create(){
  return new WaterlooVector();
}
