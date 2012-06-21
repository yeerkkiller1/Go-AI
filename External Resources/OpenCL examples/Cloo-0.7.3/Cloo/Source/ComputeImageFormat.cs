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
    using System.Runtime.InteropServices;

    /// <summary>
    /// Represents an OpenCL image format.
    /// </summary>
    /// <remarks> This structure defines the type, count and size of the image channels. </remarks>
    /// <seealso cref="ComputeImage"/>
    [StructLayout(LayoutKind.Sequential)]
    public struct ComputeImageFormat
    {
        #region Fields

        private readonly ComputeImageChannelOrder channelOrder;
        private readonly ComputeImageChannelType channelType;

        #endregion

        #region Properties

        /// <summary>
        /// Gets the <c>ComputeImageChannleOrder</c> of the <c>ComputeImage</c>.
        /// </summary>
        public ComputeImageChannelOrder ChannelOrder { get { return channelOrder; } }

        /// <summary>
        /// Gets the <c>ComputeImageChannelType</c> of the <c>ComputeImage</c>.
        /// </summary>
        public ComputeImageChannelType ChannelType { get { return channelType; } }

        #endregion

        #region Constructors

        /// <summary>
        /// Creates a new <c>ComputeImageFormat</c>.
        /// </summary>
        /// <param name="channelOrder"> The number of channels and the channel layout i.e. the memory layout in which channels are stored in the <c>ComputeImage</c>. </param>
        /// <param name="channelType"> The type of the channel data. The number of bits per element determined by the <paramref name="channelType"/> and <paramref name="channelOrder"/> must be a power of two. </param>
        public ComputeImageFormat(ComputeImageChannelOrder channelOrder, ComputeImageChannelType channelType)
        {
            this.channelOrder = channelOrder;
            this.channelType = channelType;
        }

        #endregion
    }
}