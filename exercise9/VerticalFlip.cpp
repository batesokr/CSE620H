// Copyright 2014 raodm@miamiOH.edu

#define __USE_MATH_DEFINES

#include <stdexcept>
#include <string>
#include <cmath>
#include "ClCmdQueue.h"
#include "BitmapImage.h"

// A couple of forward declarations to keep compiler happy
int getDevType(const std::string& device) throw(std::runtime_error);
BitmapImage clVflip(ClCmdQueue& queue, cl::KernelFunctor& vflip,
                    BitmapImage& srcImg) throw(cl::Error);
BitmapImage clRotate(ClCmdQueue& queue, cl::KernelFunctor& vflip,
		     BitmapImage& srcImg, float theta) throw(cl::Error);


// A simple tester for testing operation of toggling characters via OpenCL
int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cout << "Specify [CPU|GPU|ACC] <InputBMP> <OutputBMP> <theta_radians>\n";
        return 1;
    }
    // Load source image from the given file
    BitmapImage srcImg;
    srcImg.load(argv[2]);
    // Create the OpenCL command queue for a given device
    ClCmdQueue queue(getDevType(argv[1]));
    // Create a kernel and functor using OpenCL kernel code from a file.
    cl::Kernel kernel = queue.loadKernel("batesokr_rotate.cl", "img_rotate");
    cl::NDRange globalRange(srcImg.getWidth(), srcImg.getHeight());
    cl::KernelFunctor vrotate(kernel, queue.getQueue(), cl::NullRange,
                            globalRange, cl::NullRange);
    BitmapImage outImg = clRotate(queue, vrotate, srcImg, atof(argv[4])*(M_PI/180.0f));
    outImg.write(argv[3]);
    return 0;
}

BitmapImage
clRotate(ClCmdQueue& queue, cl::KernelFunctor& vrotate,
	 BitmapImage& srcImg, float theta) throw (cl::Error) {
  // Create a new bitmap and scale it
    BitmapImage outImg = srcImg;
    float imgMax = sqrt(pow(srcImg.getWidth(),2) + pow(srcImg.getHeight(),2));
    outImg.resize(imgMax, imgMax);
    // Create the input, output image, and filter buffers!
    cl::Image2D srcBuf = queue.makeImage(srcImg.getRow(0), srcImg.getWidth(),
                                         srcImg.getHeight(), queue.ROFlags);
    cl::Image2D outBuf = queue.makeImage(outImg.getRow(0), outImg.getWidth(),
                                         outImg.getHeight(), queue.WOFlags);
    // Setup sampler for source image to control sampler
    cl::Sampler sampler(queue.getContext(), CL_FALSE, CL_ADDRESS_CLAMP,
                        CL_FILTER_NEAREST);
    // Now use the OpenCl kernel to perform convolution
    vrotate(srcBuf, outBuf, outImg.getWidth(), outImg.getHeight(), sampler, 
	    (float)cos(theta), (float)sin(theta));
    // Copy result image back from the device to host.
    queue.enqueueMapImage(outBuf);
    // Return the resulting image back to the caller.
    return outImg;
}

BitmapImage
clVflip(ClCmdQueue& queue, cl::KernelFunctor& vflip,
        BitmapImage& srcImg) throw (cl::Error) {
    BitmapImage outImg = srcImg;
    // Create the input, output image, and filter buffers!
    cl::Image2D srcBuf = queue.makeImage(srcImg.getRow(0), srcImg.getWidth(),
                                         srcImg.getHeight(), queue.ROFlags);
    cl::Image2D outBuf = queue.makeImage(outImg.getRow(0), outImg.getWidth(),
                                         outImg.getHeight(), queue.WOFlags);
    // Setup sampler for source image to control sampler
    cl::Sampler sampler(queue.getContext(), CL_FALSE, CL_ADDRESS_CLAMP,
                        CL_FILTER_NEAREST);
    // Now use the OpenCl kernel to perform convolution
    vflip(srcBuf, outBuf, srcImg.getWidth(), srcImg.getHeight(), sampler);
    // Copy result image back from the device to host.
    queue.enqueueMapImage(outBuf);
    // Return the resulting image back to the caller.
    return outImg;
}

int getDevType(const std::string& device) throw(std::runtime_error) {
    const std::string DevTypes = "CPUGPUACC";
    switch (DevTypes.find(device)) {
    case 0: return CL_DEVICE_TYPE_CPU;
    case 3: return CL_DEVICE_TYPE_GPU;
    case 6: return CL_DEVICE_TYPE_ACCELERATOR;
    }
    throw std::runtime_error("Invalid device type specified");
}

// End of source code
