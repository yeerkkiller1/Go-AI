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

namespace Cloo
{
    using System;
    using System.Collections.Generic;
    using System.Collections.ObjectModel;
    using Cloo.Bindings;

    /// <summary>
    /// Represents an OpenCL image.
    /// </summary>
    /// <remarks> A memory object that stores a two- or three- dimensional structured array. Image data can only be accessed with read and write functions. The read functions use a sampler. </remarks>
    /// <seealso cref="ComputeMemory"/>
    /// <seealso cref="ComputeSampler"/>
    public abstract class ComputeImage : ComputeMemory
    {
        #region Properties

        /// <summary>
        /// Gets or sets (protected) the depth in pixels of the <c>ComputeImage</c>.
        /// </summary>
        public int Depth { get; protected set; }

        /// <summary>
        /// Gets or sets (protected) the size of the elements (pixels) of the <c>ComputeImage</c>.
        /// </summary>
        public int ElementSize { get; protected set; }

        /// <summary>
        /// Gets or sets (protected) the height in pixels of the <c>ComputeImage</c>.
        /// </summary>
        public int Height { get; protected set; }

        /// <summary>
        /// Gets or sets (protected) the size in bytes of a row of elements of the <c>ComputeImage</c>.
        /// </summary>
        public long RowPitch { get; protected set; }

        /// <summary>
        /// Gets or sets (protected) the size in bytes of a 2D slice of a <c>ComputeImage3D</c>. For a <c>ComputeImage2D</c> this value is 0.
        /// </summary>
        public long SlicePitch { get; protected set; }

        /// <summary>
        /// Gets or sets (protected) the width in pixels of the <c>ComputeImage</c>.
        /// </summary>
        public int Width { get; protected set; }

        #endregion

        #region Constructors

        protected ComputeImage(ComputeContext context, ComputeMemoryFlags flags)
            : base(context, flags)
        { }

        #endregion

        #region Public methods

        /// <summary>
        /// Gets the string representation of the <c>ComputeImage</c>.
        /// </summary>
        /// <returns> The string representation of the <c>ComputeImage</c>. </returns>
        public override string ToString()
        {
            return "ComputeImage" + base.ToString();
        }

        #endregion

        #region Protected methods

        protected static ICollection<ComputeImageFormat> GetSupportedFormats(ComputeContext context, ComputeMemoryFlags flags, ComputeMemoryType type)
        {
            unsafe
            {
                int formatCountRet = 0;
                ComputeErrorCode error = CL10.GetSupportedImageFormats(context.Handle, flags, type, 0, null, &formatCountRet);
                ComputeException.ThrowOnError(error);

                ComputeImageFormat[] formats = new ComputeImageFormat[formatCountRet];
                fixed (ComputeImageFormat* formatsPtr = formats)
                {
                    error = CL10.GetSupportedImageFormats(context.Handle, flags, type, formatCountRet, formatsPtr, null);
                    ComputeException.ThrowOnError(error);
                }

                return new Collection<ComputeImageFormat>(formats);
            }
        }

        protected void Init()
        {
            unsafe
            {
                Depth = (int)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.Depth, CL10.GetImageInfo);
                ElementSize = (int)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.ElementSize, CL10.GetImageInfo);
                Height = (int)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.Height, CL10.GetImageInfo);
                RowPitch = (long)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.RowPitch, CL10.GetImageInfo);
                Size = (long)GetInfo<ComputeMemoryInfo, IntPtr>(ComputeMemoryInfo.Size, CL10.GetMemObjectInfo);
                SlicePitch = (long)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.SlicePitch, CL10.GetImageInfo);
                Width = (int)GetInfo<ComputeImageInfo, IntPtr>(ComputeImageInfo.Width, CL10.GetImageInfo);
            }
        }

        #endregion
    }
}