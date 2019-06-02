# ZERO libraries

Zero aims to be the solution to this common question...

> "How can I simply put a pixel on the screen?"

...by gathering different implementations, depending on the platform, language
or technology you want to use, using a common and extremely simple [API](https://github.com/feiss/zero#api).

It can be used as a learning tool for children, a testbed to implement
classic or new CG routines, experiment, make apps or games, for example.

It is **deeply** inspired by the old Demoscene library **TinyPTC** by Gaffer.


# Current implementations

C |    |    status
-----|-------------|-------
[PNG](https://github.com/feiss/zero/tree/master/src/c/png)  | Writes frames to PNG files | working
[wingdi](https://github.com/feiss/zero/tree/master/src/c/wingdi) | Windows GDI | working
wingl | Windows OpenGL | planned
[linuxx11](https://github.com/feiss/zero/tree/master/src/c/linuxx11) | Linux X11 | working
[linuxgl](https://github.com/feiss/zero/tree/master/src/c/linuxgl) | Linux OpenGL | working
[linuxfb](https://github.com/feiss/zero/tree/master/src/c/linuxfb) | Linux Framebuffer | working

Javascript  |   | status
-----|-------------|-------
[canvas](https://github.com/feiss/zero/tree/master/src/js/canvas) | HTML canvas | working
WebGL | HTML WebGL | planned


## Contributions

PRs are very very welcomed. It would be really great to convert this in a bible of how to put pixels in all architectures and languages, and that's something near impossible for a single person (at least for myself).

Corrections and suggestions are very welcomed too. I'm not an expert in this field and I already assume I'm making lots of mistakes.

Anybody trying to add an implementation should try to adhere to the [KISS](https://en.wikipedia.org/wiki/KISS_principle) principle. Specially, I'd like to:

* Keep the external dependencies to a minimum, or none. The ideal usage of
the library should be a single include.

* Do not make it too generic, full of options and supporting many pixel formats.
We are only targeting typical RGB(A) and Indexed color modes, 8 bits per component.



## API

* `zero_open (target, width, height, depth, scale)`
* `zero_update (framebuffer, palette)`
* `zero_events(handler)`
* `zero_close()`

### `zero_open (target, width, height, depth, scale)`

Opens or initializes a window with specific properties.

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

Sends the frame data (and optional color palette) to the window.

#### framebuffer

Pointer to the image buffer. No checks are done about the format of the buffer content.

#### palette

If `depth` is set to **1**, a palette (array of R,G,B items) must be passed.
Otherwise, this value is ignored.

### `zero_close ()`

Closes the window, freeing resources.

### `zero_events(handler(type, param1, param2) )`

Registers a function callback that will be called whenever an event occur. The `handler` must be a function that takes 3 parameters:

* `type` a number with the event type code. See next table for a reference of the constant codes you can use.
* `param1` and `param2` are two numeric parameters which meaning depends on the even type:

type code | param1 meaning | param2 meaning
----------|----------|-----------
RESIZE  | new window width   | height 
KEY_DOWN | key code   | modifiers bitfield
KEY_UP |  key code  |  modifiers bitfield 
MOUSE_DOWN |  button  | modifiers bitfield  
MOUSE_UP |  button  | modifiers bitfield 
MOUSE_MOVE |  x  | y 
MOUSE_ENTER |  none   |  none
MOUSE_LEAVE |  none   |  none 


# Example in pseudocode

This is a basic example of a whole functional drawing program in a 500x500 window/canvas for drawing with mouse, and quits with any key. It's in pseudocode because it could be written in any of the languages supported by the library, just adapting it to the idiosyncrasies of the language.

```pascal
include 'zero'

run = true
drawing = false
framebuffer = array of 500 x 500 x 4 bytes

procedure PROCESS_EVENTS(type, param1, param2)

  if type is KEY_DOWN then
    run = false
  
  else if type is MOUSE_DOWN then
    drawing = true
    
  else if type is MOUSE_UP then
    drawing = false
    
  else if event_type is MOUSE_MOVE and drawing then
    // calculate position in framebuffer, mouse x = param1, mouse y = param2
    idx = (param2 * 500 + param1) * 4
    // paint pixel with a gray rgb (100, 100, 100)
    framebuffer[ idx + 0 ] = 100
    framebuffer[ idx + 1 ] = 100
    framebuffer[ idx + 2 ] = 100
  
  end if
  
end procedure


procedure MAIN

  zero_open("Drawing program", 500, 500, 4, 1)

  zero_events(PROCESS_EVENTS)

  while run is true
    zero_update(framebuffer)
  end while

  zero_close()

end procedure
```
