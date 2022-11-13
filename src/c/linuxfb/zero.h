#pragma once

void zero_open(char *device, int *w, int *h, int *d, int s, int *line);
int  zero_update(void *fb, void *pal);
void zero_close(void);

#define MAKE_RGB(r,g,b) (((b)<<16) | ((g)<<8) | (r))

// implementation
// from https://stackoverflow.com/questions/4996777/paint-pixels-to-screen-via-linux-framebuffer?noredirect=1

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

static int width, height, depth, scale;
//static void *active_palette;
static int fbfd = 0;
static long int screensize = 0;
static char *fbp = 0;
static struct fb_var_screeninfo vinfo;
static struct fb_fix_screeninfo finfo;


void zero_open(char *device, int *w, int *h, int *d, int s, int *line) {

    fbfd = open(device ? device : "/dev/fb0", O_RDWR);
    if (fbfd == -1) { perror("Error: cannot open framebuffer device"); exit(1); }

    if (ioctl(fbfd, FBIOGET_FSCREENINFO, &finfo) == -1) {
        perror("Error reading fixed information");
        exit(2);
    }

    if (ioctl(fbfd, FBIOGET_VSCREENINFO, &vinfo) == -1) {
        perror("Error reading variable information");
        exit(3);
    }

    width = *w = vinfo.xres_virtual;
    height = *h = vinfo.yres_virtual;
    depth = *d = vinfo.bits_per_pixel / 8;
    *line = finfo.line_length;
    scale = s <= 0 ? 1 : s;

    screensize = width * height * vinfo.bits_per_pixel / 8;

    fbp = (char *)mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
    if (fbp == MAP_FAILED) {
        perror("Error: failed to map framebuffer device to memory");
        exit(4);
    }
}


int zero_update(void *fb, void *pal) {
  char *userfb = (char *)fb;
  char *fbpointer = 
    fbp + vinfo.xoffset * (vinfo.bits_per_pixel / 8) + vinfo.yoffset * finfo.line_length;

  for (int i = 0; i < width * height * 4; i += 4) {
    *(fbpointer + i + 0) = *(userfb + i + 2); // blue
    *(fbpointer + i + 1) = *(userfb + i + 1); // green
    *(fbpointer + i + 2) = *(userfb + i + 0); // red
    *(fbpointer + i + 3) = 0; // no transparency
  }
  return 1;
}

void zero_close(void){
  munmap(fbp, screensize);
  close(fbfd);
}
