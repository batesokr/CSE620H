__kernel
void vflip(__read_only image2d_t  srcImage, __write_only image2d_t  outImage, 
           int width, int height, sampler_t sampler) {
  // Store each work-item’s unique row and column
  const int2 coords = {get_global_id(0), get_global_id(1)};
  const int2 flipCoords = {coords.x, height - coords.y - 1};
  // Bounds checking
  if(coords.x > 0 && coords.x < width && coords.y > 0 && coords.y < height) {
    // Copy pixel from top of image to bottom of output image!
    uint4 pixel = read_imageui(srcImage, sampler, coords);
    write_imageui(outImage, flipCoords, pixel);
  }
}
