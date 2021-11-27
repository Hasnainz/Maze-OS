#include "random.h"
#include "../cpu/timer.h"


//TODO: Switch to using an actual random number generator

//The below link is a paper made by Keio Uni, department of mathematics
//that outline a method of generating random numbers called the 
//mersenne twister. (It is based on mersenne primes 2^p-1) 
//http://www.math.sci.hiroshima-u.ac.jp/m-mat/MT/ARTICLES/mt.pdf

//Temporary random number generator
int rand_int(int range){
  return (get_ticks()) % range;
}
