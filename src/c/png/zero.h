#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void zero_open(char *title, int w, int h, int d, int s);
int zero_update(unsigned char *fb, unsigned char *pal);


// IMPLEMENTATION

static int width, height, depth, scale;
static char filename[1000];

void zero_open(char *title, int w, int h, int d, int s){
  width = w <= 0 ? 1 : w;
  height= h <= 0 ? 1 : h;
  depth = d != 1 ? 3 : 1;
  scale = s <= 0 ? 1 : s;
  int i = 0;
  while(title[i]) {
    filename[i] = title[i];
    i++;
  }
  filename[i] = 0;
}

int zero_update(unsigned char *fb, unsigned char *pal){
  unsigned char *fb_indexed = 0;
  unsigned char *fb_scaled = 0;
  unsigned char *frame = fb;

  if (depth == 1 && pal) {
    int col;
    fb_indexed = (unsigned char*)malloc(width * height * 3);
    for (int i = 0; i < width * height; i++) {
      col = fb[i];
      fb_indexed[i * 3 + 0] = pal[col * 3 + 0];
      fb_indexed[i * 3 + 1] = pal[col * 3 + 1];
      fb_indexed[i * 3 + 2] = pal[col * 3 + 2];
    }
    frame = fb_indexed;
  }

  if (scale > 1) {
    int from, to;
    fb_scaled = (unsigned char*)malloc(width * scale * height * scale * 3);
    for (int y = 0; y < height * scale; y++) {
      for (int x = 0; x < width * scale; x++) {
        from = y / scale * width + x / scale;
        to = y * width * scale + x;
        fb_scaled[to * 3 + 0] = frame[from * 3 + 0];
        fb_scaled[to * 3 + 1] = frame[from * 3 + 1];
        fb_scaled[to * 3 + 2] = frame[from * 3 + 2];
      }
    }
    frame = fb_scaled;
  }

  int result = stbi_write_png(filename, width * scale, height * scale, 3, frame, 0);

  if (depth == 1 && pal) free(fb_indexed);
  if (scale > 1) free(fb_scaled);
  return result;
}

