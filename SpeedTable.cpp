//Sorry, I know this is ugly :(
#include "Common.h"
struct Info{
  int n;
  int trials;
  vector<vector<clock_t> > totalTimes;
  void output(bool withTrials){
    cout << n;
    if(withTrials){
      cout << "\t&" << trials;
    }
    
    for(int f = 0; f< (int)totalTimes.size(); ++f){
      vector<clock_t> times = totalTimes[f];
      double m = median(times); 
      cout << "\t&" << m << "\t& $" << (int) (m/trials*1000000) << "\\mu$" ;
      for(int i=0;i<(int)times.size();++i){
        clog << ' ' << times[i]-m ;
      }
      clog << endl;
    }
    cout << "\\\\" << endl;
  }
};
map<int, Info> results;

void test(vector<AVectorFactory *> factories,int n, string what){
  int cs=0;
  const int TRIALS = 1000000;
  const int AT_TRIALS = what == "at" ? TRIALS*10 : 0;
  const int PUSH_TRIALS = what == "push" ? max(TRIALS / n,1):1;
  const int POP_TRIALS = what == "pop" ? max(TRIALS / n,1):1;
  if( results.find(n) == results.end()){
    results[n].n = n;
    results[n].trials = -1;
    results[n].totalTimes.resize(factories.size(),vector<clock_t>());
  }

  srand(42);
  vector<int> pr;
  for(int i=0;i<AT_TRIALS;++i){
    pr.push_back(random(n));
  }

  for(int f=0;f<(int)factories.size();++f){
    AVectorFactory * factory = factories[f];
    clock_t totalTime;
    int trials;
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
    if(what == "at"){
      totalTime = atTime;
      trials = AT_TRIALS;
    }
    if(what == "pop"){
      totalTime = popTime;
      trials = POP_TRIALS*n;
    }
    if(what == "push"){
      totalTime = pushTime;
      trials = PUSH_TRIALS*n;
    }
    results[n].totalTimes[f].push_back(totalTime);
    results[n].trials = trials;
  }
  clog << "check sum " << cs << endl;
}
int main(int argc,const char * args[]){
  if(argc<4){
    cerr 
      << "Syntax is\n" 
      << args[0] << " MAX_N REPEAT (at|pop|push)" << endl;
    return 1;
  }
  int MAX_N = atoi(args[1]);
  int REPEAT = atoi(args[2]);
  string what(args[3]);
  if(what != "at" && what!="pop" && what!="push"){
    cerr << "Bad operation" << endl;
    return 2;
  }
  vector<AVectorFactory*> implementations;
  implementations.push_back(new RegularVectorFactory());
  implementations.push_back(new WaterlooVectorFactory());
  implementations.push_back(new SuccinctVectorFactory<2>());
  implementations.push_back(new SuccinctVectorFactory<5>());
  for(int r=0;r<REPEAT;++r){
    clog << "ROUND " << r << endl;
    for(int n=10;n<=MAX_N;n*=10){
      test(implementations,n,what);
    }
  }
  for(map<int,Info>::iterator it=results.begin();it!=results.end();++it){
    it->second.output(what != "at");
  }
  return 0;
}
