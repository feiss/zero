#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static int _fb_width, _fb_height, _fb_depth;

static void* createFrameBuffer(int w, int h, int depth);
static int drawFrameBuffer(const void *fb, char const *filename, const unsigned char *palette);


static void* createFrameBuffer(int w, int h, int depth){
  void *fb = calloc(w * h, depth);
  _fb_width  = fb ? w : 0;
  _fb_height = fb ? h : 0;
  _fb_depth  = fb ? depth : 0;
  return fb;
}

static int drawFrameBuffer(const void *fb, char const *filename, const unsigned char *palette){
  const void *_fb;
  if (_fb_depth && palette) {
    _fb = createFrameBuffer(_fb_width, _fb_height, 3);
    for (int i = 0; i < _fb_width * _fb_height; i++) {
      ((unsigned char *)_fb)[i * 3 + 0] = palette[((unsigned char *)fb)[i] * 3 + 0];
      ((unsigned char *)_fb)[i * 3 + 1] = palette[((unsigned char *)fb)[i] * 3 + 1];
      ((unsigned char *)_fb)[i * 3 + 2] = palette[((unsigned char *)fb)[i] * 3 + 2];
    }
  } else {
    _fb = fb;
  }

  return stbi_write_png(filename, _fb_width, _fb_height, _fb_depth, _fb, 0);
}

