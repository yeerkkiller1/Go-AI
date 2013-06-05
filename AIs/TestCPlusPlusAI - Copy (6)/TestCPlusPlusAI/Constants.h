#pragma once

#define BOARD_SIZE 9
#define TOTAL_SQUARES 81

#define ERROR_CHECKING
#define ERR

#ifndef C
  #define C
#endif

#define bool int

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true  1
#endif

#ifndef OPEN_CL
#define __global
#endif

#ifndef OPEN_CL
#define __constant
#endif

#ifndef NULL
#define NULL 0
#endif

//#define byte unsigned char

#ifdef OPEN_CL
  #ifdef PROFILE
    #error "OPEN_CL and PROFILE are not compatible, do not compile both at the same time"
  #endif
#endif

//#ifdef OPEN_CL
  //#define COMPRESS_MEMORY
//#endif

#ifdef OPEN_CL
  #define LOCAL_MEM_FENCE barrier(CLK_LOCAL_MEM_FENCE)
#else
  #define LOCAL_MEM_FENCE 
#endif


  //We need enough for 81*5 squares as that is the max sum of liberties and
  //pieces played (it is actually a bit less, but this is fine)
//#define cSquareIntPositions 102
#ifdef COMPRESS_MEMORY
  #define cSquareIntPositions 1
#else
  #define cSquareIntPositions 21
#endif

#define cSquarePositions 408