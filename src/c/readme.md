## C types


#### Indexed mode
```
unsigned char framebuffer [W * H];
unsigned char palette [NUM_COLORS * 3]; // RGBRGBRGB...
```
#### RGB mode
```
unsigned char framebuffer [W * H * 3]; // RGB RGB RGB...
```

#### RGBA mode
```
unsigned char framebuffer [W * H * 4]; // RGBA RGBA RGBA...
```
or
```
unsigned int framebuffer [W * H]; // RGBA RGBA RGBA...
```
