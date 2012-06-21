﻿#region License

/*

Copyright (c) 2009 - 2010 Fatjon Sakiqi

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.

*/

#endregion

using System;
using System.IO;
using Cloo;

namespace Clootils
{
    class ProgramTest : TestBase
    {
        static TextWriter log;
        private static string clSource = @"kernel void Test(void) { }";

        public static void Run(TextWriter log, ComputeContext context)
        {
            StartTest(log, "Program test");
            
            ProgramTest.log = log;
            
            try
            {
                ComputeProgram program = new ComputeProgram(context, clSource);
                program.Build(null, null, notify, IntPtr.Zero);
                byte[] bytes = program.Binaries[0];
                log.WriteLine("Compiled program head:");
                log.WriteLine(BitConverter.ToString(bytes, 0, 16) + "...");
            }
            catch (Exception e)
            {
                log.WriteLine(e.ToString());
            }

            EndTest(log, "Program test");
        }

        private static void notify(IntPtr programHandle, IntPtr userDataPtr)
        {
            log.WriteLine("Program build notification.");
        }
    }
}