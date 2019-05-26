#pragma once
// Most code was taken from tinyptc library
// https://sourceforge.net/projects/tinyptc/

void zero_open(char *title, int w, int h, int d, int s);
int zero_update(void *fb, void *pal);

#define MAKE_RGB(r,g,b) (((r)<<16) | ((g)<<8) | (b))

// IMPLEMENTATION
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

HDC hdc;
static int width, height, depth, scale;
static HWND wnd;
static WNDCLASS wc;
static void *active_fb = 0;
static void *active_pal = 0;
static BITMAPINFO *bitmap_header;
static char bitmapbuffer[sizeof(BITMAPINFO) + 16 + 256 * 4];

#ifdef FPS_COUNTER
#include <stdio.h>
static LARGE_INTEGER frequency;
static LARGE_INTEGER t1, t2;
static double elapsedTime;
char window_title_fps[50];
#endif

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int result = 0;
  switch (message)
  {
    case WM_PAINT:
      if (active_fb != 0) {
        StretchDIBits(hdc,
          0, 0,
          width * scale,
          height * scale,
          0, 0,
          width, height,
          active_fb, bitmap_header,
          DIB_RGB_COLORS,
          SRCCOPY);
        ValidateRect(wnd, NULL);
      }
      break;
    case WM_CLOSE:
      active_fb = 0;
      ReleaseDC(wnd, hdc);
      DestroyWindow(wnd);
      ExitProcess(0);
      break;
    default:
      result = DefWindowProc(hWnd, message, wParam, lParam);
      break;
  }
  return result;
}

void zero_open(char *title, int w, int h, int d, int s)
{
  RECT rect;
  scale = s;

  wc.style = CS_OWNDC | CS_VREDRAW | CS_HREDRAW;
  wc.lpfnWndProc = WndProc;
  wc.cbClsExtra = 0;
  wc.cbWndExtra = 0;

  // app icon
  // wc.hInstance = GetModuleHandle(0);
  // wc.hIcon = LoadIcon(wc.hInstance, __PTC_ICON__);

  // no icon
  wc.hInstance = 0;
  wc.hIcon = NULL;

  wc.hCursor = LoadCursor(0, IDC_ARROW);
  wc.hbrBackground = NULL;
  wc.lpszMenuName = NULL;
  wc.lpszClassName = title;
  RegisterClass(&wc);

  rect.left = 0;
  rect.top = 0;
  rect.right = w * s;
  rect.bottom = h * s;

  AdjustWindowRect(&rect, WS_POPUP|WS_SYSMENU|WS_CAPTION, 0);
  rect.right -= rect.left;
  rect.bottom -= rect.top;

  width = w;
  height = h;
  depth = d == 1 ? 1 : 4;

  int posx = (GetSystemMetrics(SM_CXSCREEN) - rect.right) >> 1;
  int posy = (GetSystemMetrics(SM_CYSCREEN) - rect.bottom) >> 1;

  wnd = CreateWindowEx(0, title, title, WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_THICKFRAME, posx, posy, rect.right, rect.bottom, 0, 0, 0, 0);
  ShowWindow(wnd, SW_NORMAL);

  for (int cc = 0; cc < sizeof(BITMAPINFOHEADER)+ 16 + 256 * 4; cc++) bitmapbuffer[cc] = 0;
  bitmap_header = (BITMAPINFO *)&bitmapbuffer;
  bitmap_header->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bitmap_header->bmiHeader.biPlanes = 1;
  bitmap_header->bmiHeader.biBitCount = depth * 8;
  bitmap_header->bmiHeader.biCompression = depth == 1 ? BI_RGB : BI_BITFIELDS;
  bitmap_header->bmiHeader.biWidth = width;
  bitmap_header->bmiHeader.biHeight = -height;
  if (depth != 1) {
    ((unsigned long *)bitmap_header->bmiColors)[0] = 0x00FF0000;
    ((unsigned long *)bitmap_header->bmiColors)[1] = 0x0000FF00;
    ((unsigned long *)bitmap_header->bmiColors)[2] = 0x000000FF;
  }
  hdc = GetDC(wnd);

#ifdef FPS_COUNTER
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&t1);
#endif
}

int zero_update(void *fb, void *pal)
{
  MSG message;
  active_fb = fb;
  active_pal = pal ? pal : active_pal;

  if (pal) {
    RGBQUAD* palette = &bitmap_header->bmiColors[0];
    RGBQUAD rgb = {0, 0, 0, 0};
    BYTE *pal8 = (BYTE *)pal;
    for (int i = 0; i < 256; i++)
    {
      rgb.rgbRed   = pal8[i * 3 + 0];
      rgb.rgbGreen = pal8[i * 3 + 1];
      rgb.rgbBlue  = pal8[i * 3 + 2];
      palette[i] = rgb;
    }
  }

  InvalidateRect(wnd, NULL, TRUE);
  SendMessage(wnd, WM_PAINT, 0, 0);
  while (PeekMessage(&message, wnd, 0, 0, PM_REMOVE))
  {
    TranslateMessage(&message);
    DispatchMessage(&message);
  }
  Sleep(0);

#ifdef FPS_COUNTER
  QueryPerformanceCounter(&t2);
  elapsedTime = (float)(t2.QuadPart-t1.QuadPart)/frequency.QuadPart;
  sprintf(window_title_fps, "%.1ffps", 1.0/elapsedTime);
  SetWindowTextA(wnd, window_title_fps);
  t1 = t2;
#endif

  return 1;
}

extern void main();
void WinMainCRTStartup(void) {
    main();
}
