# Overview

## *xdvipsk*: extended dvips (TeXLive 2024)

It has four base extensions:
* one allows flexible inclusion of bitmap images
* another extension solves a quite long-standing task -- adds `OpenType` font support
to `dvips`
* accepts font map `\special` commands with prefixes `mapfile` and `mapline` 
* adds `lua` callbacks for processing `dvi specials`

*xdvipsk* goes `LuaTeX` way in `OpenType` font management:  
> works on `DVI` files compiled by `LuaTeX` and expects to find the necessary `Unicode` map files, 
> obtained as by-products of the compilation. The provision of the map files is ensured
> by a special `(Lua)LaTeX` package `luafonts`.

The extended `dvips` now accepts `BMP`, `PCX`, `TIFF`, `JPEG` and `PNG` formats and is
able to perform the same actions as with `EPS` images:  
- scaling, rotating, trim, viewport

The `xdvispk.def` (extended `dvips.def`) driver for graphics package does not yet
implement the operations of clipping, trimming and viewport.

`xdvipsk` needs information about `OpenType` fonts used in `DVI` files in the form
of `Unicode` map files. These are produced by `dvilualatex` using a special `(Lua)LaTeX`
package `luafonts` (see below more details about the
workflow). 

One particular map type (`.tounicode`) is used for postprocessing the distilled
`PDF` to enable correct unicode-based search.  For this task, a utility
`make2unc` is also provided.

### Bitmap inclusion

The extension for bitmap images does not require changes to the user-level
syntax. The `LaTeX` command `\includegraphics` should
work as described in the documentation of packages `graphics` and `graphicx`.  
That is, after inclusion in the
preamble of either
```latex
    \usepackage[*driver*]{graphics}
or
    \usepackage[*driver*]{graphicx}
```
where the file `*driver*.def` contains all the necessary
declarations and is registered in `graphics.sty`.

As `xdvipsk` accepts images in formats `BMP`, `JPEG`, `PCX`, `PNG`
and `TIFF`, they should all be declared in the form of graphic inclusion rules
in the driver file, most likely `xdvipsk.def`:
```latex
    \@namedef{Gin@rule@.tif}#1{{bmp}{.tif.bb}{#1}}
    \@namedef{Gin@rule@.tiff}#1{{bmp}{.tiff.bb}{#1}}
    \@namedef{Gin@rule@.jpeg}#1{{bmp}{.jpeg.bb}{#1}}
    \@namedef{Gin@rule@.jpg}#1{{bmp}{.jpg.bb}{#1}}
    \@namedef{Gin@rule@.png}#1{{bmp}{.png.bb}{#1}}
```

Several things for style and package authors to know:

- bitmap image file names are included in `DVI` files inside arguments of
  `\special` commands with prefix `em: graph` (the name has
  roots in the time of the EmTeX distribution)

- bitmaps can be of different color models: `BW`, `gray`, `RGB`,
  `CMYK`, indexed `RGB`

- the program ignores the content of `\special` commands with unknown
  prefixes

- the program accepts `\special` commands with prefixes: `mapfile` and `mapline`.
```
    *mapfile* is used for reading a font map file consisting of one or more font map lines. The name
    of the map file is given together with an optional leading modifier character (+). There is a
    companion special type *mapline* that allows to scan single map lines; its map line argument has the
    same syntax as the map lines from a map file. Both specials can be used concurrently.
    The operation mode is selected by an optional modifier character (+) in front. 
    This modifier defines how the individual map lines are going to be handled,
    and how a collision between an already registered map entry and a newer one is resolved; either ignoring
    a later entry with a warning in case modifier charcter is given, 
    or replacing an existing entry in case no modifier character is given. 
    Here are examples: 

    \special{mapfile: +myfont.map}
    \special{mapline: +ptmri8r Times-Italic <8r.enc <ptmri8a.pfb}

     or 

    \special{mapfile: myfont.map}
    \special{mapline: ptmri8r Times-Italic <8r.enc <ptmri8a.pfb}
```

- for more precise image positioning `xdvipsk` inserts the PostScript
  `HiResBoundingBox` parameter

### `Opentype` font support

For correct work of `xdvipsk`, a number of files should be prepared.

