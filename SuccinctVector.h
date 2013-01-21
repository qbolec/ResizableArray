#ifndef SUCCINCT_VECTOR_H
#define SUCCINCT_VECTOR_H

#define PRECOMPUTED_MIN_SIZE
#define PRECOMPUTED_BIG_BUFF_SIZE
template<int k>
class SuccinctVector : public AVector{
private:
  int s;
#ifdef PRECOMPUTED_MIN_SIZE    
  unsigned int sizeMin;
#endif
#ifdef PRECOMPUTED_BIG_BUFF_SIZE
  size_t bigBuffSize;
#endif
  //buffers[i] contains buffers of length s^i

  //buffersLen[i] = how many buffers do we have in i-th block? (alpha_i)
  //buffersLen[0] = exceptionally is the number of elements in buffers[0][0]
  int buffersLen[k+1];
  //buffers[i] contains at most 2*s-1 pointers to buffers in i-th block.
  //           They are aranged so that buffer[i][0] is adjanced to buffer[i+1].
  //           That is the ordering of blocks is from k down to 0,
  //           while the ordering of buffers in from 0 to 2*s-1.
  //           TODO: simplify it :)
  //buffers[0] exceptionally has only a single buffer of length 2*s-1  
  //           instead of 2*s-1 buffers of length 1. This is a performance decision.
  //buffers[k] is the block of the longest buffers (of length s^k)
  int ** buffers[k+1];
  void pop_rec(int last,int h){
    if(0==buffersLen[last]){
      const int next = last+1;
      pop_rec(next,h*s);
      int * big_buffer = buffers[next][--buffersLen[next]];
      buffers[next][buffersLen[next]] = NULL;
      if(0<last){
        for(int i=0;i<s;++i){
          assert(NULL == buffers[last][i]);
          buffers[last][i] = new int[h];
          memcpy(buffers[last][i],big_buffer+i*h,sizeof(int)*h);            
        }
      }else{          
        memcpy(buffers[0][0],big_buffer,sizeof(int)*h*s);          
      }
      buffersLen[last]=s;
      delete[] big_buffer;      
    }
  }
  void push_rec(int last,int h){
    if(buffersLen[last]==2*s-1){
      const int next = last+1;
      push_rec(next,h*s);
      int * big_buffer = new int[h*s];
      buffers[next][buffersLen[next]++] = big_buffer;
      if(0<last){
        for(int i=0;i<s;++i){
          memcpy(big_buffer+i*h,buffers[last][i],sizeof(int)*h);
          delete[] buffers[last][i];
          if (i+s < 2*s -1) {
            buffers[last][i] = buffers[last][i+s];
            buffers[last][i+s] = NULL;
          }else{
            buffers[last][i] = NULL;
          }
        }
      }else{
        memcpy(big_buffer,buffers[0][0],sizeof(int)*h*s);
        memcpy(buffers[0][0],buffers[0][0]+s,sizeof(int)*h*s);
      }
      buffersLen[last]-=s;      
    }    
  }
  bool is_full(){
    for(int i=0;i<=k;++i){
      if(buffersLen[i]<2*s-1){
        return false;
      }
    }
    return true;
  }
  void adapt_s(){
    int n = size();
    int new_s = max(2,(int)ceil(pow(n,1.0/k)));  
    //clog << "Adapting to size=" << size() << " s=" << s << " to new s=" << new_s << endl;
    if(s != new_s){
      adapt(new_s);
    }
  }
  void adapt(int new_s){
    SuccinctVector<k> newThis(new_s);
    int n = size();
    //There are many possible representations of n in s-ary.
    //We could choose any of them.
    //I am a bit lazy, so I simply use push to move items to the new structure.
    //This is a way which is proven to be correct and fast enough.
    //TODO: change it to (s+new_s)*k memcpy calls, and prove it is faster.
    for(int i=0;i<n;++i){
      newThis.push(at(i));
    }
    //now we still (swap) representation of the new vector
    for(int i=0;i<=k;++i){
      swap(buffers[i],newThis.buffers[i]);
      swap(buffersLen[i],newThis.buffersLen[i]);
    }

    swap(s,newThis.s);
#ifdef PRECOMPUTED_MIN_SIZE    
    swap(sizeMin,newThis.sizeMin);
#endif
#ifdef PRECOMPUTED_BIG_BUFF_SIZE
    swap(bigBuffSize,newThis.bigBuffSize);
#endif
    //destructor of newThis will actually destroy our old data.
  }
  bool s_is_too_large(){
    /*

    s > 3  sqrt[k] size
    (s/3) ^k  >  size
    k log s - k log 3  > log size

    */

    //bool a = s> 3.0*pow(size(),1.0/k) && s>2;
#ifdef PRECOMPUTED_MIN_SIZE    
    bool b = sizeMin > size() && s>2;
#else    
    bool b = pow(s/3.0,k) > size() && s>2;
#endif
    //assert(a==b);
    return b;
  }
public:
  SuccinctVector(int s=2):
     s(s)
#ifdef PRECOMPUTED_MIN_SIZE    
    ,sizeMin((unsigned int)ceil(pow(s/3.0,k)))
#endif
#ifdef PRECOMPUTED_BIG_BUFF_SIZE
    ,bigBuffSize((size_t)pow((double)s,k))
#endif
    {
    buffers[0] = new int*[1];
    buffers[0][0] = new int[2*s-1];
    for(int i=1;i<=k;++i){
      buffers[i] = new int*[2*s-1];
      for(int j=0;j<2*s-1;++j){
        buffers[i][j]= NULL;
      }
    }    
    for(int i=0;i<=k;++i){
      buffersLen[i]=0;
    }
  }
  size_t size(){
    int sum=0;
    int h=1;
    for(int i=0;i<=k;++i){
      sum+=buffersLen[i]*h;
      h*=s;
    }
    return sum;
  }
  int & at(size_t idx){
#ifdef PRECOMPUTED_BIG_BUFF_SIZE
    size_t h = bigBuffSize;
#else
    size_t h = (size_t)pow((double)s,k);
#endif
    for(int i=k;i>=0;i--){
      if(idx < (size_t)buffersLen[i]*h){
        return i>0 ? 
          buffers[i][idx/h][idx%h] : 
          buffers[0][0][idx];
      }
      idx-=buffersLen[i]*h;
      h/=s;
    }
    throw "Undefined index?";
  }
  void push(int e){
    if(is_full()){
      adapt_s();
    }
    push_rec(0,1);
    buffers[0][0][buffersLen[0]++]=e;
  }  
  int pop(){
    pop_rec(0,1);    
    int e = buffers[0][0][--buffersLen[0]];    
    if(s_is_too_large()){
      adapt_s();
    }
    return e;
  }
  
  ~SuccinctVector(){
    delete[] buffers[0][0];
    delete[] buffers[0];
    for(int i=1;i<=k;++i){
      for(int j=0;j<2*s-1;++j){
        delete[] buffers[i][j];        
      }
      delete[] buffers[i];      
    }    
  }
  size_t allocatedSize(){
    size_t ret = sizeof(*this);
    const size_t elementSize = sizeof(int);
    const size_t pointerSize = sizeof(int*);
    ret+= pointerSize;
    ret+= elementSize * (2*s-1);
    int h=1;
      
    for(int i=1;i<=k;++i){
      h*=s;
      ret+= elementSize * buffersLen[i]*h;      
    }
    ret+= k*(2*s-1)*pointerSize;    
    return ret;
  }
  
  
};
#endif
