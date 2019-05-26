#define FPS_COUNTER
#include "../zero.h"

#define makeRGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))

int fb[256 * 256];

static int noise;
static int carry;
static int seed = 0x12345;

void main(void) {

  zero_open("Zero wingdi - Noise example", 256, 256, 3, 2);

  int *p;
  int i;
  while(1) {
    p = fb;
    for(i = 0; i < 256 * 256; i++) {
      noise = seed;
      noise >>= 3;
      noise ^= seed;
      carry = noise & 1;
      noise >>= 1;
      seed >>= 1;
      seed |= (carry << 30);
      noise &= 0xFF;

      *p = makeRGB(noise, noise, noise);
      p++;
    }
    zero_update(fb, NULL);
  }
}

