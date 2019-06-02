#include "../zero.h"

static int run = 1;
static int drawing = 0;

int fb[256 * 256];

void event_handler(int type, int param1, int param2) {
	switch (type) {
		case MOUSE_DOWN: drawing = 1; break;
		case MOUSE_UP: drawing = 0; break;
		case KEY_DOWN: run = 0; break;
		case MOUSE_MOVE: 
			if (drawing) fb[param2 * 256 + param1] = 0xFFFFFFFF;
			break;
	}
}

int main (int argc, char **argv){

	zero_open("Zero linuxgl - Paint example", 256, 256, 4, 1);
	zero_events(event_handler);
	
  while(run){
  	zero_update(fb, NULL);
  }

  return 0;
}