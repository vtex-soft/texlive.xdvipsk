SHELL=/bin/bash

tldest=../../Master
tlsource=tug.org::tldevsrc/Master/texmf-dist

default: help 

dvipsdef: driver.src xdvipsk.def.sed
	sed -zf xdvipsk.def.sed <dvips.def> xdvipsk.def
	sed -zi 's/color configuration]\n/color configuration]\n\n\\declare@file@substitution{dvips.def}{xdvipsk.def}\n/g' color.cfg
	sed -zEi 's/graphics configuration]%?\n/graphics configuration]\n\n\\declare@file@substitution{dvips.def}{xdvipsk.def}\n/g' graphics.cfg
	mv xdvipsk.def $(tldest)/texmf-config/tex/latex/graphics-def/xdvipsk.def
	mv color.cfg $(tldest)/texmf-config/tex/latex/graphics-cfg/color.cfg
	mv graphics.cfg $(tldest)/texmf-config/tex/latex/graphics-cfg/graphics.cfg
	rm -rf dvips.def xdvipsk.def.sed 

driver.src:
	rsync -avz $(tlsource)/tex/latex/graphics-def/dvips.def .
	rsync -avz $(tlsource)/tex/latex/graphics-cfg/color.cfg .
	rsync -avz $(tlsource)/tex/latex/graphics-cfg/graphics.cfg .

xdvipsk.def.sed:
	@echo 's+\\ProvidesFile{dvips.def}+\\ProvidesFile{xdvipsk.def}+g' >xdvipsk.def.sed
	@echo 's+driver for dvips]+driver for xdvipsk]+g' >>xdvipsk.def.sed
	@echo 's+\\special{em: graph #1,\\Gin@urx bp}%+\\special{em: graph #1,\\number\\Gin@req@width sp}%+g' >>xdvipsk.def.sed
	@echo 's+\\special{em: graph #1,\\Gin@urx bp,\\Gin@ury bp}%+\\special{em: graph #1,\\number\\Gin@req@width sp,\\number\\Gin@req@height sp}%+g' >>xdvipsk.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.tif}#1{{bmp}{.tif.bb}{#1}}\n\\endinput+g' >>xdvipsk.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.tiff}#1{{bmp}{.tiff.bb}{#1}}\n\\endinput+g' >>xdvipsk.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.jpeg}#1{{bmp}{.jpeg.bb}{#1}}\n\\endinput+g' >>xdvipsk.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.jpg}#1{{bmp}{.jpg.bb}{#1}}\n\\endinput+g' >>xdvipsk.def.sed
	@echo 's+\n\\endinput+\n\\@namedef{Gin@rule@.png}#1{{bmp}{.png.bb}{#1}}\n\\endinput+g' >>xdvipsk.def.sed

help:
	@echo "To adapt dvips driver for xdvipsk:"
	@echo "  $$ make dvipsdef"

