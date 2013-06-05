#ifdef PROFILE

#include "Tests.h"

#include "MontecarloSimulation.h"

#include "IntStream.h"

#include "FastRandom.h"

#include <math.h>

#include <stdio.h>

float GetAverage(int* data, int count)
{
  float sum = 0;
  int x;
  for(x = 0; x < count; x++)
    sum += data[x];
  return sum / count;
}

float GetSampleStandardDeviation(int* data, int count)
{
  float standardDeviation = 0;
  int x;

  float average = GetAverage(data, count);

  for(x = 0; x < count; x++)
  {
    standardDeviation += (average - data[x]) * (average - data[x]);
  }

  return sqrt(standardDeviation / (count - 1));
}

void CreateDataOutput(int* data, int count, int* outputStream, int* pCharPos)
{ 
  int x;
  for(x = 0; x < count; x++)
  {
    WriteFloatToIntStream(outputStream, pCharPos, (float)data[x], 0);
    WriteCharToIntStream(outputStream, pCharPos, '\n');
  }
}

void CreateDataFloatOutput(float* data, int count, int* outputStream, int* pCharPos, int decimalPlaces)
{ 
  int x;
  for(x = 0; x < count; x++)
  {
    WriteFloatToIntStream(outputStream, pCharPos, data[x], decimalPlaces);
    WriteCharToIntStream(outputStream, pCharPos, '\n');
  }
}
float GetDistributionCoefficient(float targetCertainty);
void TestEarlyTermination(float targetCertainty, int targetValue, float numberOfTrials)
{  
  int* intStream = (int *)calloc(1000000, sizeof(int));
  int charPos = 0;
    
  float* metaStream = (float *)calloc(1000000, sizeof(float));
  int metaCharPos = 0;

  float* metaStream2 = (float *)calloc(1000000, sizeof(float));
  int metaCharPos2 = 0;

  //Start simulating until it is determined we are greater than the target certainty.
  //Then continue to check if we were right.

  //float targetCertainty = 0.75;
  //float targetValue = 5; //We need to be targetCertainty which side of 5 we are on

  //http://en.wikipedia.org/wiki/Confidence_interval 
  //1.96 is for 95% confidence interval (95% of the time within this interval)
  //2.23648 //95% better
  //2.2414  //95% better

  //1.28155 //80% (but yields 75%?)
  //1.64485 //80% better (yields 85.5%?)
  //2.920 (90%)
  //2.015 (85%)
  //t-distribution with 1 degree of freedom (one-sided) appears to be the best...
  //which means the formula is the same as the standard formula but
  //the constant is -cot(pi * certainty percent)
  float confidenceConstant = GetDistributionCoefficient(targetCertainty);

  float totalGuesses = 0;
  float infiniteGuesses = 20000;
  float realAverage = 6.8727;

  float correctGuesses = 0;
  float guessesSkipped = 0;
  //float numberOfTrials = 1000;

  float averageGuessNumber = 0;

  int x;
  int y;
  int z;

  unsigned long mt[N];
  int mti = N + 1;

  init_genrand(2349217, mt, &mti);

  for(x = 0; x < numberOfTrials; x++)
  {
    bool guessed = false;
    bool above = false;
    *GetProfileStreamPos() = 0;
    for(y = 0; y < infiniteGuesses; y++)
    {
      charPos = 0;      
      RunSpeedSimulation(intStream, &charPos, 1, genrand_int32(mt, &mti));

      if(!guessed)
      {
        float standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());
        float average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
        //This can of course be greatly optimized...
        float intervalRange = confidenceConstant*(standardDeviation/sqrt((float)(y + 1)));
        float lowerEndpoint = average - intervalRange;
        float upperEndpoint = average + intervalRange;

        if(upperEndpoint < targetValue)
        {
          guessed = true;          
          above = false;

          averageGuessNumber += (y + 1);
          break;
        }
        else if(lowerEndpoint >= targetValue)
        {
          guessed = true;
          above = true;

          averageGuessNumber += (y + 1);
          break;
        }
      }
    }

    {
      if(!guessed)
      {
        guessesSkipped++;
      }
      else
      {
        if(above && realAverage >= 5)
        {
          correctGuesses++;
        }
        else if(!above && realAverage < 5)
        {
          correctGuesses++;
        }
      }  
    }
  }

  {
    int stream[1000];
    int pos = 0;
    float percentCorrect = (float)correctGuesses / numberOfTrials;
    float percentSkipped = (float)guessesSkipped / numberOfTrials;
    averageGuessNumber /= numberOfTrials;
    metaCharPos = 0;
    WriteStringToIntStream(stream, &pos, "\n\nPercent correct: ");
    WriteFloatToIntStream(stream, &pos, percentCorrect, 5);
    WriteStringToIntStream(stream, &pos, "\nTarget Percent correct: ");
    WriteFloatToIntStream(stream, &pos, targetCertainty, 5);
    WriteStringToIntStream(stream, &pos, "\nPercent skipped: ");
    WriteFloatToIntStream(stream, &pos, percentSkipped, 5);
    WriteStringToIntStream(stream, &pos, "\nAverage guess number: ");
    WriteFloatToIntStream(stream, &pos, averageGuessNumber, 5);
    WriteStringToIntStream(stream, &pos, "\nDistribution coefficient: ");
    WriteFloatToIntStream(stream, &pos, confidenceConstant, 5);
    WriteCharToIntStream(stream, &pos, ' ');
    WriteCharToIntStream(stream, &pos, '\0');

    printf("%s", (char*)stream);
  }

  //float intervalRange = 1.28155*(standardDeviation/sqrt((float)subSamples));
  //float lowerEndpoint = average - intervalRange;
  //float upperEndpoint = average + intervalRange;
}

