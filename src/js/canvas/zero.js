
var _z = {};

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
