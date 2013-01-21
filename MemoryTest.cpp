#include "Common.h"
void test(AVectorFactory * factory,const int MAX_N){
  vector<size_t> maxAllocation(MAX_N+1,0);
  vector<size_t> minAllocation(MAX_N+1,((size_t)MAX_N)*100);
  const double STEP = 0.01;
      
  for(int n=1;n<=MAX_N*5;n += (int)max(1.0,n*STEP)){
    clog << n << " of " << MAX_N << "\r";
    AVector * implementation = factory->create();
    for(int j=0;j<n;++j){
      implementation->push(42);
    }
    for(int j=0;j<n;++j){
      implementation->pop();
      const size_t s = implementation->size();
      if(s<MAX_N){
        const size_t a = implementation->allocatedSize();
        minAllocation[s] = min(minAllocation[s] ,a);
        if(maxAllocation[s] < a){
          maxAllocation[s] = max(maxAllocation[s] ,a);            
        }            
      }
    }
    delete implementation;
  }
  for(int s=1;s<MAX_N;++s){
    cout 
      << "size " << s 
      << " min " << (double)minAllocation[s]/s/sizeof(int) 
      << " max " << (double)maxAllocation[s]/s/sizeof(int) 
      << " pow " << log((double)s)/log((double)maxAllocation[s]/sizeof(int)-s)
      << endl; 
  }    
}
int main(int argc,const char * args[]){
  if(argc<3){
    cerr 
      << "Syntax is\n" 
      << args[0] << " MAX_N (regular|succinct K|waterloo)" << endl;
    return 1;
  }
  AVectorFactory * factory;
  int MAX_N = atoi(args[1]);
  string name(args[2]);
  if(name=="regular"){
    factory = new RegularVectorFactory();
    clog << "Using RegularVectorFactory" << endl;
  }else if(name=="succinct"){
    if(argc<4){
      cerr << "Bad implementation" << endl;
      return 2;
    }
    int k=atoi(args[3]);
    if(k==2){
      factory = new SuccinctVectorFactory<2>();
    }else if(k==3){
      factory = new SuccinctVectorFactory<3>();
    }else if(k==4){
      factory = new SuccinctVectorFactory<4>();
    }else if(k==5){
      factory = new SuccinctVectorFactory<5>();
    }else{
      cerr << "Bad implementation" << endl;
      return 3;
    }
    clog << "Using SuccinctVectorFactory<" << k << ">" << endl;
  }else if(name=="waterloo"){
    factory = new WaterlooVectorFactory();
    clog << "Using WaterlooVectorFactory" << endl;
  }
  test(factory,MAX_N);
  return 0;
}
