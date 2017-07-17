## $Id: Makefile.am 43617 2017-03-27 18:00:45Z karl $
### Makefile.am for the TeX Live subdirectory texk/xdvipsk/
##
## Copyright 2017      Karl Berry <tex-live@tug.org>
## Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##
SUBDIRS = squeeze

GRAPHLIB_INCLUDES = $(srcdir)/graphlib

AM_CPPFLAGS = $(KPATHSEA_INCLUDES) -DUNIX -DKPATHSEA -DSHIFTLOWCHARS 
AM_CFLAGS = $(WARNING_CFLAGS) -D_FORTIFY_SOURCE=1

##AM_LDFLAGS = -lstdc++
#AM_LDFLAGS = -Wl,-Map,xdvipsk.map

bin_PROGRAMS = afm2tfm xdvipsk

xdvipsk_CPPFLAGS = -I$(srcdir)/otflib -I$(GRAPHLIB_INCLUDES) $(LIBTIFF_INCLUDES) $(LIBPNG_INCLUDES) $(ZLIB_INCLUDES) $(LIBJPEG_INCLUDES) $(KPATHSEA_INCLUDES) -DUNIX -DKPATHSEA -DSHIFTLOWCHARS  

afm2tfm_SOURCES = afm2tfm.c

$(afm2tfm_OBJECTS): $(KPATHSEA_DEPEND)

xdvipsk_SOURCES = \
	bbox.c \
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
	dvips.c \
	dvips.h \
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

$(xdvipsk_OBJECTS): $(KPATHSEA_DEPEND) $(LIBTIFF_DEPEND) $(LIBPNG_DEPEND) $(ZLIB_DEPEND) $(LIBJPEG_DEPEND)

xdvipsk_LDADD = libdvipsgraph.a $(KPATHSEA_LIBS) $(LIBTIFF_LIBS) $(LIBPNG_LIBS) $(ZLIB_LIBS) $(LIBJPEG_LIBS) -lstdc++ 

afm2tfm_LDADD = $(KPATHSEA_LIBS)

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

libdvipsgraph_a_dependencies += $(LIBPNG_DEPEND)
libdvipsgraph_a_dependencies += $(ZLIB_DEPEND)
libdvipsgraph_a_dependencies += $(LIBJPEG_DEPEND)
libdvipsgraph_a_dependencies += $(LIBTIFF_DEPEND)

$(libdvipsgraph_a_OBJECTS): $(LIBTIFF_DEPEND) $(LIBPNG_DEPEND) $(ZLIB_DEPEND) $(LIBJPEG_DEPEND) config.force

AM_CXXFLAGS = @AM_CXXFLAGS@

libdvipsgraph_a_CPPFLAGS = $(LIBPNG_INCLUDES) $(ZLIB_INCLUDES) $(LIBJPEG_INCLUDES) $(LIBTIFF_INCLUDES)

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

dist_man1_MANS = afm2tfm.1 dvips.1

info_TEXINFOS = dvips.texi
xdvipsk_TEXINFOS = contrib/config.proto dvips.help

DISTCLEANFILES = $(DVIS) $(PSS)

prolog_DATA = $(prologues)

dist_prologues = \
	color.lpro \
	crop.lpro \
	finclude.lpro \
	hps.lpro \
	special.lpro \
	tex.lpro \
	texps.lpro \
    texcid.lpro

prologdir = $(datarootdir)/texmf-dist/dvips/base

prologues = $(dist_prologues:.lpro=.pro) texc.pro

texc.lpro: texc.script tex.lpro
	$(AM_V_GEN)$(SHELL) $(srcdir)/texc.script $(srcdir)/tex.lpro $@

SUFFIXES = .pro .lpro

# The `t[mp]-' business is so that we don't create an empty .pro file,
# if running squeeze fails for some reason.  A dependency on squeeze
# fails here, so we include it below.
.lpro.pro:
	$(AM_V_GEN)squeeze/squeeze <$< >t-$@ && mv t-$@ $@

# When cross-compiling, EXEEXT for the build and host systems may differ.
# Thus we depend on the auxiliary file squeeze/stamp-squeeze.
$(prologues): squeeze/stamp-squeeze

squeeze/stamp-squeeze:
	cd squeeze && $(MAKE) $(AM_MAKEFLAGS) stamp-squeeze

