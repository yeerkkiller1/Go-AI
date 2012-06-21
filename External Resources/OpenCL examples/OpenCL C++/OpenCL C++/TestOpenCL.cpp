#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>

#define CL_USE_DEPRECATED_OPENCL_1_1_APIS

#include "CL\opencl.h"
#include "CL\cl.h"
#include "CL\cl.hpp"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
//#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <OpenCL/opencl.h>

#include <Windows.h>

using namespace std;

#define DATA_SIZE (1024)

void rot13 (char *buf)
{
    int index=0;
    char c=buf[index];
    while (c!=0) {
        if (c<'A' || c>'z' || (c>'Z' && c<'a')) {
            buf[index] = buf[index];
        } else {
            if (c>'m' || (c>'M' && c<'a')) {
	        buf[index] = buf[index]-13;
	    } else {
	        buf[index] = buf[index]+13;
	    }
        }
	c=buf[++index];
    }
}
int main ()
{
	FILE * cSourceFile = fopen("OpenCLSrc.txt", "r");

	long length = ftell(cSourceFile);

	char * cSourceCode = new char[length];



	fgets(cSourceCode, length, cSourceFile);

	char buf[]="Hello, World!";
	size_t srcsize, worksize=strlen(buf);
	
	cl_int error;
	cl_platform_id platform;
	cl_device_id device;
	cl_uint platforms, devices;

	// Fetch the Platform and Device IDs; we only want one.
	error=clGetPlatformIDs(1, &platform, &platforms);
	error=clGetDeviceIDs(platform, CL_DEVICE_TYPE_ALL, 1, &device, &devices);
	cl_context_properties properties[]={
		CL_CONTEXT_PLATFORM, (cl_context_properties)platform,
		0};
	// Note that nVidia's OpenCL requires the platform property
	cl_context context=clCreateContext(properties, 1, &device, NULL, NULL, &error);
	cl_command_queue cq = clCreateCommandQueue(context, device, 0, &error);
	
	rot13(buf);	// scramble using the CPU
	puts(buf);	// Just to demonstrate the plaintext is destroyed

	//char src[8192];
	//FILE *fil=fopen("rot13.cl","r");
	//srcsize=fread(src, sizeof src, 1, fil);
	//fclose(fil);
	
	const char *src= cSourceCode;
	srcsize=strlen(cSourceCode);

	const char *srcptr[]={src};
	// Submit the source code of the rot13 kernel to OpenCL
	cl_program prog=clCreateProgramWithSource(context,
		1, srcptr, &srcsize, &error);
	// and compile it (after this we could extract the compiled version)
	error=clBuildProgram(prog, 0, NULL, "", NULL, NULL);

	// Allocate memory for the kernel to work with
	cl_mem mem1, mem2;
	mem1=clCreateBuffer(context, CL_MEM_READ_ONLY, worksize, NULL, &error);
	mem2=clCreateBuffer(context, CL_MEM_WRITE_ONLY, worksize, NULL, &error);
	
	// get a handle and map parameters for the kernel
	cl_kernel k_rot13=clCreateKernel(prog, "rot13", &error);
	clSetKernelArg(k_rot13, 0, sizeof(mem1), &mem1);
	clSetKernelArg(k_rot13, 1, sizeof(mem2), &mem2);

	// Target buffer just so we show we got the data from OpenCL
	char buf2[sizeof buf];
	buf2[0]='?';
	buf2[worksize]=0;

	// Send input data to OpenCL (async, don't alter the buffer!)
	error=clEnqueueWriteBuffer(cq, mem1, CL_FALSE, 0, worksize, buf, 0, NULL, NULL);
	// Perform the operation
	error=clEnqueueNDRangeKernel(cq, k_rot13, 1, NULL, &worksize, &worksize, 0, NULL, NULL);
	// Read the result back into buf2
	error=clEnqueueReadBuffer(cq, mem2, CL_FALSE, 0, worksize, buf2, 0, NULL, NULL);
	// Await completion of all the above
	error=clFinish(cq);
	
	// Finally, output out happy message.
	puts(buf2);
}