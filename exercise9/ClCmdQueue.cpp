// Copyright 2014 raodm@miamiOH.edu

#ifndef CL_CMD_QUEUE_CPP
#define CL_CMD_QUEUE_CPP

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include "ClCmdQueue.h"

/// Convenient Read-Only flags (CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR)
const cl_mem_flags ClCmdQueue::ROFlags = CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR;
/// Convenient Read-Write flags (CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR)
const cl_mem_flags ClCmdQueue::RWFlags = CL_MEM_READ_WRITE |
    CL_MEM_USE_HOST_PTR;
/// Convenient Read-Write flags (CL_MEM_WRITE_ONLY | CL_MEM_USE_HOST_PTR)
const cl_mem_flags ClCmdQueue::WOFlags = CL_MEM_WRITE_ONLY |
    CL_MEM_USE_HOST_PTR;

ClCmdQueue::ClCmdQueue(cl_device_type devType, int platform) throw(cl::Error) {
    std::vector<cl::Platform> ptList;
    cl::Platform::get(&ptList);
    // The list of devices to be populated below
    std::vector<cl::Device> deviceList;
    // Populate the device list depending on platfrom
    const int MaxPlat = (platform == -1) ? ptList.size() : (platform + 1);
    platform = (platform == -1) ? 0 : platform;
    for (int i = platform; ((i < MaxPlat) && deviceList.empty()); i++) {
        // Get the list of devices on the platform
        try {
            ptList[i].getDevices(devType, &deviceList);
        } catch (cl::Error& err) {
            if (err.err() != CL_DEVICE_NOT_FOUND) {
                throw err;
            }
        }
    }
    if (deviceList.empty()) {
        throw cl::Error(CL_DEVICE_NOT_FOUND);
    }
    // Create a context & queue using the device number
    context = cl::Context(deviceList);
    device  = deviceList[0];
    queue   = cl::CommandQueue(context, device);
}

cl::Program
ClCmdQueue::createProgram(const std::string& source) throw(cl::Error) {
    // Create the program from source
    cl::Program::Sources sources(1, std::make_pair(source.c_str(), 0));
    cl::Program program(context, sources);
    // Build Open CL program for a given device
    try {
        program.build({ device });
    } catch (cl::Error error) {
        std::cout << "Build log:" << std::endl
                  << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
                  << std::endl;
        throw error;
    }
    return program;
}

cl::Program
ClCmdQueue::loadProgram(const std::string& path) throw(cl::Error) {
    // Open the input file stream for reading
    std::ifstream dataFile(path);
    if (!dataFile.good()) {
        throw cl::Error(CL_INVALID_PROGRAM);
    }
    // Load OpenCL program from the file.
    dataFile >> std::noskipws;
    std::istream_iterator<char> dataIter(dataFile), eof;
    std::string source(dataIter, eof);
    // Get utility method to create the program from the source
    return createProgram(source);
}

cl::Kernel
ClCmdQueue::loadKernel(const std::string& path,
                       const std::string& kernel) throw(cl::Error) {
    cl::Program program = loadProgram(path);
    return cl::Kernel(program, kernel.c_str());
}

cl::Buffer
ClCmdQueue::makeBuffer(void *hostPtr, size_t size,
                       cl_mem_flags flags) throw(cl::Error) {
    return cl::Buffer(context, flags, size, hostPtr);
}

cl::Image2D
ClCmdQueue::makeImage(void* hostPtr, size_t width, size_t height,
                      cl_mem_flags flags, size_t pitch,
                      cl_channel_order format, cl_channel_type pixType) {
    return cl::Image2D(context, flags, cl::ImageFormat(format, pixType),
                       width, height, pitch, hostPtr);
}

cl::size_t<3>
ClCmdQueue::makeCoords(size_t x, size_t y, size_t z) {
    cl::size_t<3> vec;
    vec.push_back(x);
    vec.push_back(y);
    vec.push_back(z);
    return vec;
}

void
ClCmdQueue::enqueueMapBuffer(cl::Buffer& buf, size_t size,
                             cl_mem_flags flags, bool blocking) {
    queue.enqueueMapBuffer(buf, blocking, flags, 0, size);
}

void
ClCmdQueue::enqueueMapImage(cl::Image2D& img, cl_mem_flags flags,
                            bool blocking) {
    // Set origin to 0,0,0 offset within the image to copy from
    cl::size_t<3> origin = makeCoords(0, 0, 0);
    // Set the region to be copied based on the width and height of image
    size_t width = 0, height = 0;
    img.getImageInfo<size_t>(CL_IMAGE_WIDTH, &width);
    img.getImageInfo<size_t>(CL_IMAGE_HEIGHT, &height);
    cl::size_t<3> region = makeCoords(width, height, 1);
    // The following two variables are not really used by are needed for the
    // API call.
    size_t rowPitch  = 0, slicePitch = 0;
    queue.enqueueMapImage(img, blocking, flags, origin,  region,
                          &rowPitch, &slicePitch);
}

#endif