`texcid.pro`

> PostScript header file used for inclusion of
> `OpenType` fonts in `PostScript` files.  It is an analogue of
> `texps.pro` that is used in case of Type 1 fonts.

`luafonts.sty` 

> A `LaTeX` package, which is just an interface
> to Lua code generating two maps necessary for `xdvipsk`. 
> It is loaded like any other `\LaTeX` package: `\usepackage{luafonts}` 

To get the general picture, how process is organized, let us present the
description of workflow involving the use of `xdvipsk`.

### Running `xdvipsk`

1. Run `dvilualatex *article*.tex` where file
  `*article*.tex` includes the line `\usepackage{luafonts}`

    - Input:

        ```
            *article*.tex
            tex/luatex/luafonts/luafonts.sty
            tex/luatex/luafonts/luafonts.lua
            ...
        ```

    - Output:

        ```
            *article*.dvi
            .xdvipsk/*psname*.encodings.map
            ...
            .xdvipsk/*article*.opentype.map
        ```

2. Run `xdvipsk *article*.dvi`

    - Input: 

        ```
            *article*.dvi
            .xdvipsk/*psname*.encodings.map
            ...
            .xdvipsk/*article*.opentype.map
            texmf-dist/dvips/base/texcid.pro
            ...
        ```

    - Output:

        ```
            *article*.ps
            ...
            .xdvipsk/*article*-cid*num*.tounicode
            ...
        ```
    
        where `*num*` is a font index in the `DVI` file and is  used here to have distinct file names.

3. Convert the `PostScript` file to `PDF` (using Ghostscript, Acrobat or any other tool which supports pdfmark command):
   
    - Input:

        ```
            *article*.ps
        ```

    - Output:

        ```
            *article*.pdf
        ```
    
4. Add `TOUNICODE` cmaps to the `PDF` file using `make2unc` utility:

    - Input:

        ```
            *article*.pdf
            ...
            .xdvipsk/*article*-cid*num*.tounicode
            ...
        ```
    
    - Output:
    
        ```
            *article*.pdf (searchable)
        ```
    
### Map files 

`OpenType` font information is extracted and saved for later use in map files.
This information is in the form as used in the `luaotfload` package, except the
changes described below.

#### `Opentype` map

The map `*article*.opentype.map` generated by the package is analogous to
`psfonts.map` and contains information about `OpenType` fonts used in a
particular article.  
The map is a list of lines with tab-separated fields
```
    TFMNAME PSNAME TEXFONTNAME >FILENAME
```

- `TFMNAME`

    is a metric file name as written in the `DVI` file by `LuaTeX`, like

    ```
    FandolFang-Regular
    FandolFang-Regular:mode=node;script=latn;language=DFLT;+tlig;
    TeXGyreAdventor
    TeXGyreAdventor/B
    TeXGyreAdventor/BI
    TeXGyreAdventor/I
    TeXGyreAdventor:mode=node;script=latn;language=DFLT;+pnum;+onum;
    [lmroman10-bold]:+tlig;
    [lmroman10-italic]:+tlig;
    [lmroman10-regular]:+tlig;
    [Avenir.ttc](11):mode=node;script=DFLT;language=dflt;
    ```

- `PSNAME` 

    is a `PostScript` font name from `luaotfload` Lua tables, like

    ```
    FandolFang-Regular
    TeXGyreAdventor-Regular
    TeXGyreAdventor-Bold
    TeXGyreAdventor-BoldItalic
    TeXGyreAdventor-Italic
    TeXGyreAdventor-Regular
    LMRoman10-Bold
    LMRoman10-Italic
    LMRoman10-Regular
    ```

- `TEXFONTNAME`

    is a fullname from 'luaotfload' Lua tables, like

    ```
    FandolFang R
    TeXGyreAdventor-Regular
    TeXGyreAdventor-Bold
    TeXGyreAdventor-BoldItalic
    TeXGyreAdventor-Italic
    TeXGyreAdventor-Regular
    LMRoman10-Bold
    LMRoman10-Italic
    LMRoman10-Regular
    ```

