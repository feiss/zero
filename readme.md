# ZERO libraries

Zero aims to be the solution to this common question...

> "How can I simply put a pixel on the screen?"

...by gathering different implementations, depending on the platform, language
or technology you want to use, using only two function calls: `zero_open()` and `zero_update()`.

It can be used as a learning tool for children, a testbed to implement
classic or new CG routines, experiment, make apps or games, for example.

It is **deeply** inspired by the old Demoscene library **TinyPTC** by Gaffer.


# Current implementations

Anybody trying to add an implementation should try to adhere to the [KISS](https://en.wikipedia.org/wiki/KISS_principle) principle:

* Keep the external dependencies to a minimum, or none. The ideal usage of
the library should be a single include.

* Do not make it too generic, full of options and supporting many pixel formats.
We are only targeting typical RGB(A) and Indexed color modes, 8 bits per component.


C |    |    status
-----|-------------|-------
[PNG](https://github.com/feiss/zero/tree/master/src/c/png)  | Writes frames to PNG files | working
[wingdi](https://github.com/feiss/zero/tree/master/src/c/wingdi) | Windows GDI | working
wingl | Windows OpenGL | planned

Javascript  |   | status
-----|-------------|-------
[canvas](https://github.com/feiss/zero/tree/master/src/js/canvas) | HTML canvas | working
WebGL | HTML WebGL | planned


## API

### `zero_open (target, width, height, depth, scale)`

#### target
Depending on the implementation used, `target` has different meanings.
For example, in Javascript it means a DOM selector;
in windows GDI it means the title of the window to create;
in png means a file name, etc.

#### width, height

Dimensions of the image, in pixels.

#### depth

- **1** for indexed images (that need a palette)
- **3** for RGB images
- **4** for RGBA (not supported in all implementations)

#### scale

Multiplier of `width` and `height`, which affects the dimensions of the output (window, canvas, image file, ...).
So if you set `scale` to 2, the window/canvas/image size will be the double of your frame size.



### `zero_update (framebuffer, palette)`

#### framebuffer

Pointer to the image buffer. No checks are done about the format of the buffer content.

#### palette

If `depth` is set to **1**, a palette (array of R,G,B items) must be passed.
Otherwise, this value is ignored.
