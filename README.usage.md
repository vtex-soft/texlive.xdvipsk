Usage: `xdvipsk [OPTION]... FILENAME[.dvi]`

Convert `DVI` input files to `PostScript`.  
See <http://tug.org/dvips/> for the `dvips` full manual and other information.  
See <https://github.com/vtex-soft/texlive.xdvipsk> for the specific `xdvipsk` 
information and source.

Options:
```
-a*  Conserve memory, not time       -A   Print only odd (TeX) pages
-b # Page copies, for posters e.g.   -B   Print only even (TeX) pages
-bitmapfontenc [on,off,strict] control bitmap font encoding
-c # Uncollated copies               -C # Collated copies
-d # Debugging                       -D # Resolution
-e # Maxdrift value                  -E*  Try to create EPSF
-f*  Run as filter                   -F*  Send control-D at end
-g*  write log file                  -G*  Shift low chars to higher pos.
-h f Add header file                 -H*  Turbo mode for PS graphics
-i*  Separate file per section       -I p Set resize mode for emTeX graphics
-j*  Download T1 fonts partially     -J*  Download OpenType fonts partially
-k*  Print crop marks                -K*  Pull comments from inclusions
-l # Last page                       -L*  Last special papersize wins
-landscaperotate*  Allow landscape to print rotated on portrait papersizes
-lua s Lua script file name
-m*  Manual feed                     -M*  Don't make fonts
-mode s Metafont device name
-n # Maximum number of pages         -N*  No structured comments
-noomega  Disable Omega extensions
-noptex   Disable pTeX extensions
-noluatex   Disable LuaTeX extensions
-noToUnicode   Disable ToUnicode CMap file generation for OpenType fonts
-o f Output file                     -O c Set/change paper offset
-p # First page                      -P s Load config.$s
-pp l Print only pages listed
-q*  Run quietly                     -Q*  Skip VTeX private specials
-r*  Reverse order of pages          -R*  Run securely
-s*  Enclose output in save/restore  -S # Max section size in pages
-t s Paper format                    -T c Specify desired page size
-title s Title in comment
-u s PS mapfile                      -U*  Disable string param trick
-v   Print version number and quit   -V*  Send downloadable PS fonts as PK
                                     -W*  Extended search for emTeX graphics
-x # Override dvi magnification      -X # Horizontal resolution
-y # Multiply by dvi magnification   -Y # Vertical resolution
-z*  Hyper PS                        -Z*  Compress bitmap fonts
    # = number   f = file   s = string  * = suffix, `0' to turn off
    c = comma-separated dimension pair (e.g., 3.2in,-32.1cm)
    l = comma-separated list of page ranges (e.g., 1-4,7-9)
    p = comma-separated pixel-form:filter pairs (e.g. RGB:t,CMYK:r), 
        `0' to turn off
```

Email `dvips` related bug reports to <tex-k@mail.tug.org>.  
Email `xdvipsk` related bug reports to <tex-dev@vtex.lt>.  

