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

`Build/utils/make2unc`

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

    ../configure --disable-all-pkgs --without-x --disable-xetex --disable-xindy --enable-make2unc -C CFLAGS=-g CXXFLAGS=-g
    make
    
    LDFLAGS="-fno-lto -fno-use-linker-plugin -static-libgcc -static-libstdc++" ../configure \
    --host=x86_64-w64-mingw32 --build=x86_64-apple-darwin --disable-all-pkgs --without-x \
    --disable-xetex --disable-xindy --enable-make2unc --enable-missing -C CFLAGS=-g CXXFLAGS=-g
    
```

