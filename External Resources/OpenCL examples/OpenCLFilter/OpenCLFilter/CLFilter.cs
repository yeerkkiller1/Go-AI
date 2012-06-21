using System;
using System.Collections.Generic;
using OpenCLTemplate;
using System.Text;

//By Douglas Andrade douglas@cmsoft.com.br
//http://www.cmsoft.com.br
//Filter examples: visit http://www.gamedev.net/reference/programming/features/imageproc/page2.asp

namespace OpenCLFilter
{
    /// <summary>Applies filter to a given image</summary>
    public static class CLFilter
    {
        /// <summary>Static class already initialized?</summary>
        private static bool Initialized = false;

        /// <summary>Apply filter kernel</summary>
        private static CLCalc.Program.Kernel kernelApplyFilter;
        /// <summary>Apply filter kernel</summary>
        private static CLCalc.Program.Kernel kernelApplyFilterWorkDim2;

        /// <summary>Filter values</summary>
        private static CLCalc.Program.Variable varFilter;

        /// <summary>Initializes class</summary>
        private static void Init(int FilterSize)
        {
            if (CLCalc.CLAcceleration == CLCalc.CLAccelerationType.Unknown) CLCalc.InitCL();

            //Compiles source code
            CLCalc.Program.Compile((new CLFilterSrc()).src);

            //Creates kernel
            kernelApplyFilter = new CLCalc.Program.Kernel("ApplyFilter");
            kernelApplyFilterWorkDim2 = new CLCalc.Program.Kernel("ImgFilter");

            //Creates filter
            varFilter = new CLCalc.Program.Variable(new float[3 * FilterSize * FilterSize]);
            //Width
            varWidth = new CLCalc.Program.Variable(new int[1]);

            Initialized = true;
        }

        private static float[] FilteredVals;
        private static CLCalc.Program.Variable varFiltered;
        private static CLCalc.Program.Variable varWidth;

        /// <summary>Applies given filter to the image</summary>
        /// <param name="imgDt">Image to be filtered</param>
        /// <param name="Filter">Filter. [3*size*size]</param>
        public static void ApplyFilter(ImageData imgDt, float[] Filter, bool useOpenCL, bool useWorkDim2)
        {
            int FilterSize = (int)Math.Sqrt(Filter.Length/3);

            if (Filter.Length != 3 * FilterSize * FilterSize)
                throw new Exception("Invalid filter");

            if (!Initialized && useOpenCL) Init(FilterSize);

            //Writes filter to device
            if(useOpenCL) varFilter.WriteToDevice(Filter);

            if (FilteredVals == null || FilteredVals.Length != imgDt.Height * imgDt.Width * 3)
            {
                //Filtered values
                FilteredVals = new float[imgDt.Height * imgDt.Width * 3];
                varFiltered = new CLCalc.Program.Variable(FilteredVals);
            }

            //Width
            if (useOpenCL) varWidth.WriteToDevice(new int[] { imgDt.Width });


            //Executes filtering
            int mean = (FilterSize - 1) / 2;
            if (useOpenCL)
            {
                CLCalc.Program.Variable[] args = new CLCalc.Program.Variable[] { imgDt.varData, varFilter, varFiltered, varWidth };
                if (useWorkDim2)
                {
                    kernelApplyFilterWorkDim2.Execute(args, new int[] { imgDt.Width - FilterSize, imgDt.Height - FilterSize });
                }
                else
                {
                    kernelApplyFilter.Execute(args, new int[] { imgDt.Height - FilterSize });
                }
                //Reads data back
                varFiltered.ReadFromDeviceTo(FilteredVals);

            }
            else
            {
                ApplyFilter(imgDt.Data, Filter, FilteredVals, new int[] { imgDt.Width }, imgDt.Height - FilterSize);
            }

            //Writes to image data
            for (int y = mean; y < imgDt.Height - mean - 1; y++)
            {
                int wy = imgDt.Width * y;
                for (int x = mean; x < imgDt.Width - mean - 1; x++)
                {
                    int ind = 3 * (x + wy);
                    imgDt.Data[ind] = (byte)FilteredVals[ind];
                    imgDt.Data[ind + 1] = (byte)FilteredVals[ind + 1];
                    imgDt.Data[ind + 2] = (byte)FilteredVals[ind + 2];
                }
            }

            //Writes filtered values
            //In the future this rewriting can be avoided
            //because byte_addressable will be widely available
            if (useOpenCL) imgDt.varData.WriteToDevice(imgDt.Data);
        }

