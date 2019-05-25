#include "../zero.h"

#define makeRGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))

int fb[256 * 256];

void main(void) {
  int *p = fb;

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      *p = makeRGB(y, x, 255 - x);
      p++;
    }
  }

  zero_open("Zero wingdi - RGB example", 256, 256, 3, 2);

  while(1) {
    zero_update(fb, NULL);
  }
}

