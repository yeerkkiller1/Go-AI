#include "IntStream.h"

void WriteCharToIntStream
(
__global int* intStream,
  int* pCharPos,
  unsigned char charValue
)
{
  int intOffset = *pCharPos % 4;

  int charAsInt = charValue << ((intOffset) * 8);
  int charMaskInverted = ((1 << 8) - 1) << ((intOffset) * 8);
  int charMask = (-1) ^ charMaskInverted;

  int intPos = *pCharPos / 4;
  
  intStream[intPos] &= charMask;
  intStream[intPos] |= charAsInt;

  *pCharPos = *pCharPos + 1;
  
}

char GetCharFromIntStream
(
__global int* intStream,
  int charPos
)
{
  int intOffset = charPos % 4;

  char charValue = (char)(intStream[charPos / 4] >> (intOffset * 8));
  return charValue;
}

void WriteStringToIntStream
(
__global int* intStream,
  int* pCharPos,
  const __constant char* stringToWrite
)
{
  int curPos = 0;

  while(stringToWrite[curPos] != '\0')
  {
    WriteCharToIntStream(intStream, pCharPos, stringToWrite[curPos++]);
  }
}

//Does not round, simply trims the float
void WriteFloatToIntStream
(
__global int* intStream,
  int* pCharPos,
  float number,
  int decimalPlaces
)
{
  //We need to find the first digit, and so the largest digit.
  //Increase the magnitude until we find it
  float curMagnitude = 1;

  if(number < 0)
  {
    WriteCharToIntStream(intStream, pCharPos, '-');
    number *= -1;
  }

  while(number / curMagnitude >= 10)
  {
    curMagnitude *= 10;
  }

  while(curMagnitude >= 1 || decimalPlaces > 0)
  {
    int curDigit = number / curMagnitude;
    WriteCharToIntStream(intStream, pCharPos, (char)(curDigit + 48));
    number -= curDigit * curMagnitude;
    curMagnitude /= 10;

    if(curMagnitude < 0.5 && curMagnitude > 0.05)
    {
      if(decimalPlaces > 0)
        WriteCharToIntStream(intStream, pCharPos, '.');      
    }
    else if(curMagnitude < 0.05)
    {
      decimalPlaces--;
    }
    
  }  
}

/*
void WriteCharToIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int* pCharPos,
  char charValue
)
{
  int intOffset = *pCharPos % 4;

  int charAsInt = charValue << ((intOffset) * 8);
  int charMaskInverted = ((1 << 8) - 1) << ((intOffset) * 8);
  int charMask = (-1) ^ charMaskInverted;

  intStream[*pCharPos / 4] &= charMask;
  intStream[*pCharPos / 4] |= charAsInt;

  (*pCharPos)++;
}

char GetCharFromIntStream
(
#ifdef OPEN_CL 
__global int* intStream,
#else
  int* intStream,
#endif  
  int charPos
)
{
  int intOffset = charPos % 4;

  char charValue = (char)(intStream[charPos / 4] >> (intOffset * 8));
  return charValue;
}

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
)
{
  int curPos = 0;

  while(stringToWrite[curPos] != '\0')
  {
    WriteCharToIntStream(intStream, pCharPos, stringToWrite[curPos++]);
  }
}

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
)
{
  //We need to find the first digit, and so the largest digit.
  //Increase the magnitude until we find it
  float curMagnitude = 1;
  int x;

  if(number < 0)
  {
    WriteCharToIntStream(intStream, pCharPos, '-');
    number *= -1;
  }

  while(number / curMagnitude >= 10)
  {
    curMagnitude *= 10;
  }

  while(curMagnitude >= 1 || decimalPlaces > 0)
  {
    int curDigit = number / curMagnitude;
    WriteCharToIntStream(intStream, pCharPos, (char)(curDigit + 48));
    number -= curDigit * curMagnitude;
    curMagnitude /= 10;

    if(curMagnitude < 0.5 && curMagnitude > 0.05)
    {
      if(decimalPlaces > 0)
        WriteCharToIntStream(intStream, pCharPos, '.');      
    }
    else if(curMagnitude < 0.05)
    {
      decimalPlaces--;
    }
    
  }  
}
*/