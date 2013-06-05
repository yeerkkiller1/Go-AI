#pragma once

#include "Constants.h"

void RunBasicSimulations
(
__global int* intStream,
  int* pCharPos
);

void RunSpeedSimulation
(
__global int* intStream,
  int* pCharPos,
  int cSimulations,
  int seed
);