#include "../zero.h"

int main(int argc, char **argv){

  unsigned int fb[256 * 256];
  unsigned int *p = fb;

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      int dx = 128 - x;
      int dy = 128 - y;
      int dist = sqrt(dx * dx + dy * dy) * 2;
      *p = MAKE_RGBA(y, x, 255 - x, dist > 255 ? 255: dist);
      p++;
    }
  }

  zero_open("rgba.png", 256, 256, 4, 1);
  zero_update(fb, NULL);
  return 0;
}
