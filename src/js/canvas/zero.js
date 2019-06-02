
var _z = {};

const RESIZE = 1;
const KEY_DOWN = 2;
const KEY_UP = 3;
const MOUSE_DOWN = 4;
const MOUSE_UP = 5;
const MOUSE_MOVE = 6;
const MOUSE_LEAVE = 7;
const MOUSE_ENTER = 8;

const SHIFT = 1;
const CONTROL = 2;
const ALT = 4;
const SUPER = 8;

const MOUSE_LEFT = 0;
const MOUSE_RIGHT = 1;
const MOUSE_MIDDLE = 2;

function zero_open(el, w, h, d, s) {
  var canvas = document.createElement('canvas');
  var dirtyCanvas = document.createElement('canvas');
  document.querySelector(el).appendChild(canvas);

  canvas.width = w * s;
  canvas.height = h * s;
  _z.ctx = canvas.getContext('2d');
  _z.ctx.imageSmoothingEnabled = false;

  dirtyCanvas.width = w;
  dirtyCanvas.height = h;
  _z.dirtyCtx = dirtyCanvas.getContext('2d');
  _z.dirtyCtx.imageSmoothingEnabled = false;

  _z.dirtyImdata = _z.ctx.createImageData(w, h);

  if (d == 1) {
    _z.dirtyImdataIndexed = _z.ctx.createImageData(w / 2, h / 2);
    return _z.dirtyImdataIndexed;
  }

  return _z.dirtyImdata;
}

function zero_update(fb, pal) {
  const canvas = _z.ctx.canvas;
  var idx;
  if (pal) {
    var data = _z.dirtyImdata.data;
    for (var i = 0, j = 0; i < data.length; i += 4, j++) {
      idx = fb.data[j];
      data[i + 0] = pal[idx * 3 + 0];
      data[i + 1] = pal[idx * 3 + 1];
      data[i + 2] = pal[idx * 3 + 2];
      data[i + 3] = 255;
    };
    _z.dirtyCtx.putImageData(_z.dirtyImdata, 0, 0);
  } else {
    _z.dirtyCtx.putImageData(fb, 0, 0);
  }
  _z.ctx.drawImage(_z.dirtyCtx.canvas, 0, 0, canvas.width, canvas.height);
}

function zero_close() {}

function zero_events(handler){
  if (_z.handler) {
    _z.ctx.canvas.removeEventListener('mousedown', __mouse_handler);
    _z.ctx.canvas.removeEventListener('mouseup', __mouse_handler);
    _z.ctx.canvas.removeEventListener('mousemove', __mouse_handler);
    document.body.removeEventListener('keydown', __key_handler);
    document.body.removeEventListener('keyup', __key_handler);
    window.removeEventListener('resize', __resize_handler);
  }

  _z.handler = handler;
  _z.ctx.canvas.addEventListener('mousedown', __mouse_handler);
  _z.ctx.canvas.addEventListener('mouseup', __mouse_handler);
  _z.ctx.canvas.addEventListener('mousemove', __mouse_handler);
  document.body.addEventListener('keydown', __key_handler);
  document.body.addEventListener('keyup', __key_handler);
  window.addEventListener('resize', __resize_handler);
}

function __mouse_handler (evt) {
  const MAP_BUTTON = [0, 2, 1];
  var type = {'mousedown': MOUSE_DOWN, 'mouseup': MOUSE_UP, 'mousemove': MOUSE_MOVE}[evt.type];
  if (type === undefined) return;
  let param1 = type === MOUSE_MOVE ? evt.offsetX : MAP_BUTTON[evt.button];
  let param2 = type === MOUSE_MOVE ? evt.offsetY : MAP_BUTTON[evt.button];
  _z.handler(type, param1, param2);
}

function __key_handler (evt) {
  const modifiers = evt.metaKey << 3 | evt.altKey << 2 | evt.controlKey << 1 | evt.shiftKey;
  _z.handler(evt.type === 'keydown' ? KEY_DOWN : KEY_UP, evt.keyCode, modifiers);
}

function __resize_handler (evt) {
  _z.handler(RESIZE, window.innerWidth, window.innerHeight);
}

