## Overview

Building the executable programs included in TeX Live involves usual steps of   
downloading the TeX Live sources, configuring, compiling, and installing.  
But there are some peculiarities, described here

    http://tug.org/texlive/build.html

To build VTeX adds append TeX Live sources with provided VTeX here,  
rerun configuration with `reautoconf` and use standard TeX Live building procedures.  
Overlapping file `Build/source/m4/kpse-pkgs.m4` should be merged eventually.

### Utilities / packages added

`Build/source/texk/xdvipsk`

`Build/utils/make2unc`

`Build/source/libs/libjpeg`

> Used in `Build/source/texk/xdvipsk`

`Build/source/libs/libtiff`

> Used in `Build/source/texk/xdvipsk`
>
> Package `libtiff` taken from **LibTIFF** project at `http://www.simplesystems.org/libtiff/`.  
> Currently used `libtiff` version 4.0.8
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

`Build/source/libs/libmupdf`

> Used in `Build/utils/make2unc`
>
> Package `libmupdf` of version 1.11 taken from `https://mupdf.com/downloads/mupdf-1.11-source.tar.gz`  
> Original `mupdf` source tree copied to `Build/source/libs/libmupdf/libmupdf-src`  
> Automake files for `texlive` building environment implemented.  

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

    ../configure --disable-all-pkgs --without-x --disable-xetex --disable-xindy --enable-make2unc -C CFLAGS=-g CXXFLAGS=-g
    make
```

