#include "../zero.h"

void main(void) {
  int w, h, d, line;
  zero_open(NULL, &w, &h, &d, 1, &line);

  char *fb = (char *)malloc(w * h * d);
  char *p=fb;

  for(int y = 0; y < h; y++) {
    for(int x = 0; x < w; x++) {
      p[0] = (int)(((float)x/w)*255.f);
      p[1] = (int)(((float)y/h)*255.f);
      p[2] = 0;
      p += d;
    }
  }
  
  // 3 secs, assuming fbdev being 60fps
  int loop = 60 * 3;
  while(--loop) {
    zero_update(fb, NULL);
  }

  free(fb);
  zero_close();

  printf ("%ix%i, %i Bytes per pixel, line length: %i\n", w, h, d, line);
}

