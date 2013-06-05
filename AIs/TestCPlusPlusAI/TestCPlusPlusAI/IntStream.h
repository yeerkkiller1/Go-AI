#pragma once

#ifndef OPEN_CL
  #include <stdlib.h>
#endif

#include "Constants.h"

void WriteCharToIntStream
(
__global int* intStream,
  int* pCharPos,
  unsigned char charValue
);

char GetCharFromIntStream
(
__global int* intStream,
  int charPos
);
void WriteStringToIntStream
(
__global int* intStream,
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
__global int* intStream,
  int* pCharPos,
  float number,
  int decimalPlaces
);