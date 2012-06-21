using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using ManOCL;
using ManOCL.IO;
using System.IO;

namespace QuickManOCLTest
{
    class Program
    {
        static void Main(string[] args)
        {
            byte[] tempData = new byte[1000];
            Random random = new Random();
            random.NextBytes(tempData);

            byte[] output = new byte[1000];

            DeviceGlobalMemory testIn = tempData;
            DeviceGlobalMemory testOut = output;

            Kernel kernel = Kernel.Create("run",
                File.ReadAllText(@"C:\Users\QuentinBrooks\Dropbox\Go AI\New C++ AI Build\C Sharp OpenCL Compile and Runner\Test.cl"),
                testIn, testOut);

            Event e = kernel.Execute(1000);

            kernel.CommandQueue.Finish();

            UnmanagedReader reader = new UnmanagedReader(new DeviceBufferStream(testOut));


        }
    }
}
