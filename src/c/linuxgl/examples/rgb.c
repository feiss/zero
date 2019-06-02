#include "../zero.h"

static int run = 1;

void event_handler(int type, int param1, int param2) {
	switch (type) {
		case MOUSE_UP: run = 0; break;
	}
	printf("event %i (%i, %i)\n", type, param1, param2);
}

int main (int argc, char **argv){
  int fb[256 * 256];
  int *p = fb;

  for(int y = 0; y < 256; y++) {
    for(int x = 0; x < 256; x++) {
    	unsigned char c = (y+x)%2 == 0 ? 0 : 1;
      *p = MAKE_RGB(y * c, x * c, 255 - x);
      p++;
    }
  }


	zero_open("Zero linuxgl - rgb example", 256, 256, 4, 2);
	zero_events(event_handler);
	
  while(run){
  	zero_update(fb, NULL);
  }

  return 0;
}