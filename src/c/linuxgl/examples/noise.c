#include "../zero.h"

void main(){
  unsigned char framebuffer[320 * 200 * 4];
  zero_open("Noise app", 320, 200, 4, 2);
  int frames = 200;
  while(frames--){
    for (int i = 0; i < 320 * 200 * 4; i++){
      framebuffer[i] = rand() % 255;
    }
    zero_update(framebuffer, NULL);
  }
}