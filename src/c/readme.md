
# C implementation

## Palette

`int zero_update(framebuffer, palette);`

To avoid unnecessary updates of the palette on each frame, if `palette` is NULL
the latest palette submitted will be used. *But remember to send one on the first frame!*

## Types


#### Indexed mode
```
unsigned char framebuffer [W * H];
unsigned char palette [256 * 3]; // RGBRGBRGB...
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
