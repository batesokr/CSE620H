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
  is >> std::noskipws;
  std::istream_iterator<char> isi (is), eof;
  std::string file_str (isi, eof);

  // Create the OpenCL program from source
  cl::Program::Sources sources(1, std::make_pair(file_str.c_str(), 0));
  cl::Program program(context, sources);

  // Build the program
  try {
    program.build({ device });
  } catch(cl::Error error) {
    if(error.err() == -11) {
      std::cout << "Build log:" << std::endl
		<< program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
    } else {
      throw error;
    }
  }
  // Create the kernel
  cl::Kernel kernel (program, entry_func.c_str());

  return kernel;
}

void toggle_data(std::string& data, const std::string& toggle_chars) {
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
    cl::Buffer buf1 = cl::Buffer(context,
				 CL_MEM_READ_WRITE | CL_MEM_USE_HOST_PTR,
				 data.size(),
				 const_cast<char*>(&data[0]));
    
   
    cl::Buffer buf2 = cl::Buffer(context,
				 CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR,
				 toggle_chars.size(),
				 const_cast<char*>(&toggle_chars[0])); 
    kernel.setArg(0, buf1);
    kernel.setArg(1, buf2);

    // Schedule the kernel for execution on a device
    queue.enqueueNDRangeKernel(kernel, cl::NullRange,
			       cl::NDRange(data.size(), toggle_chars.size()), 
			       cl::NullRange);

    // Sync the data back to host pointer
    queue.enqueueMapBuffer(buf1, CL_TRUE, CL_MAP_READ, 0, data.size());

  } catch(cl::Error error) {
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
    
    // Get the toggle characters
    std::string tcs (argv[2], argv[argc-1]);
    tcs.push_back(argv[argc-1][0]);

    // Toggle the data
    toggle_data(data, tcs);
    
    // Print the modifed data.
    std::cout << data << std::endl;

    // Exit program
    return 0;
}
