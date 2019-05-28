## Linux framebuffer

```
void zero_open(char *device, int *w, int *h, int *d, int s, int *line);
int  zero_update(void *fb, void *pal);
void zero_close(void);
```

Since the terminal framebuffer has a fixed resolution (the whole screen),
parameters to `zero_open` are of output type: instead of asking for a specific 
resolution, the function returns current terminal framebuffer resolution and depth. 

It adds an extra output parameter `line`, which is the number of bytes you have
to jump from one row to the next one (`width * depth` doesn't work here). See the *rgb* as an example of this.