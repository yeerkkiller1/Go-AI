using System;
using System.Collections.Generic;
using System.Text;
using OpenCLTemplate;

namespace OpenCLMatrixMult
{
    public class CLMatrixMult
    {
        /// <summary>Matrix multiplication kernel</summary>
        CLCalc.Program.Kernel floatMatrixMultNoLocals;
        /// <summary>Matrix multiplication kernel using locals</summary>
        CLCalc.Program.Kernel floatMatrixMultLocals;

        /// <summary>Constructor. Initializes OpenCL</summary>
        public CLMatrixMult()
        {
            if (CLCalc.GLAcceleration == CLCalc.GLAccelerationType.Unknown)
                CLCalc.InitCL();

            //Using OpenCL, declare variables
            if (CLCalc.GLAcceleration == CLCalc.GLAccelerationType.UsingGL)
            {
                floatMatrixMultSource src = new floatMatrixMultSource();
                CLCalc.Program.Compile(new string[] { src.matrixMultLocals, src.matrixMultNoLocals });

                //multiplication
                floatMatrixMultNoLocals = new CLCalc.Program.Kernel("floatMatrixMult");
                floatMatrixMultLocals = new CLCalc.Program.Kernel("floatMatrixMultLocals");
            }
        }

        #region Matrix copy to and from vector (float[,] float[,])

        /// <summary>Converts vector to matrix</summary>
        /// <param name="v">Vector</param>
        /// <param name="maxi">Matrix first dimension</param>
        /// <param name="maxj">Matrix second dimension</param>
        private float[,] VectorToMatrix(float[] v, ref int maxi, ref int maxj)
        {
            float[,] M = new float[maxi, maxj];

            for (int i = 0; i < maxi; i++)
                for (int j = 0; j < maxj; j++)
                    M[i, j] = v[i + maxi * j];

            return M;
        }

        /// <summary>Converts matrix to vector</summary>
        /// <param name="M">Matrix</param>
        /// <param name="maxi">Matrix first dimension</param>
        /// <param name="maxj">Matrix second dimension</param>
        private float[] MatrixToVector(float[,] M, ref int maxi, ref int maxj)
        {
            float[] v = new float[maxi * maxj];

            for (int i = 0; i < maxi; i++)
                for (int j = 0; j < maxj; j++)
                    v[i + maxi * j] = M[i, j];

            return v;
        }

        #endregion

        /// <summary>Returns the matrix product M1*M2</summary>
        /// <param name="M1">First matrix</param>
        /// <param name="M2">Second matrix</param>
        public float[,] MultiplyLocals(float[,] M1, float[,] M2)
        {
            //M pxq, N qxr
            int p = M1.GetLength(0);
            int q = M1.GetLength(1);
            int r = M2.GetLength(1);

            if (q != M2.GetLength(0)) throw new Exception("Matrix dimensions do not match for multiplication");

            float[] vecM1 = MatrixToVector(M1, ref p, ref q);
            float[] vecM2 = MatrixToVector(M2, ref q, ref r);
            float[] vecResp = new float[p * r];

            CLCalc.Program.Variable varResp = new CLCalc.Program.Variable(vecResp);

            CLCalc.Program.Variable varM1 = new CLCalc.Program.Variable(vecM1);
            CLCalc.Program.Variable varM2 = new CLCalc.Program.Variable(vecM2);


            //Finaliza a soma dos elementos
            int[] vecQ = new int[1] { q };
            CLCalc.Program.Variable varQ = new CLCalc.Program.Variable(vecQ);
            CLCalc.Program.Variable[] args = new CLCalc.Program.Variable[4] { varResp, varM1, varM2, varQ };
            int[] max = new int[2] { p, r };

            floatMatrixMultLocals.Execute(args, max, new int[] { 8, 8 });

            varResp.ReadFromDeviceTo(vecResp);

            varResp.Dispose();


            return VectorToMatrix(vecResp, ref p, ref r);
        }

