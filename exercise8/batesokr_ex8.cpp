#define __CL_ENABLE_EXCEPTIONS

#include <CL/cl.hpp>

#include<string>
#include<iterator>
#include<vector>

const std::string cl_file = "toggle.cl";

std::vector<cl::Device> getDeviceList() {
  // Obtain the platforms
  std::vector<cl::Platform> platformList;
  cl::Platform::get(&platformList);

  // Pick the first platform and use its GPU device
  cl_device_type devType = CL_DEVICE_TYPE_GPU;
  std::vector<cl::Device> deviceList;
  platform[0].getDevices(devType, &deviceList);
  return deviceList;
}

cl::Program getProgram(std::vector<cl::Device>& deviceList, cl::Context& context, 
		       const std::string& fileName) throw (cl::Error) {
  
}

cl::Kernel getKernel(cl::Context& context, cl::CommandQueue& queue,
		     const std::string& fileName = "toggle.cl",
		     const std::string& entryFunc = "toggle") throw (cl::Error) {
  
}

void toggle_data(std::string& data, const char* const argv[], const int& argc) {
  // Get the device list
  std::vector<cl::Device> deviceList = getDeviceList();
  
  // Create context and queue
  cl::Context context = cl::Context(deviceList);
  cl::Device device = deviceList[0];
  cl::CommandQueue queue = cl::CommandQueue(context, device);

  // Get the program
  cl::Program program = 
  
  // Replace specified charachters
  for(int curr = 2; (curr < argc); curr++) {
    toggle(data, argv[curr][0]);
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
    std::cout << data;
    
    // Exit program
    return 0;
}
