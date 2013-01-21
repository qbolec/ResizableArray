#include "RegularVector.h"

void RegularVector::push(int e){
  v.push_back(e);
}

int & RegularVector::at(size_t idx){
  return v[idx];
}

size_t RegularVector::size(){
  return v.size();
}

size_t RegularVector::allocatedSize(){
  return sizeof(v) + v.capacity()*sizeof(int);
}

int RegularVector::pop(){
  int e=v.back();
  v.pop_back();
  if(v.size()==v.capacity()/4){
    std::vector<int> w;
    w.reserve(v.size()*2);
    w.resize(v.size());
    std::copy(v.begin(),v.end(),w.begin());
    std::swap(w,v);
  }
  return e;
}
