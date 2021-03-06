# impy

A simple library for loading/saving images and animations, written in C.

Git repo: https://github.com/bcampbell/impy

License: GPL3

## Supported formats

* PNG (load, save)
* GIF (load, save, including animation)
* IFF (ILBM, PBM, load only. ANIM5 anims only)
* PCX (load only, no support for 2 or 16-colour images yet)
* BMP (load, save)
* JPEG (load only)
* Targa (load only)

## Installation

### Prerequisites

Requires cmake, libpng, zlib, giflib (v5+), libjpeg.

On Debian/Ubuntu:

```
$ sudo apt install cmake libjpeg-dev libpng-dev libgif-dev
```

### Compile & Install

Generic unix steps:

    $ git clone https://github.com/bcampbell/impy.git
    $ cd impy
    $ mkdir build
    $ cd build
    $ cmake ..
    $ make
    $ sudo make install

for a debug build:

    $ cmake -DCMAKE_BUILD_TYPE=Debug ..

Rough edges:

- Previous versions of cmake didn't correctly detect giflib 5, which impy
  requires. If you have an older cmake you can fudge `CMakeLists.txt`,
  changing the `5` in `find_package(GIF 5 REQUIRED)` to `4` instead
  (You still need giflib 5 installed).
- One of the examples, impyview, requires SDL2. No fancy cmake options yet, but you
  can just comment out the `impyview` bits of `examples/CMakeLists.txt` to skip it.

## API

Not yet. `impy.h` is the public interface, but it's subject to massive change.

## Example usage

    // load an image, output dimensions

    #include <stdio.h>
    #include <impy.h>

    int main(int argc, char* argv[])
    {
        if (argc<=1) {
            return 0;
        }

        im_img* img = im_img_load( argv[1], &err);
        if (!img) {
            fprintf(stderr,"load failed (err=%d)\n",err);
            return 1;
        }

        printf("%d x %d\n", im_img_w(img), im_img_h(img));

        im_img_free(img);
        return 0;
    }


