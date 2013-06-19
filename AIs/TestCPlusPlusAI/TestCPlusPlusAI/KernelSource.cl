__kernel void goKernel(
   __global int* inputCL,
   __global int* outputCL,
   const int countCL
   )
{
    int i = get_global_id(0);
    {		
        int cSimulations = 1000;
        
        int charPos = outputCL[0];        
		charPos = 4;		
		
        //We have to time both... as in OpenCL we need to time both too
        RunBasicSimulations(outputCL + 1, &charPos);
        RunSpeedSimulation(outputCL + 1, &charPos, cSimulations, 73471);

		outputCL[0] = charPos;
	}	
}