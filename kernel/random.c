#include "random.h"
#include "../cpu/timer.h"
#include "../cpu/types.h"

//TODO: Switch to using an actual random number generator

//The below link is a paper written by Keio Uni, department of mathematics
//that describes a method of generating random numbers called the 
//mersenne twister. (It is based on mersenne primes 2^p-1) 
//http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/ARTICLES/mt.pdf

//Period parameters
#define N 624
#define M 397
#define MATRIX_A 0x9908b0df
#define UPPER_MASK 0x80000000
#define LOWER_MASK 0x7fffffff

//Tempering parameters
#define TEMPERING_MASK_B 0x9d2c5680
#define TEMPERING_MASK_C 0xefc60000
#define TEMPERING_SHIFT_U(y) (y >> 11)
#define TEMPERING_SHIFT_S(y) (y << 7)
#define TEMPERING_SHIFT_T(y) (y << 15)
#define TEMPERING_SHIFT_L(y) (y >> 18)

static u32 mt[N];
static u16 mti = N + 1;

void sgenrand(u32 seed){
  seed = seed << 24;
  mt[0] = seed & 0xffffffff;
  for(mti = 1; mti < N; mti++)
    mt[mti] = (69069 * mt[mti-1]) & 0xffffffff;
}

double genrand(){
  u32 y;
  static u32 mag01[2] = {0x0, MATRIX_A};

  if (mti >= N){
    int k;

    if(mti == N + 1){
      sgenrand(4367);
    }

    for (k = 0; k < N-M; k++){
      y = (mt[k] & UPPER_MASK)|(mt[k+1] & LOWER_MASK);
      mt[k] = mt[k+M] ^ (y >> 2) ^ mag01[y & 0x1];
    }
    for (;k < N-1; k++){
      y = (mt[N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
      mt[k] = mt[k + (M-N)] ^ (y >> 1) ^ mag01[y & 0x1];
    }
    y = (mt[N-1] & UPPER_MASK) | (mt[0] & LOWER_MASK);
    mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1];

    mti = 0;
  }
  y = mt[mti++];
  y ^= (y >> 11);
  y ^= (y << 7) & TEMPERING_MASK_B;
  y ^= (y << 15) & TEMPERING_MASK_C;
  y ^= (y >> 18);

  return y;
}


//Temporary random number generator
int rand_int(int range){
  return (u32)(genrand()) % range;
}
