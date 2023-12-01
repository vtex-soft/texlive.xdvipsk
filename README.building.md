## Overview

Building the executable programs included in TeX Live involves usual steps of   
downloading the TeX Live sources, configuring, compiling, and installing.  
But there are some peculiarities, described here

    http://tug.org/texlive/build.html

To build VTeX adds, append TeX Live sources with ones provided by VTeX here,  
rerun configuration with `reautoconf` and use standard TeX Live building procedures.  
Overlapping file `Build/source/m4/kpse-pkgs.m4` should be merged eventually.

### Utilities / packages added

`Build/source/texk/xdvipsk`

`Build/source/libs/libjpeg`

> Used in `Build/source/texk/xdvipsk`

`Build/source/libs/libtiff`

> Used in `Build/source/texk/xdvipsk`
>
> Package `libtiff` taken from **LibTIFF** project at `http://www.simplesystems.org/libtiff/`.  
> Currently used `libtiff` version is 4.0.8
>
> Repository mapped using instructions:
>
>```
>    cd Build/source/libs
>    export CVSROOT=:pserver:cvsanon@cvs.maptools.org:/cvs/maptools/cvsroot
>    cvs login # (use empty password)
>    cvs checkout libtiff
>```
>
> `Build/source/libs/libtiff/libtiff` renamed to `libtiff-src` and `automake` files adapted to `texlive` building environment.

### Updating configuration files

```
    cd Build/source
    ./reautoconf
```

### Build examples

```
    mkdir Work
    cd Build/source/Work

    ../configure --disable-all-pkgs --without-x --disable-xetex --disable-xindy --enable-xdvipsk -C CFLAGS=-g CXXFLAGS=-g
    make

    ../configure --host=x86_64-w64-mingw32 --build=x86_64-apple-darwin --disable-all-pkgs --without-x --disable-xetex --disable-xindy \
    --enable-xdvipsk -C CFLAGS=-g CXXFLAGS=-g 
```

### Build in `Windows`

Visual Studio 2019 project files are provided in `texk/xdvipsk/windows`.

Due `github` restrictions on file size some libs are tar gzip'ed.
So, before building unzip folowwing files:

```
    texk/xdvipsk/thirdparty/libmupdf/win32/libmupdfd.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win32/libmupdf.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win64/libmupdfd.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win32/libthirdparty.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win64/libthirdpartyd.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win64/libmupdf.lib.tgz
    texk/xdvipsk/thirdparty/libmupdf/win64/libthirdparty.lib.tgz
```
