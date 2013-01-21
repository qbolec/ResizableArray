#include "Common.h"

vector<size_t> test(AVectorFactory * factory,const int MAX_N){
  vector<size_t> maxAllocation(MAX_N+1,0);
  double STEP = 0.01;
    
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
        const size_t a = implementation->allocatedSize()/sizeof(int)-s;
        maxAllocation[s] = max(maxAllocation[s] ,a);            
      }
    }
    delete implementation;
  }
  return maxAllocation;
}
int main(int argc,const char * args[]){
  if(argc<2){
    cerr 
      << "Syntax is\n" 
      << args[0] << " MAX_N"  << endl;
    return 1;
  }
  int MAX_N = atoi(args[1]);
  vector<AVectorFactory*> implementations;
  implementations.push_back(new RegularVectorFactory());
  implementations.push_back(new WaterlooVectorFactory());
  implementations.push_back(new SuccinctVectorFactory<2>());
  implementations.push_back(new SuccinctVectorFactory<5>());
  vector<vector<size_t> > results;
  for(int i=0;i<(int)implementations.size();++i){
    results.push_back(test(implementations[i],MAX_N));
  }
  for(int n=1;n<=MAX_N;n=max(n+1,min(n*10,MAX_N))){
    cout << n << "\t&" ;
    for(int i=0;i<(int)implementations.size();++i){
      size_t worst = 0;
      for(int x=0;x<=n;++x){
        worst=max(worst,results[i][x]);
      }
      cout << worst << "\t&";
    }
    cout << ceil(2*sqrt(n)) << "\t&";
    cout << ceil(5*pow(n,0.2)) << "\\\\" << endl;
  }
  return 0;
}
