

#include <time.h>


#include "q.h"

#include "MontecarloSimulation.h"


#include "FastRandom.h"

#include "Constants.h"

#include <stdio.h>

#include "Tests.h"
#include "IntStream.h"

#ifdef PROFILE
#include "Distributions.h"
#endif
//setup a build console to use new C++ 11 stuff (not sure if it will already)
//and full optimization

//Avg Sim time (milli)    1st Result    2nd   3rd     Avg Result    Avg Black Win
//(per montecarlo     
//simulation)

//0.3
//0.109                   -18           11    24      9.469         0.715

int main (int argc, char** argv)
{   

#ifndef C
  try
  {
#endif
 
  
long t1 = clock(); double time;

int cSimulations = 1000;

int intStream[10000];
int charPos = 0;

//We have to time both... as in OpenCL we need to time both too
RunBasicSimulations(intStream, &charPos);
RunSpeedSimulation(intStream, &charPos, cSimulations, 73471);
    
time = clock() - t1;

WriteCharToIntStream(intStream, &charPos, '{');
WriteFloatToIntStream(intStream, &charPos, (cSimulations * 1000) / (time), 2);
WriteStringToIntStream(intStream, &charPos, " per second");
WriteCharToIntStream(intStream, &charPos, '}');

WriteCharToIntStream(intStream, &charPos, '\0');

printf("%s", (char*)intStream);

#ifndef C
  } catch(exception e)
  {
    cout << e.what();
  }
#endif  

#ifdef PROFILE
  TestDistributions();
#endif

  //Gives you a chance to read the output
  //DASSERT(false);

  //while(true);

  return 0;
}
