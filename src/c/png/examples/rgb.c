#include "../zero.h"

int main(int argc, char **argv){

  unsigned char *fb = (unsigned char *)createFrameBuffer(256, 256, 3);

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
      fb[y * 256 * 3 + x * 3 + 0] = y;
      fb[y * 256 * 3 + x * 3 + 1] = x;
      fb[y * 256 * 3 + x * 3 + 2] = 255 - x;
    }
  }

  return drawFrameBuffer(fb, "rgb.png", 0);
}
