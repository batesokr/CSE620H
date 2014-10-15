#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>

#include <string>
#include <iterator>
#include <vector>
#include <iostream>
#include <fstream>

const std::string cl_file = "batesokr_toggle.cl";
const std::string cl_entry_func = "toggle";

std::vector<cl::Device> getDeviceList() {
  // Obtain the platforms
  std::vector<cl::Platform> platformList;
  cl::Platform::get(&platformList);

  // Pick the first platform and use its GPU device
  cl_device_type devType = CL_DEVICE_TYPE_GPU;
  std::vector<cl::Device> deviceList;
  platformList[0].getDevices(devType, &deviceList);
  return deviceList;
}

cl::Kernel getKernel(cl::Device device, cl::Context& context, 
		      const std::string& fileName, 
		      const std::string& entry_func) throw (cl::Error) {
  // Read in the OpenCL program from another file
  std::ifstream is (fileName, std::ios::in);
  std::istream_iterator<char> isi (is), eof;
  std::string file_str (isi, eof);

  std::cout << file_str << std::endl;

  // Create the OpenCL program from source
  cl::Program::Sources sources(1, std::make_pair(file_str.c_str(), 0));
  cl::Program program(context, sources);

  // Build the program
  program.build({ device });

  // Create the kernel
  cl::Kernel kernel (program, entry_func.c_str());

  return kernel;
}

void toggle_data(std::string& data, const char* const argv[], const int& argc) {
  try {
    // Get the device list
    std::vector<cl::Device> deviceList = getDeviceList();
    
    // Create context and queue
    cl::Context context = cl::Context(deviceList);
    cl::Device device = deviceList[0];
    cl::CommandQueue queue = cl::CommandQueue(context, device);
    
    // Get the kernel
    cl::Kernel kernel = getKernel(device, context, cl_file, 
				     cl_entry_func);
    
    // Create a buffer and copy the host's contents
    const int ROFlags = CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR;
    const int byteSize = data.size();
    cl::Buffer buf = cl::Buffer(context,
				ROFlags,
				byteSize,
				const_cast<char*>(&data[0]));
    
    kernel.setArg(0, buf);

    // Schedule the kernel for execution on a device
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
			       cl::NDRange(byteSize), cl::NullRange);

    // Sync the data back to host pointer
    queue.enqueueMapBuffer(buf, CL_TRUE, CL_MAP_READ, 0, byteSize);

  } catch(cl::Error error) {
    if(error.err() == -11) {
      std::cout << "Build log:" << std::endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
    }
    std::cout << error.what() << ": " << error.err() << std::endl;
    
  }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Specify <File> <toggle_char> ...\n";
        return 1;
    }
    
    // Read the file.
    std::ifstream dataFile(argv[1]);
    dataFile >> std::noskipws;
    std::istream_iterator<char> dataIter(dataFile), eof;
    std::string data(dataIter, eof);
    
    // Toggle the data
    toggle_data(data, argv, argc);
    
    // Print the modifed data.
    //std::cout << data;
    
    // Exit program
    return 0;
}
