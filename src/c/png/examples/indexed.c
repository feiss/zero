#define _USE_MATH_DEFINES
#include "../zero.h"

int main(int argc, char **argv){

  unsigned char fb [256 * 256];
  unsigned char pal[256 * 3];

  for(int i = 0; i < 256; i++){
    pal[i * 3 + 0] = i / 2 + 128;
    pal[i * 3 + 1] = i / 2;
    pal[i * 3 + 2] = 255 - i;
  }

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      fb[y * 256 + x] = x;
    }
  }

  zero_open("indexed.png", 256, 256, 1, 1);
  zero_update(fb, pal);
  return 0;
}
