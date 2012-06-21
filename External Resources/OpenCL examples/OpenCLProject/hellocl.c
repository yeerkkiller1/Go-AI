/*

File: compute_sumints.c

Abstract: source for compute_sum kernel and initialization and runtime 
           code for summing integers in and OpenCL kernel

Version: 1.0

Disclaimer: IMPORTANT:  This Apple software is supplied to you by 
Apple Inc. ("Apple") in consideration of your agreement to the
following terms, and your use, installation, modification or
redistribution of this Apple software constitutes acceptance of these
terms.  If you do not agree with these terms, please do not use,
install, modify or redistribute this Apple software.

In consideration of your agreement to abide by the following terms, and
subject to these terms, Apple grants you a personal, non-exclusive
license, under Apple's copyrights in this original Apple software (the
"Apple Software"), to use, reproduce, modify and redistribute the Apple
Software, with or without modifications, in source and/or binary forms;
provided that if you redistribute the Apple Software in its entirety and
without modifications, you must retain this notice and the following
text and disclaimers in all such redistributions of the Apple Software. 
Neither the name, trademarks, service marks or logos of Apple Inc. 
may be used to endorse or promote products derived from the Apple
Software without specific prior written permission from Apple.  Except
as expressly stated in this notice, no other rights or licenses, express
or implied, are granted by Apple herein, including but not limited to
any patent rights that may be infringed by your derivative works or by
other works in which the Apple Software may be incorporated.

The Apple Software is provided by Apple on an "AS IS" basis.  APPLE
MAKES NO WARRANTIES, EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION
THE IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY AND FITNESS
FOR A PARTICULAR PURPOSE, REGARDING THE APPLE SOFTWARE OR ITS USE AND
OPERATION ALONE OR IN COMBINATION WITH YOUR PRODUCTS.

IN NO EVENT SHALL APPLE BE LIABLE FOR ANY SPECIAL, INDIRECT, INCIDENTAL
OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) ARISING IN ANY WAY OUT OF THE USE, REPRODUCTION,
MODIFICATION AND/OR DISTRIBUTION OF THE APPLE SOFTWARE, HOWEVER CAUSED
AND WHETHER UNDER THEORY OF CONTRACT, TORT (INCLUDING NEGLIGENCE),
STRICT LIABILITY OR OTHERWISE, EVEN IF APPLE HAS BEEN ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Copyright (C) 2008 Apple Inc. All Rights Reserved.

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
//#include <bool>
#include <sys/types.h>
#include <sys/stat.h>
#include <OpenCL/opencl.h>
#include <mach/mach_time.h>

static cl_device      device;
static cl_context      context;

const char *sum_kernel_code = 
"__kernel void compute_sum(__global int *a, int n, __local long *tmp_sum, __global long *sum)\n"
"{\n"
"    int  tid = get_local_thread_id(0);\n"
"    int  lsize = get_local_thread_size(0);\n"
"    int  i;\n"
"\n"
"    tmp_sum[tid] = 0;\n"
"    for (i=tid; i<n; i+=lsize)\n"
"        tmp_sum[tid] += a[i];\n"
"\n"
"    for (i=lsize/2; i>0; i/=2)\n"
"    {\n"
"        barrier(CL_GLOBAL_MEM_FENCE);\n"
"        if (tid < i)\n"
"            tmp_sum[tid] += tmp_sum[tid + i];\n"
"    }\n"
"\n"
"    if (tid == 0)\n"
"        *sum = tmp_sum[0];\n"
"}\n";


static int
verify_sum(int *inptr, long long *outptr, int n)
{
    long long   r = 0;
    int         i;
    
    for (i=0; i<n; i++)
    {
      r += inptr[i];
    }
   
   if (r != outptr[0])
   {
      printf("sum of ints test failed\n");
      return -1;
   }
    
    printf("sum of ints test passed\n");
    return 0;
}

int
compute_sumints(int num_elements, long long *compute_sum, float *compute_time)
{
	int result
	err = fmt.Scanf("%d", &result)
	fmt.Println(
   cl_mem                  streams[2];
   long long               sum;
   int                     *input_ptr;
   cl_program               program;
   cl_kernel               kernel;
   void                  *values[4];
   size_t                  sizes[4] = { sizeof(cl_mem), sizeof(int), 0, sizeof(cl_mem) };
   size_t                  lengths[1];
   unsigned int            global_threads[1];
   unsigned int            local_threads[1];
   int                     err;
   unsigned int            max_threadgroup_size;
   int                     i;
   cl_device_id            device_id;
   uint64_t               t0, t1;
   struct mach_timebase_info   info;
   
   mach_timebase_info(&info);
   
   printf( "computing sum for %d randomly generated ints\n", num_elements );
   input_ptr = malloc(sizeof(int) * num_elements);
   for (i=0; i<num_elements; i++)
      input_ptr[i] = (int)rand();
   
   err = clGetDeviceGroupInfo(device, CL_DEVICE_IDS, &device_id, sizeof(cl_device_id), NULL);
   if (err != CL_SUCCESS) {
      printf( "clGetDeviceGroupInfo failed\n" );
      return -1;
   }

   clGetDeviceConfigInfo(device_id, CL_DEVICE_MAX_THREAD_GROUP_SIZE, &max_threadgroup_size, sizeof(unsigned int), NULL);

   lengths[0] = strlen(sum_kernel_code);
    program = clCreateProgramWithSource(device, 1, &sum_kernel_code, lengths);
   if (!program)
   {
      printf("clCreateProgramWithSource failed\n");
      return -1;
   }
   
   err = clBuildProgramExecutable(program, false, NULL, NULL);
   if (err != CL_SUCCESS)
   {
      printf("clBuildProgramExecutable failed\n");
      return -1;
   }
   
   kernel = clCreateKernel(program, "compute_sum");
   if (!kernel)
   {
      printf("clCreateKernel failed\n");
      return -1;
   }

   streams[1] = clCreateArray(device, CL_MEM_ALLOC_GLOBAL_POOL, sizeof(long long), 1, NULL);
   if (!streams[1])
   {
      printf("clCreateArray failed\n");
      return -1;
   }
   
   // begin timing
   t0 = mach_absolute_time();
   streams[0] = clCreateArray(device, CL_MEM_ALLOC_GLOBAL_POOL | CL_MEM_COPY_HOST_PTR, sizeof(int), num_elements, input_ptr);
   if (!streams[0])
   {
      printf("clCreateArray failed\n");
      return -1;
   }
      
   sizes[0] = sizeof(cl_mem);  values[0] = streams[0];
   sizes[1] = sizeof(int);     values[1] = (void *)&num_elements;
   sizes[2] = max_threadgroup_size*sizeof(long long); values[2] = NULL;
   sizes[3] = sizeof(cl_mem);  values[3] = streams[1];
   err = clSetKernelArgs(context, kernel, 4, NULL, values, sizes);
   if (err != CL_SUCCESS)
   {
      printf("clSetKernelArgs failed\n");
      return -1;
   }

   global_threads[0] = max_threadgroup_size;
   local_threads[0] = max_threadgroup_size;
    err = clExecuteKernel(context, kernel, NULL, global_threads, local_threads, 1, NULL, 0, NULL);
    if (err != CL_SUCCESS)
    {
        printf("clExecuteKernel failed\n");
        return -1;
    }
   
    err = clReadArray(context, streams[1], false, 0, sizeof(long long), (void *)&sum, NULL);
    if (err != CL_SUCCESS)
    {
        printf("clReadArray failed\n");
        return -1;
    }
    // end timing
   t1 = mach_absolute_time();
   
   {
      
      if (compute_time) *compute_time = 1e-9 * (t1 - t0) * info.numer / info.denom;
   }
    err = verify_sum(input_ptr, &sum, num_elements);
   
   if (compute_sum) *compute_sum = sum;
    
   // cleanup
   clReleaseMemObject(streams[0]);
   clReleaseMemObject(streams[1]);
   clReleaseKernel(kernel);
   clReleaseProgram(program);
   free(input_ptr);
   
   return err;
}

int
init_compute()
{
   cl_device_id compute_device_id[2];
   unsigned int num_devices = 0;
   int return_value = 0;
   
    return_value = clGetComputeDevices(CL_DEVICE_TYPE_GPU, 2, compute_device_id, &num_devices);
    if(return_value || 0 == num_devices) {
      printf( "clGetComputeDevices failed (with %d devices available)\n", num_devices );
        return -1;
   }
   
    device = clCreateDeviceGroup(1, &compute_device_id[0]);
   if (!device)
   {
      printf("clCreateDeviceGroup failed\n");
      return -1;
   }
   
   context = clCreateContext(0, device);
   if (!context)
   {
      printf("clCreateContext failed\n");
      return -1;
   }
   
   return 0;
}

void release_compute()
{
   clReleaseContext(context);
   clReleaseDeviceGroup(device);
}

#if 0
int
main(int argc, char *argv[])
{
   if (init_compute())
      return -1;
   
   int r = compute_sumints(1024*1024, NULL, NULL);
   release_compute();
   return r;
}
#endif