        private const int FILTERSIZE = 7;
        private const int CENTER = 3;

        private static void ApplyFilter(byte[] ImgData,
                                  float[] Filter,
                                  float[] FilteredImage,
                                  int[] ImgWidth, int n)
        {
            for (int y = 0; y < n; y++)
            {
                int w = ImgWidth[0];

                //Image values
                float[, ,] ImgValues = new float[3, FILTERSIZE, FILTERSIZE];

                //Copies filter to local memory
                float[, ,] localfilter = new float[3, FILTERSIZE, FILTERSIZE];

                //Initialization of ImgValues
                for (int i = 0; i < FILTERSIZE; i++)
                {
                    for (int j = 0; j < FILTERSIZE; j++)
                    {
                        ImgValues[0, i, j] = ImgData[3 * ((y + i) * w + j)];
                        ImgValues[1, i, j] = ImgData[3 * ((y + i) * w + j) + 1];
                        ImgValues[2, i, j] = ImgData[3 * ((y + i) * w + j) + 2];

                        localfilter[0, i, j] = Filter[3 * (i * FILTERSIZE + j)];
                        localfilter[1, i, j] = Filter[3 * (i * FILTERSIZE + j) + 1];
                        localfilter[2, i, j] = Filter[3 * (i * FILTERSIZE + j) + 2];
                    }
                }


                int xMax = w - CENTER - 1;
                int ind = 0;
                for (int x = 0; x < xMax; x++)
                {
                    //Calculates filtered value
                    float[] filteredValue = new float[3];
                    for (int i = 0; i < FILTERSIZE; i++)
                    {
                        for (int j = 0; j < FILTERSIZE - 1; j++)
                        {
                            //              float4 pixVals = (float4)(ImgValues[0][i][j],ImgValues[1][i][j],ImgValues[2][i][j],0);
                            //              float4 filterVals = (float4)(localfilter[0][i][j],localfilter[1][i][j],localfilter[2][i][j],0);
                            //              
                            //              filteredValue = mad(pixVals,filterVals,filteredValue);

                            filteredValue[0] = ImgValues[0, i, j] * localfilter[0, i, j] + filteredValue[0];
                            filteredValue[1] = ImgValues[1, i, j] * localfilter[1, i, j] + filteredValue[1];
                            filteredValue[2] = ImgValues[2, i, j] * localfilter[2, i, j] + filteredValue[2];
                        }
                    }
                    ind = 3 * ((y + CENTER) * w + x + CENTER);
                    FilteredImage[ind] = Math.Min(Math.Max(filteredValue[0], 0), 255);
                    FilteredImage[ind + 1] = Math.Min(Math.Max(filteredValue[1], 0), 255);
                    FilteredImage[ind + 2] = Math.Min(Math.Max(filteredValue[2], 0), 255);

                    //Gets next filter values
                    for (int i = 0; i < FILTERSIZE; i++)
                    {
                        for (int j = 0; j < FILTERSIZE - 1; j++)
                        {
                            ImgValues[0, i, j] = ImgValues[0, i, j + 1];
                            ImgValues[1, i, j] = ImgValues[1, i, j + 1];
                            ImgValues[2, i, j] = ImgValues[2, i, j + 1];
                        }
                        ind = 3 * ((y + i) * w + x + FILTERSIZE - 1);
                        ImgValues[0, i, FILTERSIZE - 1] = ImgData[ind];
                        ImgValues[1, i, FILTERSIZE - 1] = ImgData[ind + 1];
                        ImgValues[2, i, FILTERSIZE - 1] = ImgData[ind + 2];
                    }

                }
            }
        }

