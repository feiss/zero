#pragma once

unsigned char *zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);
void zero_close();

#define MAKE_RGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))
#define MAKE_RGBA(r,g,b,a) (((a)<<24) | ((r)<<16) | ((g)<<8) | (b))

// implementation

#include<string.h>
#include<stdlib.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<sys/types.h>

static int width, height, depth, scale;
static Window wnd;
static Display *display = 0;
static XImage *bg;
static GC mainGC;
static Atom WM_message[2];

unsigned char *zero_open(char *title, int w, int h, int d, int s) {
  fd_set eventset;
  fd_set zeroset;


  int screen;
  XVisualInfo vinfo;
  XSetWindowAttributes attr;

  int run = 1;

  FD_ZERO(&eventset);
  FD_ZERO(&zeroset);

  if(!(display = XOpenDisplay(0))) { return 0; }
  screen = XDefaultScreen(display);

  width = w;
  height = h;
  depth = d == 1 ? 1 : 4;
  scale = s > 0 ? s : 1;

  if(!XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo)) {
      if(!XMatchVisualInfo(display, screen, 24, TrueColor, &vinfo)) {
          XCloseDisplay(display); 
          return 0;
      }
  }
  attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
  attr.border_pixel = 0;
  attr.background_pixel = 0x80000000;
  attr.bit_gravity = NorthWestGravity;
  attr.win_gravity = NorthWestGravity;

  wnd = XCreateWindow(
    display, DefaultRootWindow(display), 
    0, 0, width * scale, height * scale, 0,
    vinfo.depth, InputOutput, vinfo.visual,
    CWColormap | CWBorderPixel | CWBackPixel | CWBitGravity | CWWinGravity, &attr);

  // Subscribe to events 
  WM_message[0] = XInternAtom(display, "WM_PROTOCOLS", 1);
  WM_message[1] = XInternAtom(display, "WM_DELETE_WINDOW", 1);
  XSetWMProtocols(display, wnd, WM_message, 2);

  XFreeColormap(display, attr.colormap);
  XSelectInput(display, wnd, ExposureMask | ButtonPressMask | KeyPressMask);
  XMapWindow(display, wnd);
  XFlush(display);
  mainGC = XCreateGC(display, wnd, 0, 0);

  unsigned char *fb = (unsigned char *)malloc(width * height * 4);
  bg = XCreateImage(display, CopyFromParent, vinfo.depth, ZPixmap, 0, 
    fb, width, height, vinfo.depth, width * 4);

  return fb;
}


int zero_update(void *fb, void *pal) {
  //XKeyEvent *key;
  XEvent event;
  XNextEvent(display, &event);
  switch(event.type) {
    case Expose:
      XPutImage(display, wnd, mainGC, bg, 0, 0, width, height, width, height);
      break;

    case KeyPress:
    case DestroyNotify:
      return 0;

    case ClientMessage:
      if(event.xclient.message_type == WM_message[0])
        if(event.xclient.data.l[0] == WM_message[1]) {
          return 0;
        }
      break;
  }
  return 1;
}


void zero_close() {
  XDestroyImage(bg);
  XDestroyWindow(display, wnd);
  XCloseDisplay(display);
}
