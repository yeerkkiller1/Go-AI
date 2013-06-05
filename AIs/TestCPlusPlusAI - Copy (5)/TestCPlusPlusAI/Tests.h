#pragma once

void RunBasicSimulations
(
#ifdef OPEN_CL
__global int* intStream,
#else
int* intStream,
#endif  
  int* pCharPos
);

void RunSpeedSimulation
(
#ifdef OPEN_CL
__global int* intStream,
#else
int* intStream,
#endif  
  int* pCharPos,
  int cSimulations
);