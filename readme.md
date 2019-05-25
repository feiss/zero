# ZERO libraries

Zero aims to be the solution to this common question...

> "How can I simply put a pixel on the screen?"

...by gathering different implementations, depending on the platform, language
or technology you want to use, using only two function calls: `zero_open()` and `zero_update()`.


## Current implementations

Name | Description | Status
-----|-------------|-------
png  | Writes frames to PNG files | working
wingdi | Windows GDI | working
wingl | Windows OpenGL | pending


## API

### `zero_open (target, width, height, depth, scale)`

#### target
Depending on the implementation used, `target` has different meanings.
For example, in Javascript it means a DOM selector;
in windows GDI it means the title of the window to create;
in png means a file name, etc.

#### width, height

Dimensions of the image, in pixels

#### depth

**1** for indexed images (that need a palette)
**3** for RGB images
**4** for RGBA (not supported in all implementations)

#### scale

Multiplier of `width` and `height`, which affects the dimensions of the output (window, canvas, image file, ...).
So if you set `scale` to 2, the window/canvas/image will be scaled by 2.


### `zero_update (framebuffer, palette)`

#### framebuffer

Pointer to the image buffer. No checks are done about the format of the buffer content.

#### palette

If `depth` is set to **1**, a palette (array of R,G,B items) must be passed.
Otherwise, this value is ignored.
