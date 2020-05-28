## $Id: Makefile.am 43617 2017-03-27 18:00:45Z karl $
### Makefile.am for the TeX Live subdirectory texk/xdvipsk/
##
## Copyright 2017-2020 Karl Berry <tex-live@tug.org>
## Copyright 2009-2015 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##
GRAPHLIB_INCLUDES = $(srcdir)/graphlib

AM_CPPFLAGS = $(KPATHSEA_INCLUDES) -DUNIX -DKPATHSEA -DSHIFTLOWCHARS 
AM_CFLAGS = $(WARNING_CFLAGS) -D_FORTIFY_SOURCE=1

##AM_LDFLAGS = -lstdc++
#AM_LDFLAGS = -Wl,-Map,xdvipsk.map

bin_PROGRAMS = xdvipsk

xdvipsk_CPPFLAGS = -I$(srcdir)/otflib -I$(GRAPHLIB_INCLUDES) $(ZLIB_INCLUDES) $(LIBPNG_INCLUDES) $(LIBJPEG_INCLUDES) $(LIBTIFF_INCLUDES) $(KPATHSEA_INCLUDES) -DUNIX -DKPATHSEA -DSHIFTLOWCHARS  

xdvipsk_SOURCES = \
	bbox.c \
	bitmapenc.c \
	charcode.c \
	color.c \
	config.h \
	debug.h \
	dopage.c \
	dosection.c \
	dospecial.c \
	download.c \
	dpicheck.c \
	drawPS.c \
	dviinput.c \
	emspecialex.c \
	finclude.c \
	fontdef.c \
	header.c \
	hps.c \
	loadfont.c \
	luamap.c \
	makefont.c \
	output.c \
	papersiz.c \
	paths.h \
	pprescan.c \
	prescan.c \
	protos.h \
	protos_add.h \
	ptexmac.h \
	repack.c \
	resident.c \
	scalewidth.c \
	scanpage.c \
	search.c \
	sfntload.c \
	skippage.c \
	t1part.c \
	t1part.h \
	tfmload.c \
	unpack.c \
	utarray.h \
	uthash.h \
	virtualfont.c \
	writecid.c \
	writet1.c \
	xdvips.c \
	xdvips.h \
	otflib/cff.c \
	otflib/cff.h \
	otflib/cff_dict.c \
	otflib/cff_dict.h \
	otflib/cff_limits.h \
	otflib/cff_stdstr.h \
	otflib/cff_types.h \
	otflib/cidsysinfo.c \
	otflib/cidsysinfo.h \
	otflib/cidtype0.c \
	otflib/cidtype0.h \
	otflib/cidtype2.c \
	otflib/cidtype2.h \
	otflib/cmap.c \
	otflib/cmap.h \
	otflib/cmap_p.h \
	otflib/cmap_write.c \
	otflib/cmap_write.h \
	otflib/cs_type2.c \
	otflib/cs_type2.h \
	otflib/dpxfile.c \
	otflib/dpxfile.h \
	otflib/dpxutil.c \
	otflib/dpxutil.h \
	otflib/error.c \
	otflib/error.h \
	otflib/mem.c \
	otflib/mem.h \
	otflib/mfileio.c \
	otflib/mfileio.h \
	otflib/numbers.c \
	otflib/numbers.h \
	otflib/sfnt.c \
	otflib/sfnt.h \
	otflib/system.h \
	otflib/tt_aux.c \
	otflib/tt_aux.h \
	otflib/tt_cmap.c \
	otflib/tt_cmap.h \
	otflib/tt_glyf.c \
	otflib/tt_glyf.h \
	otflib/tt_post.c \
	otflib/tt_post.h \
	otflib/tt_table.c \
	otflib/tt_table.h \
	otflib/unicode.c \
	otflib/unicode.h

$(xdvipsk_OBJECTS): $(KPATHSEA_DEPEND) $(ZLIB_DEPEND) $(LIBPNG_DEPEND) $(LIBJPEG_DEPEND) $(LIBTIFF_DEPEND)

xdvipsk_LDADD = libdvipsgraph.a $(KPATHSEA_LIBS) $(LIBTIFF_LIBS) $(LIBPNG_LIBS) $(ZLIB_LIBS) $(LIBJPEG_LIBS) -lstdc++

noinst_LIBRARIES = libdvipsgraph.a

libdvipsgraph_a_dependencies =
libdvipsgraph_a_SOURCES = \
    graphlib/bitmap.cpp \
    graphlib/bitmap.h \
    graphlib/bitmapAccess.cpp \
    graphlib/bitmapIO.cpp \
    graphlib/bitmapIO.h \
    graphlib/colors.cpp \
    graphlib/conversion.cpp \
    graphlib/conversion24.cpp \
    graphlib/conversion32.cpp \
    graphlib/conversion8.cpp \
    graphlib/filters.h \
    graphlib/loadBMP.cpp \
    graphlib/loadJPEG.cpp \
    graphlib/loadPCX.cpp \
    graphlib/loadPNG.cpp \
    graphlib/loadTIFF.cpp \
    graphlib/loaders.h \
    graphlib/resample.cpp \
    graphlib/rescale.cpp \
    graphlib/resize.cpp \
    graphlib/resize.h \
    graphlib/tiffLogLuv.cpp \
    graphlib/toFloat.cpp \
    graphlib/toFloat.h \
    graphlib/utilities.h

libdvipsgraph_a_dependencies += $(ZLIB_DEPEND)
libdvipsgraph_a_dependencies += $(LIBPNG_DEPEND)
libdvipsgraph_a_dependencies += $(LIBJPEG_DEPEND)
libdvipsgraph_a_dependencies += $(LIBTIFF_DEPEND)

$(libdvipsgraph_a_OBJECTS): $(ZLIB_DEPEND) $(LIBPNG_DEPEND) $(LIBJPEG_DEPEND) $(LIBTIFF_DEPEND) config.force

AM_CXXFLAGS = @AM_CXXFLAGS@

libdvipsgraph_a_CPPFLAGS = $(ZLIB_INCLUDES) $(LIBPNG_INCLUDES) $(LIBJPEG_INCLUDES) $(LIBTIFF_INCLUDES)

libdvipsgraph_a_LIBADD = $(LIBTIFF_LIBS) $(LIBPNG_LIBS) $(ZLIB_LIBS) $(LIBJPEG_LIBS)

###############################################################################
# THE LIBRARIES TO BUILD
###############################################################################

## Rebuild libkpathsea
@KPATHSEA_RULE@
## Rebuild libz
@ZLIB_RULE@
## Rebuild libpng
@LIBPNG_RULE@
## Rebuild libjpeg
@LIBJPEG_RULE@
## Rebuild libtiff
@LIBTIFF_RULE@

enc_DATA = dvips-all.enc

# Files not to be distributed
include $(srcdir)/../../am/dist_hook.am
