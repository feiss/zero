#include "../zero.h"

void main(void) {
  unsigned char *fb = zero_open("Zero linux X11 - RGB example", 256, 256, 4, 1);
  unsigned int *fbp = (unsigned int *)fb;

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      *fbp = MAKE_RGBA(y, x, 255 - x, 0);
      fbp++;
    }
  }

  int run = 1;
  while(run){
    run = zero_update(fb, NULL);
  }

  zero_close();
}

