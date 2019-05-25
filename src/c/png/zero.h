#pragma once
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);

#define MAKE_RGB(r,g,b) (((b)<<16) | ((g)<<8) | (r))
#define MAKE_RGBA(r,g,b,a) (((a)<<24) | ((b)<<16) | ((g)<<8) | (r))
#define GET_RED(c)   (((c)>>16) & 0x000000FF)
#define GET_GREEN(c) (((c)>>8) & 0x000000FF)
#define GET_BLUE(c)  ((c) & 0x000000FF)
#define GET_ALPHA(c) (((c)>>24) & 0x000000FF)

// IMPLEMENTATION

static int width, height, depth, scale;
static char filename[1000];

void zero_open(char *title, int w, int h, int d, int s){
  width = w <= 0 ? 1 : w;
  height= h <= 0 ? 1 : h;
  depth = d == 1 ? 1 : (d == 4 ? 4 : 3);
  scale = s <= 0 ? 1 : s;
  int i = 0;
  while(title[i]) {
    filename[i] = title[i];
    i++;
  }
  filename[i] = 0;
}

int zero_update(void *fb, void *pal){
  void *fb_indexed = 0;
  void *fb_scaled = 0;
  void *frame = fb;
  int final_depth = depth == 1 ? 4 : depth;

  if (depth == 1 && pal) {
    unsigned char idx;
    unsigned char *palette = (unsigned char *)pal;
    unsigned int color;
    fb_indexed = malloc(width * height * final_depth);
    for (int i = 0; i < width * height; i++) {
      idx = ((unsigned char*)fb)[i];
      color = MAKE_RGBA(
        palette[idx * 3 + 0],
        palette[idx * 3 + 1],
        palette[idx * 3 + 2],
        255);
      ((unsigned int *)fb_indexed)[i] = color;
    }
    frame = fb_indexed;
  }

  if (scale > 1) {
    int from, to;
    fb_scaled = malloc(width * scale * height * scale * final_depth);
    for (int y = 0; y < height * scale; y++) {
      for (int x = 0; x < width * scale; x++) {
        from = y / scale * width + x / scale;
        to = y * width * scale + x;
        if (final_depth == 3) {
          ((unsigned char*)fb_scaled)[to * 3 + 0] = ((unsigned char*)frame)[from * 3 + 0];
          ((unsigned char*)fb_scaled)[to * 3 + 1] = ((unsigned char*)frame)[from * 3 + 1];
          ((unsigned char*)fb_scaled)[to * 3 + 2] = ((unsigned char*)frame)[from * 3 + 2];
        } else {
          ((unsigned int *)fb_scaled)[to] = ((unsigned int *)frame)[from];
        }
      }
    }
    frame = fb_scaled;
  }

  int result = stbi_write_png(filename, width * scale, height * scale, final_depth, frame, 0);

  if (fb_indexed) free(fb_indexed);
  if (fb_scaled) free(fb_scaled);
  return result;
}

