# ZERO api

Zero API is an ultra-simple, basic and old-school graphics API, aimed to be
an universal, multiplatform and multilanguage way of draw to a window or screen.

It is only comprised in three functions (pseudocode):

*  createFrameBuffer (width, height, depth) : fb
*  drawFrameBuffer (fb, dest, [pal])

### createFrameBuffer

Inputs:

* `width` and `height`: dimensions of the frame buffer.
* `depth`: Bits per pixel of the buffer. It can have on of these values:
  * 8 for [indexed colored](https://en.wikipedia.org/wiki/Indexed_color) pixels.
  * 24 for RGB pixels (8 bits per color component, no transparency).

Outputs:

* A memory buffer of `width * height * depth` Bytes set to zero.


### drawFrameBuffer

Inputs:

* `fb`: The frame buffer to draw in the screen
* `dest`: The destiny. The window, file, draw context, etc. where to draw.
* `pal`: The palette to use (optional).

When the `depth` of the frame buffer is `8`, a palette is needed to paint
the buffer in the screen. Here a buffer of `n * 3` elements (n colors * (Red, Green, Blue) values).

Palette format: RGB RGB RGB RGB ...

