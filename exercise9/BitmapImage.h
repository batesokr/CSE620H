// Copyright 2014 raodm@miamioh.edu

#ifndef BITMAP_IMAGE_H
#define BITMAP_IMAGE_H

#include <cstdint>
#include <string>
#include <vector>

/** A simple class to read/write 24-bit Bitmap images to/from flat
    arrays.

    \note The bitmap must be 24-bit/32-bit and uncompressed!

    This class provides a convenient wrapper to reader and write
    bitmap images.  The data is read/written to/from a flat array of
    bytes that can be accessed via call to the getRow() method.  This
    class can be used in the following manners:

    <ul>

    <li>Load bitmap image from a given file:
    
    \begincode
    BitmapImage img;
    img.load("test.bmp");    
    \endcode
    </li>

    <li>create an empty (all black) bitmap image :

    \begincode    
    BitmapImage img;
    img.resize(100, 100);
    img.write("test.bmp");
    \endcode
    </li>
    </ul>
*/

class BitmapImage {
    struct Header;
public:
    /** Load bitmap from a given input stream.

        This method resizes the bitmap as needed.

        \param[in,out] is The input stream from where the bitmap is to
        be read.
    */
    void load(std::istream& is) throw(std::exception);

    /** Load bitmap from a given file.

        This method essentially opens the specified file and uses the
        the overloaded load(std::istream&) method to load the bitmap
        image from the specified file into memory.

        \param[in,out] path The path to the input file from where the
        data is to be read.
    */
    void load(const std::string& path) throw(std::exception);

    /** Write the bitmap to a given output stream.

        This method writes the data in 32-bit format in uncompressed
        format.

        \param[in,out] os The output stream to where the bitmap is to
        be written.
    */
    void write(std::ostream& os) throw(std::exception);

    /** Write bitmap from a given output stream.

        This method essentially opens the specified file and uses the
        the overloaded write(std::ostream&) method to write the bitmap
        image to the specified file.

        \param[in,out] path Path to the file to where the image is to
        be written.
    */
    void write(const std::string& path) throw(std::exception);

    /** Obtain a pointer to the pixels in a given row.

        This is a convenience method that can be used to obtain a
        pointer to the first pixel in a given row. Use getRow(0) to
        obtain pointer to first pixel in image.

        \param[in] row The row in the image to which the pointer is to
        be returned.  Result of passing-in invalid rows is undefined.

        \return Pointer to first pixel in the given row.
    */
    uint32_t* getRow(const int row) { return &pixData[row * header.width]; }

    /** The width of the image.

        \return The wide of the image.
    */
    uint32_t getWidth() const { return header.width; }

    /** The height of the image

        \return The height of the image.
    */
    uint32_t getHeight() const { return header.height; }

    /** Resize the image to the specified size.

        When images are resized, all existing information is cleared
        and the image is resized to a black bit map.

        \param[in] width The width of the image (in number of pixels).

        \param[in] height The height of the image (in number of pixels).
    */
    void resize(int width, int height, uint32_t color = 0);

protected:
    /** Helper method to check if the BMP header is compatible.

        This method is a convenience method that is used to perform
        some basic sanity checks on the bitmap data loaded from a
        given file.

        \parma[in] header THe bit map header to be validated for
        compatibility.
     */
    bool isValid(const struct Header& header) throw(std::exception);

    /** Convenience method to convert rgb value to rgba.

        \param[in] rgb Reference to 3-byte RGB value.

        \param[out] rgba Pointer to the 4-byte RGBA location into
        which the data is to be stored.
    */
    void convert(const char* const rgb, uint32_t* const rgba, const int cols);

    /** Convenience method to convert rgba value to rgb.

        \param[in] rgba Reference to 4-byte RGB value.

        \param[out] rgb Pointer to the 3-byte RGB location into
        which the data is to be stored.
    */    
    void convert(const uint32_t* rgba, char *rgb, const int cols);

private:
    /** The flat memory to store pixels associated with a given
        Bitmap.  The data is always stored in RGBA order.
    */
    std::vector<uint32_t> pixData;

#pragma pack(push)
#pragma pack(1)
    struct Header {
        char     id[2];        // must be 0x424d
        uint32_t size;         // size of BMP file in bytes
        uint16_t reserved1;    // unused
        uint16_t reserved2;    // unused
        uint32_t offset;       // Starting address of pixel data
        uint32_t headerSize;   // Size of info header. Must be 40 bytes
        int32_t  width;        // width in pixels (signed)
        int32_t  height;       // height in pixels (signed)
        int16_t  clrPlanes;    // color planes must be 1
        int16_t  colorDepth;   // Bits per pxiel 1,4,8,16,24, or 32
        uint32_t compression;  // Compression method used. Must be zero
        uint32_t imgSize;      // Raw size of image in bytes
        int32_t  hReso;        // Horizontal resolution (pixels/meter)
        int32_t  vReso;        // Vertical resolution (pixels/meter)
        uint32_t palette;      // Colors in color palette 0 or 2^n
        uint32_t impClrs;      // Important colors (generally ignored)
    } header;
#pragma pack(pop)
};

#endif