- `FILENAME`

    is a font file name also found in `luaotfload` Lua tables and
    modified so that the file-path prefix,
    corresponding to the actual TeX-tree branch used, is replaced by variable
    `$SELFAUTOPARENT` and `$SELFAUTOGRANDPARENT`:

    ```
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/fandol/FandolFang-Regular.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/tex-gyre/texgyreadventor-regular.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/tex-gyre/texgyreadventor-bold.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/tex-gyre/texgyreadventor-bolditalic.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/tex-gyre/texgyreadventor-italic.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/tex-gyre/texgyreadventor-regular.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/lm/lmroman10-bold.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/lm/lmroman10-italic.otf
    $SELFAUTOPARENT/texmf-dist/fonts/opentype/public/lm/lmroman10-regular.otf

    ```

The current version of `luaotfload` operates with only one writable cache, which
incorporates file paths specific to an OS, which is inconvenient in case of production environment 
which contains different operating systems and we wanted to organize things in a more flexible way.

#### Encodings maps

Another type of maps generated by `luafonts` stores information about
characters.  
It consists of five columns
```
    <tex charcode>,<OT font glyph index>,<unicode equivalent>,<glyph width>,<glyph height>

    59964,707,00AF,376832,365690.88
    59965,708,00AF,376832,414842.88
    59966,709,00200331,376832,0
    59967,710,0304,0,573440
    59968,711,02DA,569507.84,408944.64
    59969,712,0020030A0301,569507.84,543948.8
    59970,713,0020030A0301,569507.84,586547.2
    59971,714,030A,0,610140.16
    59972,715,02DC,376832,389283.84
```

#### `TOUNICODE` cmaps

`xdvipsk` saves `TOUNICODE` cmaps needed for `PDF` postprocessing, done by a
utility `make2unc`, which incorporate the cmaps into the `PDF` files.  The file
name form is `*article*-cid*num*.tounicode`, where `*num*` is a font index in
the `DVI` file and is used here to have distinct file names for distinct fonts.

### `Lua` callbacks (experimental stuff)

`xdvipsk` reads `lua` script file `xdvipsk.lua` if it is found by `kpse` and looks
for these lua functions:

- `prescan_specials_callback(special, table)`
> `special` - original special data, `table` with keys: `hh`, `vv`, `pagenum`

- `scan_specials_callback(special, table)`
> `special` - original special data, `table` with keys: `hh`, `vv`, `pagenum`

- `after_prescan_callback(table)`
> `table` with keys: `hpapersize`, `vpapersize`, `hoff`, `voff`, `actualdpi`, 
>                    `vactualdpi`, `num`, `den`, `mag`, `totalpages`

- `after_drawchar_callback(table)`
> `table` with keys: `charcode`, `cid`, `pixelwidth`, `rhh`, `rvv`, `dir`, `lastfont`
>                    `tounicode`

- `after_drawrule_callback(table)`
> `table` with keys: `hh`, `vv`, `dir`, `rw`, `rh`

- `process_stack_callback(table)`
> `table` with keys: `cmd`, `hh`, `vv`, `dir`

Two functions: `prescan_specials_callback` and `scan_specials_callback`  
are used for processing `dvi specials` in prescan and scan steps accordingly  
before be processed by `xdvipsk` native mechanism. 

Return values defines processing behaviour:

  - non empty string defines modified `dvi special` to be processed;
  - true value means processing original `dvi special` in default way;
  - empty string or false value means skipping `dvi special`.

Demo script file: `testdata/luacallbacks/xdvipsk.lua`

Default `lua` script file can be changed by command line argument `-lua`
or command `luascript` in `xdvipsk` config files. 

### External libraries 

The main development and building of executables is
moved on a *MS Windows* workstation to the *Visual Studio 2019 IDE*. 
In parallel, we build the code on two more architectures: *Linux* and *macOS*.
For these, we are quite close to the TeX Live build environment except for
extra libraries: `libjpeg`, `libtiff` for `xdvipsk`,
and `libmupdf` for new utility `make2unc`.
Other needed libraries are taken from TeX Live distributions.
Latest `Windows` binaries are cross compiled with `mingw`.

The current `xdvipsk` version is based on `dvips 2024`, `web2c + kpathsea 6.4.0`, `TeXLive 2024`.
