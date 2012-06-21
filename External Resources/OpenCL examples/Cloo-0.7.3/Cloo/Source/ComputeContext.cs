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
    using System.Runtime.InteropServices;
    using Cloo.Bindings;

    /// <summary>
    /// Represents an OpenCL context.
    /// </summary>
    /// <remarks> The environment within which the kernels execute and the domain in which synchronization and memory management is defined. </remarks>
    /// <br/>
    /// <example> 
    /// This example shows how to create a <c>ComputeContext</c> that is able to share data with an OpenGL context in a Microsoft Windows OS:
    /// <code>
    /// [DllImport("opengl32.dll")]
    /// extern static IntPtr wglGetCurrentDC();
    /// // ...
    /// IntPtr deviceContextHandle = wglGetCurrentDC();
    /// ComputePlatform platform = ComputePlatform.GetByName(nameOfPlatformCapableOfCLGLInterop);
    /// ComputeContextProperty p1 = new ComputeContextProperty(ComputeContextPropertyName.Platform, platform.Handle);
    /// ComputeContextProperty p2 = new ComputeContextProperty(ComputeContextPropertyName.CL_GL_CONTEXT_KHR, openGLContextHandle);
    /// ComputeContextProperty p3 = new ComputeContextProperty(ComputeContextPropertyName.CL_WGL_HDC_KHR, deviceContextHandle);
    /// ComputeContextPropertyList cpl = new ComputeContextPropertyList(new ComputeContextProperty[] { p1, p2, p3 });
    /// ComputeContext context = new ComputeContext(ComputeDeviceTypes.Gpu, cpl, null, IntPtr.Zero);
    /// </code>
    /// </example>
    /// <seealso cref="ComputeDevice"/>
    /// <seealso cref="ComputePlatform"/>
    public class ComputeContext : ComputeResource
    {
        #region Fields

        private readonly ReadOnlyCollection<ComputeDevice> devices;
        private readonly ComputePlatform platform;
        private readonly ComputeContextPropertyList properties;

        #endregion

        #region Properties

        /// <summary>
        /// Gets a read-only collection of the <c>ComputeDevice</c>s of the <c>ComputeContext</c>.
        /// </summary>
        public ReadOnlyCollection<ComputeDevice> Devices { get { return devices; } }

        /// <summary>
        /// Gets the <c>ComputePlatform</c> of the <c>ComputeContext</c>.
        /// </summary>
        public ComputePlatform Platform { get { return platform; } }

        /// <summary>
        /// Gets a collection of <c>ComputeContextProperty</c>s of the <c>ComputeContext</c>.
        /// </summary>
        public ComputeContextPropertyList Properties { get { return properties; } }

        #endregion

        #region Constructors

        /// <summary>
        /// Creates a new <c>ComputeContext</c> on a collection of <c>ComputeDevice</c>s.
        /// </summary>
        /// <param name="devices"> A collection of <c>ComputeDevice</c>s to associate with the <c>ComputeContext</c>. </param>
        /// <param name="properties"> A <c>ComputeContextPropertyList</c> of the <c>ComputeContext</c>. </param>
        /// <param name="notify"> A callback function that can be registered by the application. This callback function will be used by the OpenCL implementation to report information on errors that occur in the <c>ComputeContext</c>. This callback function may be called asynchronously by the OpenCL implementation. It is the application's responsibility to ensure that the callback function is thread-safe. If <paramref name="notify"/> is <c>null</c>, no callback function is registered. </param>
        /// <param name="notifyDataPtr"> Optional user data that will be passed to <paramref name="notify"/>. </param>
        public ComputeContext(ICollection<ComputeDevice> devices, ComputeContextPropertyList properties, ComputeContextNotifier notify, IntPtr notifyDataPtr)
        {
            unsafe
            {
                IntPtr[] deviceHandles = Tools.ExtractHandles(devices);
                IntPtr[] propertiesList = (properties != null) ? properties.ToIntPtrArray() : null;
                IntPtr notifyFuncPtr = (notify != null) ? Marshal.GetFunctionPointerForDelegate(notify) : IntPtr.Zero;

                ComputeErrorCode error = ComputeErrorCode.Success;
                fixed (IntPtr* propertiesPtr = propertiesList)
                fixed (IntPtr* deviceHandlesPtr = deviceHandles)
                    Handle = CL10.CreateContext(
                        propertiesPtr,
                        devices.Count,
                        deviceHandlesPtr,
                        notifyFuncPtr,
                        notifyDataPtr,
                        &error);
                ComputeException.ThrowOnError(error);

                this.properties = properties;
                ComputeContextProperty platformProperty = properties.GetByName(ComputeContextPropertyName.Platform);
                this.platform = ComputePlatform.GetByHandle(platformProperty.Value);
                this.devices = GetDevices();
            }
        }

        /// <summary>
        /// Creates a new <c>ComputeContext</c> on all the <c>ComputeDevice</c>s that match the specified <c>ComputeDeviceTypes</c>.
        /// </summary>
        /// <param name="deviceType"> A bit-field that identifies the type of <c>ComputeDevice</c> to associate with the <c>ComputeContext</c>. </param>
        /// <param name="properties"> A <c>ComputeContextPropertyList</c> of the <c>ComputeContext</c>. </param>
        /// <param name="notify"> A callback function that can be registered by the application. This callback function will be used by the OpenCL implementation to report information on errors that occur in the <c>ComputeContext</c>. This callback function may be called asynchronously by the OpenCL implementation. It is the application's responsibility to ensure that the callback function is thread-safe. If <paramref name="notify"/> is <c>null</c>, no callback function is registered. </param>
        /// <param name="userDataPtr"> Optional user data that will be passed to <paramref name="notify"/>. </param>
        public ComputeContext(ComputeDeviceTypes deviceType, ComputeContextPropertyList properties, ComputeContextNotifier notify, IntPtr userDataPtr)
        {
            unsafe
            {
                IntPtr[] propertiesList = (properties != null) ? properties.ToIntPtrArray() : null;
                IntPtr notifyFuncPtr = (notify != null) ? Marshal.GetFunctionPointerForDelegate(notify) : IntPtr.Zero;

                ComputeErrorCode error = ComputeErrorCode.Success;
                fixed (IntPtr* propertiesPtr = propertiesList)
                    Handle = CL10.CreateContextFromType(
                        propertiesPtr,
                        deviceType,
                        notifyFuncPtr,
                        userDataPtr,
                        &error);
                ComputeException.ThrowOnError(error);

                this.properties = properties;
                ComputeContextProperty platformProperty = properties.GetByName(ComputeContextPropertyName.Platform);
                this.platform = ComputePlatform.GetByHandle(platformProperty.Value);
                this.devices = GetDevices();
            }
        }

        #endregion

        #region Public methods

        /// <summary>
        /// Gets the string representation of the <c>ComputeContext</c>.
        /// </summary>
        /// <returns> The string representation of the <c>ComputeContext</c>. </returns>
        public override string ToString()
        {
            return "ComputeContext" + base.ToString();
        }

        #endregion

        #region Protected methods

        protected override void Dispose(bool manual)
        {
            if (manual)
            {
                //free managed resources
            }

            // free native resources
            if (Handle != IntPtr.Zero)
            {
                CL10.ReleaseContext(Handle);
                Handle = IntPtr.Zero;
            }
        }

        #endregion

        #region Private methods

        private ReadOnlyCollection<ComputeDevice> GetDevices()
        {
            unsafe
            {
                List<IntPtr> validDeviceHandles = new List<IntPtr>(GetArrayInfo<ComputeContextInfo, IntPtr>(ComputeContextInfo.Devices, CL10.GetContextInfo));
                List<ComputeDevice> validDevices = new List<ComputeDevice>();
                foreach (ComputePlatform platform in ComputePlatform.Platforms)
                {
                    foreach (ComputeDevice device in platform.Devices)
                        if (validDeviceHandles.Contains(device.Handle))
                            validDevices.Add(device);
                }
                return new ReadOnlyCollection<ComputeDevice>(validDevices);
            }
        }

        #endregion
    }

    /// <summary>
    /// A callback function that can be registered by the application to report information on errors that occur in the <c>ComputeContext</c>.
    /// </summary>
    /// <param name="errorInfo"> An error string. </param>
    /// <param name="clDataPtr"> A pointer to binary data that is returned by the OpenCL implementation that can be used to log additional information helpful in debugging the error.</param>
    /// <param name="clDataSize"> The size of the binary data that is returned by the OpenCL. </param>
    /// <param name="userDataPtr"> The pointer to the optional user data specified in <paramref name="notifyDataPtr"/> argument of <c>ComputeContext</c> constructor. </param>
    /// <remarks> This callback function may be called asynchronously by the OpenCL implementation. It is the application's responsibility to ensure that the callback function is thread-safe. </remarks>
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void ComputeContextNotifier(String errorInfo, IntPtr clDataPtr, IntPtr clDataSize, IntPtr userDataPtr);
}