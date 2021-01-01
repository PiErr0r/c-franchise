#include <stdio.h>


// set n bits of x that begin at position p to the rightmost n bits of y
int set_bits(int x, int p, int y, int n) {
  return p >= n ? (x & ~(~(~0 << n) << p+1-n) | (y & ~(~0 << n)) << p+1-n) : (y & ~(~0 << p) >> n+1-p | x);
}
// invert n bits of x from position p (1->0 and 0->1)
int invert(int x, int p, int n) {
  return (~x & (~(~0 << n) << p+1-n)) | (x & ~(~(~0 << n) << p+1-n));
}

int rightrot(int x, int n) {
  int s = sizeof(x) * 8;
  return (x >> n) | (x << s - n);
}

int main(void) {
  return 0;
}
