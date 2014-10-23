__kernel 
void img_rotate(__read_only image2d_t srcImage, __write_only image2d_t outImage, 
		int width, int height, sampler_t sampler,
		float cosTheta, float sinTheta) {
  // Obtain the global coordinages for this work item
  const int2 coords = {get_global_id(0), get_global_id(1)};
  
  // Obtain the rotated coordinates
  const int2 origin = {width/2, height/2};
  const int2 dist = {cosTheta*origin.x, sinTheta*origin.y};
  const int2 rotated_coords = {
    (cosTheta * (coords.x - origin.x) + sinTheta * (coords.y - origin.y)) + origin.x, 
    (cosTheta * (coords.y - origin.y) - sinTheta * (coords.x - origin.x)) + origin.y
  };
  
  // Rotate the pixel
  if(rotated_coords.x > 0 && rotated_coords.x < width
     && rotated_coords.y > 0 && rotated_coords.y < height) {
    uint4 pixel = read_imageui(srcImage, sampler, coords);
    write_imageui(outImage, rotated_coords, pixel);
    }
}
