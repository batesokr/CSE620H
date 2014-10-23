__kernel void toggle(__global char * str, __global const char * const chars) {
  size_t i = get_global_id(0);
  size_t j = get_global_id(1);

  if(str[i] == chars[j]) {
    str[i] = (int)str[i] - 32;
  } else if ((int)str[i] + 32 == (int)chars[j]) {
    str[i] = (int)str[i] + 32;
  }
}
