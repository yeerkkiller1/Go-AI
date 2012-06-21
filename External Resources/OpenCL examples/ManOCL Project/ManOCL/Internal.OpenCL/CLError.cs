namespace ManOCL.Internal.OpenCL
{
    using System;

    internal enum CLError
    {
        BuildProgramFailure = -11,
        DeviceCompilerNotAvailable = -3,
        DeviceNotAvailable = -2,
        DeviceNotFound = -1,
        ImageFormatMismatch = -9,
        ImageFormatNotSupported = -10,
        InvalidArgIndex = -49,
        InvalidArgSize = -51,
        InvalidArgValue = -50,
        InvalidBinary = -42,
        InvalidBufferSize = -61,
        InvalidBuildOptions = -43,
        InvalidCommandQueue = -36,
        InvalidContext = -34,
        InvalidDevice = -33,
        InvalidDeviceType = -31,
        InvalidEvent = -58,
        InvalidEventWaitList = -57,
        InvalidGlobalOffset = -56,
        InvalidGlobalWorkSize = -63,
        InvalidGLObject = -60,
        InvalidHostPtr = -37,
        InvalidImageFormatDescriptor = -39,
        InvalidImageSize = -40,
        InvalidKernel = -48,
        InvalidKernelArgs = -52,
        InvalidKernelDefinition = -47,
        InvalidKernelName = -46,
        InvalidMemObject = -38,
        InvalidMipLevel = -62,
        InvalidOperation = -59,
        InvalidPlatform = -32,
        InvalidProgram = -44,
        InvalidProgramExecutable = -45,
        InvalidQueueProperties = -35,
        InvalidSampler = -41,
        InvalidValue = -30,
        InvalidWorkDimension = -53,
        InvalidWorkGroupSize = -54,
        InvalidWorkItemSize = -55,
        MapFailure = -12,
        MemCopyOverlap = -8,
        MemObjectAllocationFailure = -4,
        OutOfHostMemory = -6,
        OutOfResources = -5,
        ProfilingInfoNotAvailable = -7,
        None = 0
    }
}

