// Copyright 2014 raodm@miamiOH.edu

#ifndef CL_CMD_QUEUE_H
#define CL_CMD_QUEUE_H

#include <string>
#include <iostream>
#include <fstream>
#include <iterator>
#include <algorithm>

// Enable OpenCL C++ exceptions
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

/** \brief A convenience class to create and run OpenCL kernels on a
    device.

    The class provides a few convenience methods that wrap the core
    OpenCL API.  The methods essentially provide a streamlined API
    that wraps routine startup code for OpenCL.
*/
class ClCmdQueue {
public:
    /// Convenient Read-Only flags (CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR)
    static const cl_mem_flags ROFlags;
    /// Convenient Read-Write flags (CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR)
    static const cl_mem_flags RWFlags;
    /// Convenient Read-Write flags (CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR)
    static const cl_mem_flags WOFlags;

    /** Constructor that creates 

        \param[in] devType The type of device on which an OpenCL cmd
        queue is to be created. Valid values are CL_DEVICE_TYPE_CPU,
        CL_DEVICE_TYPE_GPU, CL_DEVICE_TYPE_ACCELERATOR.

        \param[in] platform The index on the platform on which the
        queue is to be created. If the platform is -1, then the first
        platform that has the given device type is chosen.

        \exception cl::Error The constructor throws an cl::Error if a
        valid queue could not be created.  If a valid device is not
        found this method throws cl::Error(CL_DEVICE_NOT_FOUND)
    */
    ClCmdQueue(cl_device_type devType = CL_DEVICE_TYPE_GPU,
               int platform = -1) throw(cl::Error);

    /** Create a program from the given source string.

        This method can be used to create a program by building it for
        the device associated with this command queue.  This method
        reports any errors that are reported during the build process
        and rethrows exceptions.

        \param[in] source The program source code as a string.

        \exception cl::Error If build errors occurr, this method
        reports build errors on std::cerr and rethrows the exception.
     */
    cl::Program createProgram(const std::string& source) throw(cl::Error);

    /** Create a program by loading code from a given file.
        
        This method can be used to create a program by loading the
        source code from a given file.  This method calls the
        createProgram method to actually parse and create the program.
                
        \param[in] path The path to the file from where the source
        code is to be loaded.  If the file cannot be opened for
        reading this method throws cl::Error(CL_INVALID_PROGRAM).
        
        \exception cl::Error If build errors occurr, this method
        reports build errors on std::cerr and rethrows the exception.
    */
    cl::Program loadProgram(const std::string& path) throw(cl::Error);

    /** Load program from a given file and create a kernel.

        This is a convenience method that is used to create a kernel
        from a program that is loaded from a given file.

        \param[in] path The path to the file from where the OpenCL
        program is to be loaded.

        \param[in] kernel The name of the kernel in the program.

        \exception cl::Error This method throws errors if the program
        could not be loaded, built, or if the kernel could not be
        created.
    */
    cl::Kernel loadKernel(const std::string& path,
                          const std::string& kernel) throw(cl::Error);

    /** Create an OpenCL buffer for a given memory buffer on the host.

        This is a convenience method to create an OpenCL buffer using
        the context associated with this command queue.

        \param[in,out] hostPtr Pointer to the host buffer backing the
        OpenCL buffer.  This parameter cannot be NULL.

        \param[in] size The size (in number of bytes) associated with
        this buffer.

        \param[in] flags Optional memory flags to be set for the
        OpenCL buffer.

        \exception cl::Error This method exposes any exceptions that
        may occur during buffer creation.
    */
    cl::Buffer makeBuffer(void *hostPtr, size_t size,
                          cl_mem_flags flags = RWFlags)  throw(cl::Error);

    /** Convenience method to create an 2D image buffer backed by
        store on the host.

        This is a convenience method to create an OpenCL Image2D
        buffer using the context and device associated with this
        command queue.  The default parameters are setup to create an
        RGBA (32-bits per pixel) image where pixels are stored in a
        contiguous buffer in row-major fasion.  The pitch is directly
        passed to the constructor of Image2D as the row_pitch
        parameter.  Refer to the documentation on Image2D class for
        details.
        
        \param[in] hostPtr The memory buffer backing the OpenCL
        buffer.  This parameter cannot be NULL. 
        
        \param[in] width The width of the image in pixels.

        \param[in] height The height of the image in pixels.

        \param[in] pitch The pitch (bytes per row of image).
        
        \param[in] format The pixel storage format.  The defualt is
        CL_RGBA.

        \param[in] pixType The type of pixel data to be used in the
        OpenCL kernel.
    */
    cl::Image2D makeImage(void* hostPtr, size_t width, size_t height,
                          cl_mem_flags flags = RWFlags, size_t pitch = 0,
                          cl_channel_order format  = CL_RGBA,
                          cl_channel_type  pixType = CL_UNSIGNED_INT8);

    /** Convenience method to enqueue mapping operation to read/write
        an image from/to the device associated with this command
        queue.

        \param[in,out] image The image whose contents is to be
        read/written.  This image is typically obtained via (an
        earlier) call to ClCmdQueue::makeImage.

        \param[in] flags The flag that indicates if the data is to be
        read or written. Valid values are CL_MAP_READ and
        CL_MAP_WRITE.

        \param[in] blocking Typically mapping operations are performed
        in a blocking manner at the end of one (or more) OpenCL kernel
        operations.  If this flag is true then mapping is performed in
        a blocking manner (ensuring that mapping is complete when this
        method returns).
    */
    void enqueueMapImage(cl::Image2D& image, cl_mem_flags flags = CL_MAP_READ,
                         bool blocking = CL_TRUE);

    /** Convenience method to enqueue mapping operation to read/write
        a buffer from/to the device associated with this command
        queue.

        \param[in,out] buf The buffer whose contents is to be
        read/written.  This buf is typically obtained via (an
        earlier) call to ClCmdQueue::makeBuffer.

        \param[in] size The size of the regions to be mapped (in
        bytes)
        
        \param[in] flags The flag that indicates if the data is to be
        read or written. Valid values are CL_MAP_READ and
        CL_MAP_WRITE.

        \param[in] blocking Typically mapping operations are performed
        in a blocking manner at the end of one (or more) OpenCL kernel
        operations.  If this flag is true then mapping is performed in
        a blocking manner (ensuring that mapping is complete when this
        method returns).
    */
    void enqueueMapBuffer(cl::Buffer& buf, size_t size,
                          cl_mem_flags flags = CL_MAP_READ,
                          bool blocking = CL_TRUE);

    /** Obtain the context associated with this command queue

        \return The context associated with this command queue.
     */
    cl::Context& getContext() { return context; }

    /** Obtain the actual OpenCL command queue encapsulated by this
        class.

        \return The actual OpenCL command queue encapsulated by this
        class.
    */
    cl::CommandQueue& getQueue() { return queue; }

    /** Convenience method to make a cl::size_t<3> vector using x, y,
        and z values.

        \param[in] x The value for the x coordinate.

        \param[in] y The value for the y coordinate.

        \param[in] z The value for the z coordinate.

        \return This method returns a cl::size_t vector that has
        exactly three entries in it.
    */
    cl::size_t<3> makeCoords(size_t x, size_t y, size_t z);

private:
    /// The OpenCL context associated with queues and buffers
    cl::Context context;
    /// The command queue for executing the kernel
    cl::CommandQueue queue;
    /// The device associated with the command queue
    cl::Device device;
};

#endif
