#include "../zero.h"

void main(void) {
  int w, h, d, line;
  zero_open(NULL, &w, &h, &d, 1, &line);

  char *fb = (char *)malloc(w * h * d);
  char *p;

  for(int y = 0; y < 256; y++) {
    p = fb + y * line;
    for(int x = 0; x < 256; x++) {
      p[0] = y;
      p[1] = x;
      p[2] = 255 - x;
      p += d;
    }
  }
  
  // 5 secs, assuming fbdev being 60fps
  int loop = 60 * 5;
  while(--loop) {
    zero_update(fb, NULL);
  }

  free(fb);
  zero_close();

  printf ("%ix%i, %i Bytes per pixel, line length: %i\n", w, h, d, line);
}

