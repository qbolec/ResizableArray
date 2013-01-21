//Pieces marked with comment "mine" are
//clarfications of things which were not in the paper
//or where wrong in the paper, and I had to do something.
//Blame me, not the authors of the paper if something is wrong here.

#include "WaterlooVector.h"
#include <cstring>
#include <cassert>

WaterlooVector::BufferInfo::BufferInfo(size_t size,size_t occupancy):
  size(size),
  occupancy(occupancy){
}

bool WaterlooVector::BufferInfo::isFull(){
  return size<=occupancy;
}

bool WaterlooVector::BufferInfo::isEmpty(){
  return 0==occupancy;
}

bool WaterlooVector::BufferInfo::isTooLarge(){
  return size>8 && //mine
    occupancy*4 <= size;
}

WaterlooVector::WaterlooVector():
  n(0),
  s(1),//mine
  //d(0) //mine : I decided not to store d explicitly as it is the same as indexBlock.occupancy 
  hasSpare(false),
  lastDataBlock(1,1),
  lastSuperBlock(1,0),
  indexBlock(8,0),//mine : I chosen 8 as starting value for s
  index(new int * [8]){//mine
}

void WaterlooVector::push(int e){
  grow();
  //clog << "grown" << endl;
  at(n-1) = e;
  //clog << "ated" << endl;
}

int & WaterlooVector::at(size_t idx){

  //clog << "at(" << idx << ")" << endl;
  size_t r = idx+1;
  int k = MSB(r);//dokładnie |r|-1
  const int H = (k+1)/2;
  const int L = k/2;
  size_t e = r & ((((size_t)1)<<H)-1);
  size_t b = (r >> H) & ((((size_t)1)<<L)-1);
  //size_t p = (((size_t)1)<<k) - 1;//theirs

  //0    1     2    3   4   5    6  7  8   
  //1    1     2    2   4   4    8  8  16
  //1    2     4    6  10  14   22 30  46

  size_t p = (((size_t)1) << H) + (((size_t)1)<<L) - 2;//mine

  //clog << "r=" <<  r << endl;
  //clog << "k=" <<  k << endl;
  //clog << "H=" <<  H << endl;
  //clog << "L=" <<  L << endl;
  //clog << "e=" <<  e << endl;
  //clog << "b=" <<  b << endl;
  //clog << "p=" <<  p << endl;
  return index[p+b][e];
}

size_t WaterlooVector::size(){
  return n;
}

size_t WaterlooVector::allocatedSize(){
  size_t total = sizeof(*this);
  total += sizeof(int *) * indexBlock.size;
  int dataBlocksNum = 1;
  int dataBlocksCnt = 0;
  int dataBlockSize = 1;
  int k = 1;
  //clog << "indexBlock.occupancy " << indexBlock.occupancy << endl;
  for(int i=0;i<indexBlock.occupancy + hasSpare;++i){
    total +=  sizeof(int) * dataBlockSize;
    ++dataBlocksCnt;
    if(dataBlocksCnt == dataBlocksNum){
      dataBlocksCnt = 0;
      ++k;
      if(k%2){
        dataBlocksNum*=2;
      }else{
        dataBlockSize*=2;
      }
    }
  }

  return total;
}

int WaterlooVector::pop(){
  int val = at(n-1);
  //clog << "ated" << endl;
  shrink();
  //cout << "shrunk" << endl;
  return val;
}

WaterlooVector::~WaterlooVector(){
  for(int i=0;i<indexBlock.occupancy+hasSpare;++i){
    delete[] index[i];
  }
  delete[] index;
}

void WaterlooVector::shrink(){
  --n;
  --lastDataBlock.occupancy;
  if(lastDataBlock.isEmpty()){
    if(hasSpare){
      delete[] index[indexBlock.occupancy];
      hasSpare = false;
    }
    if(indexBlock.isTooLarge()){
      //clog << "resizing down" << endl;
      int ** newIndex = new int * [indexBlock.size/2];
      memcpy(newIndex,index,sizeof(index[0])*indexBlock.occupancy);
      delete[] index;
      index = newIndex;
      indexBlock.size/=2;
    }
    --indexBlock.occupancy;
    --lastSuperBlock.occupancy;
    hasSpare = true;
    if(s>1 && lastSuperBlock.isEmpty()){//mine
      --s;
      if(s%2){
        lastDataBlock.size/=2;
      }else{
        lastSuperBlock.size/=2;
      }
      lastSuperBlock.occupancy=lastSuperBlock.size;
    }
    lastDataBlock.occupancy=lastDataBlock.size;
  }
}

void WaterlooVector::grow(){
  if(lastDataBlock.isFull()){
    if(lastSuperBlock.isFull()){
      ++s;
      if(s%2){
        lastSuperBlock.size*=2;
      }else{
        lastDataBlock.size*=2;
      }
      lastSuperBlock.occupancy = 0;
    }
    if(!hasSpare){
      if(indexBlock.isFull()){
        assert(!hasSpare);
        int ** newIndex = new int * [indexBlock.size*2];
        memcpy(newIndex,index,sizeof(index[0])*indexBlock.occupancy);
        delete[] index;
        index = newIndex;
        indexBlock.size*=2;
      }
      index[indexBlock.occupancy] = new int[lastDataBlock.size];
    }else{
      hasSpare = false;
    }
    ++indexBlock.occupancy;
    ++lastSuperBlock.occupancy;
    lastDataBlock.occupancy = 0;
  }
  ++n;
  ++lastDataBlock.occupancy;
}

unsigned int WaterlooVector::MSB(unsigned long long int x){
  return 63-__builtin_clzll(x);
}
