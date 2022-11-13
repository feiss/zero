#include "../zero.h"

void main(){
  int w, h, d, line;
  zero_open(NULL, &w, &h, &d, 1, &line);

  char *fb = (char *)malloc(w * h * d);
  char *p=fb;
  
  int frames = 60*3;
  while(frames--){
    for (int i = 0; i < w * h * 4; i++){
      fb[i] = rand() % 255;
    }
    zero_update(fb, NULL);
  }
  free(fb);  
  zero_close();
}