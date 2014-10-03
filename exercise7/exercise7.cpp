// Compiler command-line:
//     make SRC=exercise7.cpp LIBS=-lOpenCL
//
// Alternative (not preferred):
//     g++ -g -Wall -std=c++11 exercise7.cpp -o execise7 -lOpenCL

// Enable OpenCL C++ exceptions
#define __CL_ENABLE_EXCEPTIONS
#include "CL/cl.hpp"

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <iterator>

const std::string vadd =
  "#pragma OPENCL EXTENSION cl_khr_fp64 : enable "
  "__kernel void vadd(__global float * a, __global float * b, "
  "__global float * c) { "
  "size_t i = get_global_id(0); // Get index to operate on "
  "c[i] = a[i] + b[i]; "
  "}";

// Implement the following add method (add suitable helper methods as
// you see fit) to use an OpenCL kernel to add each entry in the the
// two vectors, that is: result[i] = vec1[i] + vec2[i]
std::vector<double>
add(const std::vector<double>& vec1, const std::vector<double>& vec2) {
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
  cl::Program::Sources sources(1, std::make_pair(vadd.c_str(), 0));
  cl::Program program(context, sources);

  // Build the OpenCL program for a given device
  try {
    program.build({ device });
  } catch (cl::Error error) {
    std::cout << "Build log:" << std::endl
              << program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device)
              << std::endl;
    throw error;
  }

  // Create a kernel from a program
  cl::Kernel kernel(program, "vadd");
  
  // Create a buffer and copy host content's
  const int ROFlags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
  const int byteSize = vec1.size() * sizeof(double);
  std::vector<double> vec3(vec1);
  cl::Buffer vec1Buf = cl::Buffer(context,
                                 ROFlags,
                                 byteSize,
                                 const_cast<double*>(&vec1[0]));
  cl::Buffer vec2Buf = cl::Buffer(context,
                                  ROFlags,
                                  byteSize,
                                  const_cast<double*>(&vec2[0]));
  cl::Buffer vec3Buf = cl::Buffer(context,
                                  ROFlags,
                                  byteSize,
                                  const_cast<double*>(&vec3[0]));
  
  // Create more buffers between host and device
  kernel.setArg(0, vec1Buf);
  kernel.setArg(1, vec2Buf);
  kernel.setArg(2, vec3Buf);
  
  // Schedule the kernel for execution on a device
  queue.enqueueNDRangeKernel(kernel, cl::NullRange,
                             cl::NDRange(byteSize*3), cl::NullRange);
  
  // Once the kernel has finished funning, sync the data back
  // to host pointer -- this enforces a sync with
  // the host backing space
  queue.enqueueMapBuffer(vec3Buf, CL_TRUE, CL_MAP_READ, byteSize*2, byteSize);

  return vec3;
}

//-----------------------------------------------------------------------
//           DO  NOT   MODIFY   CODE  BELOW  THIS  LINE
//-----------------------------------------------------------------------

std::vector<double> generate(const int size) {
    int i = 0;
    auto producer = [&i](){ return i++; };
    std::vector<double> list(size);
    std::generate_n(list.begin(), size, producer);
    return list;
}

int
main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage  : " << argv[0] << " <vector_size>\n"
                  << "Example: " << argv[0] << " 20\n";
        return 2;
    }
    // Create the vectors for use.
    const int count = std::stoi(argv[1]);
    const std::vector<double> vec1 = generate(count), vec2 = generate(count);
    // Get the result by doing the addition via OpenCL calls
    const std::vector<double> result = add(vec1, vec2);
    // Print the results
    std::copy(result.begin(), result.end(),
              std::ostream_iterator<double>(std::cout, "\n"));
    // All done.
    return 0;
}

// End of source code
