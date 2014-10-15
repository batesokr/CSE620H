__kernel void toggle(__global char * string) {
  size_t i = get_global_id(0);
  c[i] = 'h';
}
