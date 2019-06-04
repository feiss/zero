#define FPS_COUNTER
#include "../zero.h"

void main(void) {
  unsigned char fb [256 * 256];
  unsigned char pal[256 * 3];

  for(int i = 0; i < 256; i++){
    pal[i * 3 + 0] = i / 2 + 128;
    pal[i * 3 + 1] = i / 2;
    pal[i * 3 + 2] = 255 - i;
  }

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      fb[y * 256 + x] = y|x;
    }
  }

  zero_open("Zero linuxgl - Indexed example", 256, 256, 1, 1);
  zero_update(fb, pal);

  unsigned char r, g, b;
  int frame = 60 * 2;
  while(frame-- > 0) {
    r = pal[0];
    g = pal[1];
    b = pal[2];
    for(int i = 0; i < 255 * 3; i++){
      pal[i] = pal[i + 3];
    }
    pal[255 * 3 + 0] = r;
    pal[255 * 3 + 1] = g;
    pal[255 * 3 + 2] = b;
    zero_update(fb, pal);
  }
}

