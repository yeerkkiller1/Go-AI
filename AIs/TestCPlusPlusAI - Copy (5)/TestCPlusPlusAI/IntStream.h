#pragma once

#ifndef OPEN_CL
  #include <stdlib.h>
#endif



void WriteCharToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
  char charValue
);

char GetCharFromIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int charPos
);
void WriteStringToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
#ifdef OPEN_CL
  const __constant char* stringToWrite
#else
  const char* stringToWrite
#endif
);

//Does not round, simply trims the float
void WriteFloatToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
  float number,
  int decimalPlaces
);