EXTRA_DIST = $(dist_prologues) texc.script

CLEANFILES = $(prologues) texc.lpro

## xdvipsk tests
TEST_EXTENSIONS = .pl .test
TESTS = afm2tfm-test.pl
afm2tfm-test.log: afm2tfm$(EXEEXT)
TESTS += beginfontk1.test eepic-nan.test pfbincl.test \
	quotecmd-test.pl same-name.test test-dvips.test
beginfontk1.log eepic-nan.log pfbincl.log \
	quotecmd-test.log same-name.log test-dvips.log: xdvipsk$(EXEEXT)

AM_TESTS_ENVIRONMENT  = TEXMFCNF=$(srcdir)/../kpathsea; export TEXMFCNF;
AM_TESTS_ENVIRONMENT += TEXCONFIG=$(srcdir)/testdata; export TEXCONFIG;
AM_TESTS_ENVIRONMENT += TEXFONTS=$(srcdir)/testdata; export TEXFONTS;
AM_TESTS_ENVIRONMENT += TEXFONTMAPS=$(srcdir)/testdata; export TEXFONTMAPS;
AM_TESTS_ENVIRONMENT += TEXPSHEADERS=$(srcdir)/testdata; export TEXPSHEADERS;

EXTRA_DIST += \
	$(TESTS) \
	testdata/8r.enc \
	testdata/ad.enc \
	testdata/alt-rule.pro \
	testdata/cmex10.pfb \
	testdata/cmex10.tfm \
	testdata/cmmi10.pfb \
	testdata/cmmi10.tfm \
	testdata/cmr10.pfb \
	testdata/cmr10.tfm \
	testdata/cmsy10.pfb \
	testdata/cmsy10.tfm \
	testdata/cmtt10.pfb \
	testdata/cmtt10.tfm \
	testdata/config.pdf \
	testdata/config.ps \
	testdata/psfonts.map \
	testdata/ptmbi.vf \
	testdata/ptmbi8r.tfm \
	testdata/ptmr.vf \
	testdata/ptmr8r.tfm \
	testdata/ptmri.vf \
	testdata/ptmri8r.tfm \
	testdata/special.pro \
	testdata/tex.pro \
	testdata/texc.pro \
	testdata/texps.pro \
    testdata/texcid.pro

## afm2tfm-test.pl
DISTCLEANFILES += afmtest.tfm
## beginfontk1.test
EXTRA_DIST += testdata/beginfontk1.dvi testdata/beginfontk1.eps testdata/beginfontk1.tex
DISTCLEANFILES += beginfontk1.ps
## eepic-nan.test
EXTRA_DIST += testdata/eepic-nan.dvi testdata/eepic-nan.tex
DISTCLEANFILES += eepic-nan.ps
## pfbincl.test
EXTRA_DIST += testdata/pfbincl.eps testdata/pfbincl.tex testdata/pfbincl.xdv testdata/pfbincl.xps 
DISTCLEANFILES += pfbincl.ps
## quotecmd-test.pl
EXTRA_DIST += testdata/quotecmd.dvi testdata/quotecmd.tex 
DISTCLEANFILES += *badnews*
## same-name.test
DISTCLEANFILES += same-name.out
## test-dvips
EXTRA_DIST += testdata/dvipstst.tex testdata/dvipstst.xdv testdata/dvipstst.xps
DISTCLEANFILES += dvipstst.ps missfont.log mtest.ps

EXTRA_DIST += \
	NEWS \
	TODO \
	testdata/intoverflow.dvi \
	testdata/vfnameoverflow.dvi

## Not used
##
EXTRA_DIST += \
	atari \
	cmfonts.map \
	config.ps \
	contrib/afm-extra \
	contrib/colorsep.lpro \
	contrib/configs \
	contrib/crop.lpr \
	contrib/latex209 \
	contrib/treen.sh \
	contrib/treen2.sh \
	contrib/volker \
	contrib/whichinpath \
	cyrfonts.map \
	eufonts.map \
	flib.c \
	hps/CHANGES_HPS.txt \
	hps/README_HPS.txt \
	lafonts.map \
	makefont.c \
	mvs \
	psfonts.map \
	tex \
	vmcms \
	vms

# Files not to be distributed
include $(srcdir)/../../am/dist_hook.am
