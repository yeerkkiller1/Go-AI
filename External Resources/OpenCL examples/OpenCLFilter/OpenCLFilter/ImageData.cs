using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.Text;
using OpenCLTemplate;

namespace OpenCLFilter
{
    /// <summary>Stores image data in OpenCL memory</summary>
    public class ImageData
    {
        /// <summary>Pixel size</summary>
        private const int PIXELSIZE = 3;

        /// <summary>OpenCL byte array that stores the image data</summary>
        public CLCalc.Program.Variable varData;
        /// <summary>Host memory image data</summary>
        public byte[] Data;

        private int width, height;

        /// <summary>Gets image width</summary>
        public int Width
        {
            get { return width; }
        }

        /// <summary>Gets image height</summary>
        public int Height
        {
            get { return height; }
        }

        /// <summary>ImageData constructor. Reads data from a bitmap</summary>
        /// <param name="bmp">Bitmap to read from</param>
        public ImageData(Bitmap bmp)
        {
            if (CLCalc.CLAcceleration == CLCalc.CLAccelerationType.Unknown) CLCalc.InitCL();

            width = bmp.Width;
            height = bmp.Height;

            //Allocates space for data
            Data = new byte[3 * width * height];

            //Reads bmp to local Data variable
            ReadToLocalData(bmp);

            //Transfer data to OpenCL device
            varData = new CLCalc.Program.Variable(Data);
        }

        /// <summary>Reads data from a bitmap.</summary>
        /// <param name="bmp">Source bitmap</param>
        public void ReadBmp(Bitmap bmp)
        {
            ReadToLocalData(bmp);
            varData.WriteToDevice(Data);
        }

        /// <summary>Returns stored data in a bitmap</summary>
        /// <param name="bmp">Reference bitmap</param>
        public Bitmap GetStoredBitmap(Bitmap bmp)
        {
            Bitmap resp = new Bitmap(width, height, bmp.PixelFormat);

            BitmapData bmd = resp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height),
             System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);

            //Write data
            unsafe
            {
                for (int y = 0; y < bmd.Height; y++)
                {
                    byte* row = (byte*)bmd.Scan0 + (y * bmd.Stride);

                    for (int x = 0; x < bmd.Width; x++)
                    {
                        row[x * PIXELSIZE] = Data[3 * (x + width * y)];
                        row[x * PIXELSIZE + 1] = Data[3 * (x + width * y) + 1];
                        row[x * PIXELSIZE + 2] = Data[3 * (x + width * y) + 2];
                    }
                }
            }

            //Unlock bits
            resp.UnlockBits(bmd);

            return resp;
        }

        /// <summary>Copies bitmap data to local Data</summary>
        /// <param name="bmp">Bitmap to copy</param>
        private void ReadToLocalData(Bitmap bmp)
        {
            //Lock bits
            BitmapData bmd = bmp.LockBits(new Rectangle(0, 0, bmp.Width, bmp.Height),
                             System.Drawing.Imaging.ImageLockMode.ReadOnly, bmp.PixelFormat);
            //Read data
            unsafe
            {
                for (int y = 0; y < bmd.Height; y++)
                {
                    byte* row = (byte*)bmd.Scan0 + (y * bmd.Stride);

                    for (int x = 0; x < bmd.Width; x++)
                    {
                        Data[3 * (x + width * y)] = row[x * PIXELSIZE];
                        Data[3 * (x + width * y) + 1] = row[x * PIXELSIZE + 1];
                        Data[3 * (x + width * y) + 2] = row[x * PIXELSIZE + 2];
                    }
                }
            }

            //Unlock bits
            bmp.UnlockBits(bmd);
        }
    }
}