float GetDistributionCoefficient(float targetCertainty)
{
  return -1 / tan(3.1415926535897932384626433 * targetCertainty);
}
void TestDistributions()
{  
  int* profileStream = GetProfileStream();
  (*GetProfileStreamPos()) = 0;

  return;

  TestEarlyTermination(0.50, 6, 1000);
  TestEarlyTermination(0.60, 6, 1000);
  TestEarlyTermination(0.70, 6, 1000);
  TestEarlyTermination(0.80, 6, 1000);
  TestEarlyTermination(0.90, 6, 1000);
  TestEarlyTermination(0.95, 6, 1000);

  (*GetProfileStreamPos()) = 0;

  {

    int* intStream = (int *)calloc(1000000, sizeof(int));
    int charPos = 0;
    int cSimulations = 10000;

    //We do a lot of simulations, and then compile the results in the metaStream
    //BASICALLY: The less the trials vary the more we can rely on them and so the less we need to run
    //we don't bother checking the variance in the individual trials... (although it is likely possible to calculate
    //what we need from that, its just harder), instead we do lots of trial runs to determine how different sample
    //sizes impact the result. Then we will pick one that looks good. It would be ideal to do this at run time
    //as the board will change the number of samples we need to run... but for now there is no need.
    float* metaStream = (float *)calloc(1000000, sizeof(float));
    int metaCharPos = 0;

    float* metaStream2 = (float *)calloc(1000000, sizeof(float));
    int metaCharPos2 = 0;

    unsigned long mt[N];
    int mti = N + 1;

    init_genrand(2349217, mt, &mti);

    RunSpeedSimulation(intStream, &charPos, 60000, genrand_int32(mt, &mti));
    charPos = 0;
    CreateDataOutput(GetProfileStream(), *GetProfileStreamPos(), intStream, &charPos);

    {
      int numberOfSamplings = 20000;
      int samples = 100;
      int subSamples = 10;
      int x;
      float average;
      float standardDeviation;

      //float standardMaxStandardDeviation;

      //int standardDeviationSamples = 10000;

      //The average and standard deviation themselves are inexact, so we create a confidence interval around the
      //the standard deviation based on a large number of samples, and use this to increase or decrease the standard deviation
      //calculated in future samplings

      //charPos = 0;
      //*GetProfileStreamPos() = 0;
      //RunSpeedSimulation(intStream, &charPos, standardDeviationSamples, genrand_int32(mt, &mti));

      //average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
      //standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());

      //standardMaxStandardDeviation = average + 1.96*(standardDeviation/sqrt((float)standardDeviationSamples));

      //for(x = 0; x < numberOfSamplings; x++)
      {        
        charPos = 0;
        *GetProfileStreamPos() = 0;
        RunSpeedSimulation(intStream, &charPos, samples, genrand_int32(mt, &mti));

        average = GetAverage(GetProfileStream(), *GetProfileStreamPos());
        standardDeviation = GetSampleStandardDeviation(GetProfileStream(), *GetProfileStreamPos());

        {
          //http://en.wikipedia.org/wiki/Confidence_interval 
          //1.96 is for 95% confidence interval (95% of the time within this interval)
          //2.23648 //95% better
          //2.2414  //95% better

          //1.28155 //80% (but yields 75%?)
          //1.64485 //80% better (yields 85.5%?)
          float intervalRange = 1.28155*(standardDeviation/sqrt((float)subSamples));
          float lowerEndpoint = average - intervalRange;
          float upperEndpoint = average + intervalRange;

          int amountInInterval = 0;
          int amountOutofInterval = 0;

          for(x = 0; x < numberOfSamplings; x++)
          {
            charPos = 0;
            *GetProfileStreamPos() = 0;
            RunSpeedSimulation(intStream, &charPos, subSamples, genrand_int32(mt, &mti));

            {
              float result = GetAverage(GetProfileStream(), *GetProfileStreamPos());

              if(result > lowerEndpoint && result < upperEndpoint)
              {
                amountInInterval++;
              }
              else
              {
                amountOutofInterval++;
              }
            }
          }

          {
            int stream[1000];
            int pos = 0;
            float percentCorrect = (float)amountInInterval / numberOfSamplings;
            float percentInCorrect = (float)amountOutofInterval / numberOfSamplings;
            metaCharPos = 0;
            WriteStringToIntStream(stream, &pos, "\n\nPercent correct: ");
            WriteFloatToIntStream(stream, &pos, percentCorrect, 5);
            WriteCharToIntStream(stream, &pos, ' ');
            WriteCharToIntStream(stream, &pos, '\0');

            printf("%s", (char*)stream);
          }
        }
      }

      charPos = 0;
      CreateDataFloatOutput(metaStream, metaCharPos, intStream, &charPos, 3);
      charPos = 0;
      CreateDataFloatOutput(metaStream2, metaCharPos2, intStream, &charPos, 3);
    }


    RunSpeedSimulation(intStream, &charPos, cSimulations, 73471);

    charPos = 0;
    CreateDataOutput(GetProfileStream(), *GetProfileStreamPos(), intStream, &charPos);

    RunSpeedSimulation(intStream, &charPos, cSimulations, 73470);
  }
}

#endif