        /// <summary>Filter source</summary>
        private class CLFilterSrc
        {
            public string src = @"

#define FILTERSIZE 7
#define CENTER 3

__kernel void ApplyFilter(__global uchar * ImgData,
                          __global float * Filter,
                          __global float * FilteredImage,
                          __global int   * ImgWidth)

{
   int y = get_global_id(0);
   int w = ImgWidth[0];
   
   //Image values
   float ImgValues[3][FILTERSIZE][FILTERSIZE];

   //Copies filter to local memory
   float localfilter[3][FILTERSIZE][FILTERSIZE];
   
   //Initialization of ImgValues
   for (int i = 0; i < FILTERSIZE; i++)
   {
       for (int j = 0; j < FILTERSIZE; j++)
       {
           ImgValues[0][i][j] = ImgData[3*((y+i)*w+j)];
           ImgValues[1][i][j] = ImgData[3*((y+i)*w+j)+1];
           ImgValues[2][i][j] = ImgData[3*((y+i)*w+j)+2];

           localfilter[0][i][j] = Filter[3*(i*FILTERSIZE + j)];
           localfilter[1][i][j] = Filter[3*(i*FILTERSIZE + j)+1];
           localfilter[2][i][j] = Filter[3*(i*FILTERSIZE + j)+2];
       }
   }
   
   
   int xMax = w-CENTER;
   int ind = 0;
   for (int x = 0; x < xMax; x++)
   {
       //Calculates filtered value
       float4 filteredValue = 0;
       for (int i = 0; i < FILTERSIZE; i++)
       {
          for (int j = 0; j < FILTERSIZE-1; j++)
          {
              float4 pixVals = (float4)(ImgValues[0][i][j],ImgValues[1][i][j],ImgValues[2][i][j],0);
              float4 filterVals = (float4)(localfilter[0][i][j],localfilter[1][i][j],localfilter[2][i][j],0);
              
              filteredValue = pixVals*filterVals+filteredValue;

//              filteredValue.x = mad(ImgValues[0][i][j], localfilter[0][i][j], filteredValue.x);
//              filteredValue.y = mad(ImgValues[1][i][j], localfilter[1][i][j], filteredValue.y);
//              filteredValue.z = mad(ImgValues[2][i][j], localfilter[2][i][j], filteredValue.z);
          }
       }
       ind = 3*((y+CENTER)*w+x+CENTER);       
       FilteredImage[ind] = clamp(filteredValue.x,0,255);
       FilteredImage[ind+1] = clamp(filteredValue.y,0,255);
       FilteredImage[ind+2] = clamp(filteredValue.z,0,255);
   
       //Gets next filter values
       for (int i = 0; i < FILTERSIZE; i++)
       {
          for (int j = 0; j < FILTERSIZE-1; j++)
          {
             ImgValues[0][i][j] = ImgValues[0][i][j+1];
             ImgValues[1][i][j] = ImgValues[1][i][j+1];
             ImgValues[2][i][j] = ImgValues[2][i][j+1];
          }
          ind = 3*((y+i)*w+x+FILTERSIZE-1);
          ImgValues[0][i][FILTERSIZE-1] = ImgData[ind];
          ImgValues[1][i][FILTERSIZE-1] = ImgData[ind+1];
          ImgValues[2][i][FILTERSIZE-1] = ImgData[ind+2];
       }
       
   }    
}

kernel void ImgFilter(global uchar * image,
                      global float * Filter,
                      global float * FilteredImage,
                      global int * Width)
                      
{
   int x = get_global_id(0);
   int y = get_global_id(1);
   int w = Width[0];
   int ind = 0;   
   int ind2 = 0;   

   float4 filteredVal = (float4)(0,0,0,0);
   for (int i = 0; i < FILTERSIZE; i++)
   {
       for (int j = 0; j < FILTERSIZE; j++)
       {
           ind = 3*(x+j + w*(y+i));
           ind2 = 3*(i*FILTERSIZE + j);
           filteredVal.x =  mad(Filter[ind2] ,   (float)image[ind],  filteredVal.x);
           filteredVal.y =  mad(Filter[ind2+1] , (float)image[ind+1],filteredVal.y);
           filteredVal.z =  mad(Filter[ind2+2] , (float)image[ind+2],filteredVal.z);
       }
   }
   ind = 3*(x+CENTER + w*(y+CENTER));
   FilteredImage[ind] = clamp(filteredVal.x,0,255);
   FilteredImage[ind+1] = clamp(filteredVal.y,0,255);
   FilteredImage[ind+2] = clamp(filteredVal.z,0,255);
}
";
        }
    }
}