        /// <summary>Returns the matrix product M1*M2</summary>
        /// <param name="M1">First matrix</param>
        /// <param name="M2">Second matrix</param>
        public float[,] MultiplyNoLocals(float[,] M1, float[,] M2)
        {
            //M pxq, N qxr
            int p = M1.GetLength(0);
            int q = M1.GetLength(1);
            int r = M2.GetLength(1);

            if (q != M2.GetLength(0)) throw new Exception("Matrix dimensions do not match for multiplication");

            float[] vecM1 = MatrixToVector(M1, ref p, ref q);
            float[] vecM2 = MatrixToVector(M2, ref q, ref r);
            float[] vecResp = new float[p * r];

            CLCalc.Program.Variable varResp = new CLCalc.Program.Variable(vecResp);

            CLCalc.Program.Variable varM1 = new CLCalc.Program.Variable(vecM1);
            CLCalc.Program.Variable varM2 = new CLCalc.Program.Variable(vecM2);


            //Finaliza a soma dos elementos
            int[] vecQ = new int[1] { q };
            CLCalc.Program.Variable varQ = new CLCalc.Program.Variable(vecQ);
            CLCalc.Program.Variable[] args = new CLCalc.Program.Variable[4] { varResp, varM1, varM2, varQ };
            int[] max = new int[2] { p, r };

            floatMatrixMultNoLocals.Execute(args, max);

            varResp.ReadFromDeviceTo(vecResp);

            varResp.Dispose();


            return VectorToMatrix(vecResp, ref p, ref r);
        }

        /// <summary>Returns the matrix product M1*M2 without using OpenCL</summary>
        /// <param name="M1">First matrix</param>
        /// <param name="M2">Second matrix</param>
        public double[,] MultiplyNoOpenCL(float[,] M1, float[,] M2)
        {
            //M pxq, N qxr
            int p = M1.GetLength(0);
            int q = M1.GetLength(1);
            int r = M2.GetLength(1);

            if (q != M2.GetLength(0)) throw new Exception("Matrix dimensions do not match for multiplication");

            double[,] resp = new double[p, r];

            for (int i = 0; i < p; i++)
            {
                for (int j = 0; j < r; j++)
                {
                    for (int k = 0; k < q; k++)
                    {
                        resp[i, j] += M1[i, k] * M2[k, j];
                    }
                }
            }
            return resp;
        }

        #region OpenCL Source
        private class floatMatrixMultSource
        {
            #region OpenCL source for matrix multiplication
            /// <summary>Matrix multiplication. Dimensions { p, r }.
            /// </summary>
            public string matrixMultNoLocals = @"
__kernel void
floatMatrixMult(     __global       float * MResp,
                     __global       float * M1,
                     __global       float * M2,
                     __global       int * q)
{
    // Vector element index
    int i = get_global_id(0);
    int j = get_global_id(1);

    int p = get_global_size(0);
    int r = get_global_size(1);

    MResp[i + p * j] = 0;
    int QQ = q[0];
    for (int k = 0; k < QQ; k++)
    {
        MResp[i + p * j] += M1[i + p * k] * M2[k + QQ * j];
    }
}
";

            public string matrixMultLocals = @"
#define BLOCK_SIZE 8

__kernel __attribute__((reqd_work_group_size(BLOCK_SIZE, BLOCK_SIZE, 1))) void
floatMatrixMultLocals(__global       float * MResp,
                      __global       float * M1,
                      __global       float * M2,
                      __global       int * q)
{
   //Identification of this workgroup
   int i = get_group_id(0);
   int j = get_group_id(1);

   //Identification of work-item
   int idX = get_local_id(0); 
   int idY = get_local_id(1); 

   //matrixes dimensions
   int p = get_global_size(0);
   int r = get_global_size(1);
   int qq = q[0];

   //Number of submatrixes to be processed by each worker
   int numSubMat = qq/BLOCK_SIZE;

   float4 resp = (float4)(0,0,0,0);
   __local float A[BLOCK_SIZE][BLOCK_SIZE];
   __local float B[BLOCK_SIZE][BLOCK_SIZE];

   for (int k=0; k<numSubMat; k++)
   {
       //Copy submatrixes to local memory. Each worker copies one element
       //Notice that A[i,k] accesses elements starting from M[BLOCK_SIZE*i, BLOCK_SIZE*j]
       A[idX][idY] = M1[BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*k+idY)];
       B[idX][idY] = M2[BLOCK_SIZE*k + idX + qq*(BLOCK_SIZE*j+idY)];
       barrier(CLK_LOCAL_MEM_FENCE);

       for (int k2 = 0; k2 < BLOCK_SIZE; k2+=4) 
       {
            float4 temp1=(float4)(A[idX][k2],A[idX][k2+1],A[idX][k2+2],A[idX][k2+3]);
            float4 temp2=(float4)(B[k2][idY],B[k2+1][idY],B[k2+2][idY],B[k2+3][idY]);
            resp += temp1 * temp2;
       }
       barrier(CLK_LOCAL_MEM_FENCE);
   }

   MResp[BLOCK_SIZE*i + idX + p*(BLOCK_SIZE*j+idY)] = resp.x+resp.y+resp.z+resp.w;

}
";
            #endregion
        }
        #endregion
    }
}
