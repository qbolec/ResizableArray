#include<iostream>
#include<vector>
#include<cassert>
#include<cmath>
#include<string>
#include<algorithm>
#include<map>
using namespace std;
int random(int n){
  return (((rand()<<16)^rand())&0x7FFFFF)%n;
}
double median(vector<clock_t> times){
  sort(times.begin(),times.end());//yes, I know n_th_element.
  return times[times.size()/2];
}

#include "AVector.h"
#include "RegularVectorFactory.h"
#include "SuccinctVectorFactory.h"
#include "WaterlooVectorFactory.h"
