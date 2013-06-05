#include "FastRandom.h"

// initializes mt[N] with a seed 
void init_genrand(unsigned long s, unsigned long* mt, int* pmti)
{
    mt[0]= s & 0xffffffffUL;
    for ((*pmti)=1; *pmti<N; (*pmti)++) {
        mt[*pmti] = 
	    (1812433253UL * (mt[*pmti-1] ^ (mt[*pmti-1] >> 30)) + *pmti); 
        // See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. 
        // In the previous versions, MSBs of the seed affect   
        // only MSBs of the array mt[].                        
        // 2002/01/09 modified by Makoto Matsumoto             
        mt[*pmti] &= 0xffffffffUL;
        // for >32 bit machines 
    }
}



// initialize by an array with array-length 
// init_key is the array for initializing keys 
// key_length is its length 
// slight change for C++, 2004/2/26 
void init_by_array(unsigned long init_key[], int key_length, unsigned long* mt, int* pmti)
{
    int i, j, k;
    init_genrand(19650218UL, mt, pmti);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; // non linear 
        mt[i] &= 0xffffffffUL; // for WORDSIZE > 32 machines 
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; // non linear 
        mt[i] &= 0xffffffffUL; // for WORDSIZE > 32 machines 
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; // MSB is 1; assuring non-zero initial array  
}


// generates a random number on [0,0xffffffff]-interval 
//mt must be of size N and mti should start off at N + 1
unsigned long genrand_int32(unsigned long* mt, int* pmti)
{
    unsigned long y;
    unsigned long mag01[2]={0x0UL, MATRIX_A};
    // mag01[x] = x * MATRIX_A  for x=0,1

    if (*pmti >= N) { // generate N words at one time 
        int kk;

        if (*pmti == N+1)   // if init_genrand() has not been called,
            init_genrand(5489UL, mt, pmti); // a default initial seed is used

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        *pmti = 0;
    }
  
    y = mt[(*pmti)++];

    // Tempering
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}


// generates a random number on [0,0x7fffffff]-interval 
long genrand_int31(unsigned long* mt, int* pmti)
{
    return (long)(genrand_int32(mt, pmti)>>1);
}

// generates a random number on [0,1]-real-interval 
double genrand_real1(unsigned long* mt, int* pmti)
{
    return genrand_int32(mt, pmti)*(1.0/4294967295.0); 
    // divided by 2^32-1  
}

// generates a random number on [0,1)-real-interval 
double genrand_real2(unsigned long* mt, int* pmti)
{
    return genrand_int32(mt, pmti)*(1.0/4294967296.0); 
    // divided by 2^32 
}

// generates a random number on (0,1)-real-interval 
double genrand_real3(unsigned long* mt, int* pmti)
{
    return (((double)genrand_int32(mt, pmti)) + 0.5)*(1.0/4294967296.0); 
    // divided by 2^32 
}

// generates a random number on [0,1) with 53-bit resolution
double genrand_res53(unsigned long* mt, int* pmti) 
{ 
    unsigned long a=genrand_int32(mt, pmti)>>5, b=genrand_int32(mt, pmti)>>6; 
    return(a*67108864.0+b)*(1.0/9007199254740992.0); 
} 
// These real versions are due to Isaku Wada, 2002/01/09 added 