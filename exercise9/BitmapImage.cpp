// Copyright 2014 raodm@miamioh.edu

#include "BitmapImage.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

void
BitmapImage::resize(int width, int height, uint32_t color)  {
    // Setup properties in the header.
    header.id[0]        = 'B';
    header.id[1]        = 'M';
    header.width        = width;
    header.height       = height;
    header.clrPlanes    = 1;
    header.colorDepth   = 24;
    header.compression  = 0;
    header.hReso        = 0;
    header.vReso        = 0;
    header.palette      = 0;
    header.impClrs      = 0;
    // resize the buffer.
    pixData.clear();
    pixData.resize(width * height * sizeof(uint32_t), color);
}

void
BitmapImage::load(const std::string& path) throw(std::exception) {
    std::ifstream file(path);
    if (!file.good()) {
        throw std::runtime_error("Unable to open file: " + path);
    }
    load(file);
}

void
BitmapImage::load(std::istream& is) throw(std::exception) {
    // Load the file header at the beginning of the BMP
    if (!is.read(reinterpret_cast<char*>(&header), sizeof(header)).good() ||
        !isValid(header)) {
        throw std::runtime_error("Error loading BMP or invalid BMP file");
    }
    // Resize our pixel data buffer to hold the necessary pixels
    pixData.resize(header.width * header.height * sizeof(uint32_t));
    // Read each row into a vector based on color depth
    const int bytesPerRow = header.width * header.colorDepth / 8;
    std::vector<char> tempBuf(bytesPerRow);
    // Now load the data from the file row-by-row accounting for padding.
    const int padSize = (4 - (bytesPerRow % 4)) % 4;
    is.seekg(header.offset);
    for (int row = 1; (row <= header.height); row++) {
        if (!is.read(&tempBuf[0], bytesPerRow).good()) {
            throw std::runtime_error("Error reading BMP pixel data");
        }
        // Convert 24-bit colors to 32-bit colors with alpha set to 255
        convert(&tempBuf[0], getRow(header.height - row), header.width);
        // Skip padding bytes
        is.ignore(padSize);
    }
}

void
BitmapImage::convert(const char* const rgb, uint32_t* const rgba,
                     const int columns) {
    const uint8_t* src = reinterpret_cast<const uint8_t*>(rgb);
    for (int destCol = 0, srcCol = 0; (destCol < columns);
        destCol++, srcCol += 3) {
        rgba[destCol] = (src[srcCol + 2] << 16) | (src[srcCol + 1] << 8) |
            src[srcCol];
    }
}

void
BitmapImage::convert(const uint32_t* rgba, char* rgb, const int columns) {
    const char* src = reinterpret_cast<const char*>(rgba);
    for (int srcCol = 0, destCol = 0; (srcCol < columns * 4);
        srcCol += 4, destCol += 3) {
        rgb[destCol + 0]  = src[srcCol + 0];
        rgb[destCol + 1]  = src[srcCol + 1];
        rgb[destCol + 2]  = src[srcCol + 2];
    }
}

bool
BitmapImage::isValid(const struct Header& header) throw(std::exception) {
    if ((header.id[0] != 'B') || (header.id[1] != 'M')) {
        throw std::runtime_error("Invalid BMP header identifier (must be BM)");
    }
    if (header.offset > header.size) {
        throw std::runtime_error("Pixel data offset exceeds BMP file size");
    }
    if (header.headerSize < 40) {
        throw std::runtime_error("Invalid BMP info header size (must be 40)");
    }
    if (header.clrPlanes != 1) {
        throw std::runtime_error("Invalid number of color planes (must be 1)");
    }
    if (header.colorDepth != 24) {
        throw std::runtime_error("Bitmap must be 24-bit (no alpha)");
    }
    if (header.compression != 0) {
        throw std::runtime_error("Compressed BMP not handled");
    }
    if (header.palette != 0) {
        throw std::runtime_error("Non-default color palette not handled");
    }
    // This header looks good.
    return true;
}

void
BitmapImage::write(std::ostream& os) throw(std::exception) {
    header.colorDepth = 24;  // Reset color depth to 24-bit data.
    // Compute number of bytes of data to write for each row.
    const int bytesPerRow = header.width * header.colorDepth / 8;
    // Compute padding to be added to each row as required by BMP format
    const int padSize = (4 - (bytesPerRow % 4)) % 4;
    // Setup properties in the header.
    header.imgSize    = header.height * (bytesPerRow + padSize);
    header.size       = sizeof(header) + header.imgSize;
    header.offset     = sizeof(header);
    header.headerSize = 40;
    // Write the header
    if (!os.write(reinterpret_cast<char*>(&header), sizeof(header)).good()) {
        throw std::runtime_error("Error writing BMP header!");
    }
    const char pad[4] = {0, 0, 0, 0};
    std::vector<char> tempBuf(bytesPerRow);
    // Write row-by-row to the file in 24-bits.
    for (int row = header.height - 1; (row >= 0); row--) {
        // Convert from 32-bit to 24-bit format
        convert(getRow(row), &tempBuf[0], header.width);
        if (!os.write(&tempBuf[0], bytesPerRow).good()) {
            throw std::runtime_error("Error writing BMP pixel data");
        }
        // Write padding as needed
        os.write(pad, padSize);
    }
}

void
BitmapImage::write(const std::string& path) throw(std::exception) {
    std::ofstream file(path);
    if (!file.good()) {
        throw std::runtime_error("Unable to open file: " + path);
    }
    write(file);
}

