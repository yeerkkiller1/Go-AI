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
    /// Represents an OpenCL 3D image.
    /// </summary>
    /// <seealso cref="ComputeImage"/>
    public class ComputeImage3D : ComputeImage
    {
        #region Constructors

        /// <summary>
        /// Creates a new <c>ComputeImage3D</c>.
        /// </summary>
        /// <param name="context"> A valid <c>ComputeContext</c> in which the <c>ComputeImage3D</c> is created. </param>
        /// <param name="flags"> A bit-field that is used to specify allocation and usage information about the <c>ComputeImage3D</c>. </param>
        /// <param name="format"> A structure that describes the format properties of the <c>ComputeImage3D</c>. </param>
        /// <param name="width"> The width of the <c>ComputeImage3D</c> in pixels. </param>
        /// <param name="height"> The height of the <c>ComputeImage3D</c> in pixels. </param>
        /// <param name="depth"> The depth of the <c>ComputeImage3D</c> in pixels. </param>
        /// <param name="rowPitch"> The size in bytes of each row of elements of the <c>ComputeImage3D</c>. If <paramref name="rowPitch"/> is zero, OpenCL will compute the proper value based on <c>ComputeImage.Width</c> and <c>ComputeImage.ElementSize</c>. </param>
        /// <param name="slicePitch"> The size in bytes of each 2D slice in the <c>ComputeImage3D</c>. If <paramref name="slicePitch"/> is zero, OpenCL will compute the proper value based on <c>ComputeImage.RowPitch</c> and <c>ComputeImage.Height</c>. </param>
        /// <param name="data"> The data to initialize the <c>ComputeImage3D</c>. Can be <c>IntPtr.Zero</c>. </param>
        public ComputeImage3D(ComputeContext context, ComputeMemoryFlags flags, ComputeImageFormat format, int width, int height, int depth, long rowPitch, long slicePitch, IntPtr data)
            : base(context, flags)
        {
            unsafe
            {
                ComputeErrorCode error = ComputeErrorCode.Success;
                Handle = CL10.CreateImage3D(
                    context.Handle,
                    flags,
                    &format,
                    new IntPtr(width),
                    new IntPtr(height),
                    new IntPtr(depth),
                    new IntPtr(rowPitch),
                    new IntPtr(slicePitch),
                    data,
                    &error);
                ComputeException.ThrowOnError(error);

                Init();
            }
        }

        private ComputeImage3D(IntPtr handle, ComputeContext context, ComputeMemoryFlags flags)
            : base(context, flags)
        {
            Handle = handle;

            Init();
        }

        #endregion

        #region Public methods

        /// <summary>
        /// Creates a new <c>ComputeImage3D</c> from an OpenGL 3D texture object.
        /// </summary>
        /// <param name="context"> A <c>ComputeContext</c> with enabled CL/GL sharing. </param>
        /// <param name="flags"> A bit-field that is used to specify usage information about the <c>ComputeImage3D</c>. Only <c>ComputeMemoryFlags.ReadOnly</c>, <c>ComputeMemoryFlags.WriteOnly</c> and <c>ComputeMemoryFlags.ReadWrite</c> are allowed. </param>
        /// <param name="textureTarget"> The image type of texture. Must be GL_TEXTURE_3D. </param>
        /// <param name="mipLevel"> The mipmap level of the OpenGL 2D texture object to be used. </param>
        /// <param name="textureId"> The OpenGL 2D texture object id to use. </param>
        /// <returns> The created <c>ComputeImage2D</c>. </returns>
        public static ComputeImage3D CreateFromGLTexture3D(ComputeContext context, ComputeMemoryFlags flags, int textureTarget, int mipLevel, int textureId)
        {
            IntPtr image = IntPtr.Zero;
            unsafe
            {
                ComputeErrorCode error = ComputeErrorCode.Success;
                image = CL10.CreateFromGLTexture3D(
                    context.Handle,
                    flags,
                    textureTarget,
                    mipLevel,
                    textureId,
                    &error);
                ComputeException.ThrowOnError(error);
            }
            return new ComputeImage3D(image, context, flags);
        }

        /// <summary>
        /// Gets a collection of supported <c>ComputeImage3D</c> <c>ComputeImageFormat</c>s in a <c>ComputeContext</c>.
        /// </summary>
        /// <param name="context"> The <c>ComputeContext</c> for which the collection of <c>ComputeImageFormat</c>s is queried. </param>
        /// <param name="flags"> The <c>ComputeMemoryFlags</c> for which the collection of <c>ComputeImageFormat</c>s is queried. </param>
        /// <returns> The collection of the required <c>ComputeImageFormat</c>s. </returns>
        public static ICollection<ComputeImageFormat> GetSupportedFormats(ComputeContext context, ComputeMemoryFlags flags)
        {
            return GetSupportedFormats(context, flags, ComputeMemoryType.Image3D);
        }

        #endregion
    }
}