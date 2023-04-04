SHELL=/bin/bash

tldest=../../Master
tlsource=tug.org::tldevsrc/Master

default: help 

dvipsdef: dvips.def dvips.def.sed
	sed -zf dvips.def.sed <dvips.def> dvips.def.xdvipsk
	mv dvips.def.xdvipsk $(tldest)/texmf-dist/tex/latex/graphics-def/dvips.def 
	rm -rf dvips.def dvips.def.sed

dvips.def:
	rsync -avz $(tlsource)/texmf-dist/tex/latex/graphics-def/dvips.def .

dvips.def.sed:
	@echo 's+\\special{em: graph #1,\\Gin@urx bp}%+\\special{em: graph #1,\\number\\Gin@req@width sp}%+g' >dvips.def.sed
	@echo 's+\\special{em: graph #1,\\Gin@urx bp,\\Gin@ury bp}%+\\special{em: graph #1,\\number\\Gin@req@width sp,\\number\\Gin@req@height sp}%+g' >>dvips.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.tif}#1{{bmp}{.tif.bb}{#1}}\n\\endinput+g' >>dvips.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.tiff}#1{{bmp}{.tiff.bb}{#1}}\n\\endinput+g' >>dvips.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.jpeg}#1{{bmp}{.jpeg.bb}{#1}}\n\\endinput+g' >>dvips.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.jpg}#1{{bmp}{.jpg.bb}{#1}}\n\\endinput+g' >>dvips.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.png}#1{{bmp}{.png.bb}{#1}}\n\\endinput+g' >>dvips.def.sed

luaotfload: luaotfload.lua luaotfload.lua.sed luaotfload-database.lua luaotfload-database.lua.sed
	sed -zf luaotfload.lua.sed <luaotfload.lua> luaotfload.lua.xdvipsk
	mv luaotfload.lua.xdvipsk $(tldest)/texmf-dist/tex/luatex/luaotfload/luaotfload.lua
	sed -zf luaotfload-database.lua.sed <luaotfload-database.lua> luaotfload-database.lua.xdvipsk
	mv luaotfload-database.lua.xdvipsk $(tldest)/texmf-dist/tex/luatex/luaotfload/luaotfload-database.lua
	rm -rf luaotfload.lua luaotfload.lua.sed luaotfload-database.lua luaotfload-database.lua.sed

luaotfload.lua:
	rsync -avz $(tlsource)/texmf-dist/tex/luatex/luaotfload/luaotfload.lua .

luaotfload-database.lua:
	rsync -avz $(tlsource)/texmf-dist/tex/luatex/luaotfload/luaotfload-database.lua .

luaotfload.lua.sed:
	@echo 's+loadmodule "dvi"+-- !breaks xvipsk! loadmodule "dvi"+g' >luaotfload.lua.sed

luaotfload-database.lua.sed:
	@echo 's+tableappend (filenames, collect_font_filenames_system ())+-- !VTeX: switched off system fonts! tableappend (filenames, collect_font_filenames_system ())+g' >luaotfload-database.lua.sed

help:
	@echo "To adapt dvips.def for xdvipsk:"
	@echo "  $$ make dvipsdef"
	@echo
	@echo "To adapt luaotfload for xdvipsk:"
	@echo "  $$ make luaotfload"

