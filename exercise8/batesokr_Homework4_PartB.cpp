/**
 * Copyright 2014 Keith Batesole
 */

// Enable OpenCL C++ exceptions
#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>

#include "vector_operations.h"


// Convert that the following toggle function to run on a OpenCL device
void toggle(std::string& data, const char low) {
    const char up  = toupper(low);
    auto  toggle = [low,up](char c) {
        return (c == low ? up : c == up ? low : c);
    };
    std::transform(data.begin(), data.end(), data.begin(), toggle);
}

std::string vec_op =
    "__kernel void vec_op(__global float * a, __global float * b, "
    "__global float * c) {"
    "size_t i = get_global_id(0);"
    "c[i] = a[i] + b[i];"
    "}";

void run_program(cl::Context& context, cl::Device& device,
                    cl::CommandQueue& queue, cl::Program program,
                    const NumList& nl1, const NumList& nl2, NumList& nl3) {
  // Create kernel fromm program
  cl::Kernel kernel(program, "vec_op");

  // Create a buffer and copy the host's contents
  const int ROFlags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
  const int byteSize = nl1.size() * sizeof(float);
  cl::Buffer nl1Buf = cl::Buffer(context,
                                 ROFlags,
                                 byteSize,
                                 const_cast<float*>(&nl1[0]));

  cl::Buffer nl2Buf = cl::Buffer(context,
                                 ROFlags,
                                 byteSize,
                                 const_cast<float*>(&nl2[0]));

  cl::Buffer nl3Buf = cl::Buffer(context,
                                 ROFlags,
                                 byteSize,
                                 const_cast<float*>(&nl3[0]));

  // Create more buffers between host and device
  kernel.setArg(0, nl1Buf);
  kernel.setArg(1, nl2Buf);
  kernel.setArg(2, nl3Buf);

  // Schedule the kernel for execution on a device
  queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                             cl::NDRange(byteSize*3), cl::NullRange);

  // Once the kernel has finished running, sync the data back
  // to the host pointer
  queue.enqueueMapBuffer(nl3Buf, CL_TRUE, CL_MAP_READ, 0, byteSize);
}

NumList build_program(const NumList& nl1, const NumList& nl2) {
  // Obtain the platforms
  std::vector<cl::Platform> platformList;
  cl::Platform::get(&platformList);

  // Pick the first platform and use its GPU device
  cl_device_type devType = CL_DEVICE_TYPE_GPU;
  std::vector<cl::Device> deviceList;
  platformList[0].getDevices(devType, &deviceList);

  // Create a context and queue using the first device
  cl::Context context = cl::Context(deviceList);
  cl::Device device = deviceList[0];
  cl::CommandQueue queue = cl::CommandQueue(context, device);

  // Create the OpenCL program from source
  cl::Program::Sources sources(1, std::make_pair(vec_op.c_str(), 0));
  cl::Program program(context, sources);

  // Build and run the OpenCL program for a given device
  NumList nl3(nl1);
  try {
    program.build({ device });
    run_program(context, device, queue, program, nl1, nl2, nl3);
  } catch(cl::Error error) {
    std::cout << error.err() << std::endl;
  }

  return nl3;
}

NumList operator+(const NumList& nl1, const NumList& nl2) {
    vec_op[122] = '+';
    return build_program(nl1, nl2);
}

NumList operator-(const NumList& nl1, const NumList& nl2) {
    vec_op[122] = '-';
    return build_program(nl1, nl2);
}

NumList operator*(const NumList& nl1, const NumList& nl2) {
    vec_op[122] = '*';
    return build_program(nl1, nl2);
}

NumList operator/(const NumList& nl1, const NumList& nl2) {
    vec_op[122] = '/';
    return build_program(nl1, nl2);
}
