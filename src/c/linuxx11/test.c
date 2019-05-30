#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<X11/Xlib.h>
#include<X11/Xutil.h>
#include<sys/stat.h>
#include<sys/time.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
    fd_set eventset;
    fd_set zeroset;
//  struct timeval timeout = {0, 0};

    Display *display = 0;
    int screen;
    Window wnd;
    XVisualInfo vinfo;
    XSetWindowAttributes attr;
    XEvent event;
    XImage *bg;
    GC mainGC;

    Atom WM_message[2];

    int run = 1;

    FD_ZERO(&eventset);
    FD_ZERO(&zeroset);

    if(!(display = XOpenDisplay(0))) {
        /* Display not found */
        printf("Fail display.\n");
        return 0;
    }

    screen = XDefaultScreen(display);

    int depth = 32;

    if(!XMatchVisualInfo(display, screen, 32, TrueColor, &vinfo)) {
        if(!XMatchVisualInfo(display, screen, 24, TrueColor, &vinfo)) {
            /* No proper color depth available */
            XCloseDisplay(display); /* Close X communication */
            printf("No found color display. Sorry.\n");
            return 0;
        }
        depth = 24;
    }

    attr.colormap = XCreateColormap(display, DefaultRootWindow(display), vinfo.visual, AllocNone);
    attr.border_pixel = 0;
    attr.background_pixel = 0x80000000;
    attr.bit_gravity = NorthWestGravity;
    attr.win_gravity = NorthWestGravity;

    wnd = XCreateWindow(display, DefaultRootWindow(display), 0, 0, 100, 100, 0,
        vinfo.depth, InputOutput, vinfo.visual,
        CWColormap | CWBorderPixel | CWBackPixel | CWBitGravity | CWWinGravity, &attr);

    /* Subscribe to window closing event */
    WM_message[0] = XInternAtom(display, "WM_PROTOCOLS", 1);
    WM_message[1] = XInternAtom(display, "WM_DELETE_WINDOW", 1);
    XSetWMProtocols(display, wnd, WM_message, 2);

    XFreeColormap(display, attr.colormap);
    XSelectInput(display, wnd, ExposureMask | ButtonPressMask | KeyPressMask);

    XMapWindow(display, wnd);
    XFlush(display);

    mainGC = XCreateGC(display, wnd, 0, 0);

    //bg = XGetImage(display, wnd, 0, 0, 100, 100, AllPlanes, ZPixmap);
    int width, height;
    width = height = 256;
    unsigned char *image32 = (unsigned char *)malloc(width*height*4);
    bg = XCreateImage(display, CopyFromParent, vinfo.depth, ZPixmap, 0, image32, width, height, vinfo.depth, width*4);
    Pixmap p = XCreatePixmap(display, XDefaultRootWindow(display), width, height, vinfo.depth);
    XGCValues gcvalues;
    GC gc = XCreateGC(display, p, 0, &gcvalues);
    
    int x, y, idx;
    for(y = 0; y < width; y++) {
        for(x = 0; x < height; x++) {
            idx = (y * width + x) * 4;
            bg->data[idx + 0] = y;
            bg->data[idx + 1] = x;
            bg->data[idx + 2] = 255 - x;
            bg->data[idx + 3] = 0;
        }
    }

    XKeyEvent *key;
    while(run) {
        XNextEvent(display, &event);
        switch(event.type) {
            case Expose:
                // draw framebuffer
                XPutImage(display, wnd, mainGC, bg, 0, 0, width, height, width, height);
                break;

            case KeyPress:
                run = 0;
                break;

            case DestroyNotify:
                run = 0;
                break;

            case ClientMessage:
                {
                    if(event.xclient.message_type == WM_message[0]) {
                        if(event.xclient.data.l[0] == WM_message[1]) {
                            run = 0;
                        }
                    }
                }
            default:;
            }
    }
    XDestroyImage(bg);
    XDestroyWindow(display, wnd);
    XCloseDisplay(display);

    return 0;
}