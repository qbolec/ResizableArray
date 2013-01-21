#include<iostream>
#include<vector>
#include<cassert>
#include<cmath>
#include<algorithm>
using namespace std;
#include "a_vector.h"
#include "regular_vector.h"
#include "optimized_succinct_vector.h"
#include "waterloo_vector.h"

void test(vector<AVector*> & implementations){
  int popPbb = 30;
  for(int t=0;t<2000000;++t){
    if(popPbb == 30 && implementations[0]->size() > 10000){
      popPbb = 70;
    }else if(popPbb == 70 && implementations[0]->size()==0){
      popPbb = 30;
    }
    if(rand()%100<popPbb && 0<implementations[0]->size()){
      //clog << "pop" << endl;
      int e=implementations[0]->pop();
      for(size_t i=1;i<implementations.size();++i){
        assert(e == implementations[i]->pop());
      }
    }else{
      //clog << "push" << endl;
      int e = rand();
      for(size_t i=0;i<implementations.size();++i){
        implementations[i]->push(e);
      }
    }
    //clog << "verifying" << endl;
    size_t s=implementations[0]->size();
    for(size_t i=1;i<implementations.size();++i){
      assert(s == implementations[i]->size());
      implementations[i]->allocatedSize();
      for(size_t idx=0;idx<s;++idx){
        assert(implementations[0]->at(idx) == implementations[i]->at(idx));
      }
    }
    if(t%1==0){
      cout << "\r" << t << " " << implementations[0]->size() << "   " << flush;
    }
  }
}
int main(){
  vector<AVector*> implementations;
  implementations.push_back(new RegularVector());
  implementations.push_back(new SuccinctVector<3>());
  implementations.push_back(new WaterlooVector());
  test(implementations);
  return 0;
}
