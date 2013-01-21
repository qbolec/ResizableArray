//Sorry, I know this is ugly :(
#include "Common.h"
struct Info{
  int n;
  vector<clock_t> pushTimes,popTimes,atTimes;
  int AT_TRIALS, PUSH_TRIALS, POP_TRIALS;

  void output(){
    cout 
      << "size " << n
      << " push " <<fixed<< (double)median(pushTimes) / (PUSH_TRIALS*n)
      << " pop " <<fixed<< (double)median(popTimes) / (POP_TRIALS*n)
      << " at " <<fixed<< (double)median(atTimes) / AT_TRIALS
      << endl;
  }
};
map<int, Info> results;
void test(AVectorFactory * factory,const int MAX_N){
  int cs=0;
  for(int n=1;n<MAX_N;n=(int)ceil(n*1.1)){
    const int TRIALS = 1000000;
    const int AT_TRIALS = TRIALS*10;
    const int PUSH_TRIALS = max(TRIALS / n,1);
    const int POP_TRIALS = max(TRIALS / n,1);
    if( results.find(n) == results.end()){
      results[n].n = n;
      results[n].AT_TRIALS = AT_TRIALS;
      results[n].PUSH_TRIALS = PUSH_TRIALS;
      results[n].POP_TRIALS = POP_TRIALS;
    }
    srand(42);
    vector<int> pr;
    for(int i=0;i<AT_TRIALS;++i){
      pr.push_back(random(n));
    }
    clog << n << " of " << MAX_N << endl;
    vector<AVector *> implementations;
    for(int t=0;t<max(PUSH_TRIALS,POP_TRIALS);++t){
      implementations.push_back(factory->create());
    }
    clog << "created" << endl;

    clock_t pushStart = clock(); 
    for(int t=0;t<PUSH_TRIALS;++t){
      for(int j=0;j<n;++j){
        implementations[t]->push(42);
      }
    }
    clock_t pushTime = clock() - pushStart;
    clog << "pushed" << endl;

    for(int t=PUSH_TRIALS;t<POP_TRIALS;++t){
      for(int j=0;j<n;++j){
        implementations[t]->push(42);
      }
    }
      

    int * p = &pr[0];
    clock_t atStart = clock();
    for(int j=0;j<AT_TRIALS;++j){
      cs+=implementations[0]->at(*(p++));
    }
    clock_t atTime = clock() - atStart;
    clog << "ated" << endl;

    clock_t popStart = clock();
    for(int t=0;t<POP_TRIALS;++t){
      for(int j=0;j<n;++j){
        implementations[t]->pop();
      }
    }
    clock_t popTime = clock() - popStart;
    clog << "poped" << endl;

    for(int t=0;t<(int)implementations.size();++t){
      delete implementations[t];
    }
    results[n].pushTimes.push_back(pushTime);
    results[n].popTimes.push_back(popTime);
    results[n].atTimes.push_back(atTime);
  }
  clog << "check sum " << cs << endl;
}
int main(int argc,const char * args[]){
  if(argc<4){
    cerr 
      << "Syntax is\n" 
      << args[0] << " MAX_N REPEAT (regular|succinct K|waterloo)" << endl;
    return 1;
  }
  AVectorFactory * factory;
  int MAX_N = atoi(args[1]);
  int REPEAT = atoi(args[2]);
  string name(args[3]);
  if(name=="regular"){
    factory = new RegularVectorFactory();
    clog << "Using RegularVectorFactory" << endl;
  }else if(name=="succinct"){
    if(argc<5){
      cerr << "Bad implementation" << endl;
      return 2;
    }
    int k=atoi(args[4]);
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
  for(int r=0;r<REPEAT;++r){
    clog << "ROUND " << r << endl;
    test(factory,MAX_N);
  }
  for(map<int,Info>::iterator it=results.begin();it!=results.end();++it){
    it->second.output();
  }
  return 0;
}
