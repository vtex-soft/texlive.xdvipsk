## Makefile.am for the TeX Live subdirectory utils/make2unc/
##
## Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
## You may freely use, modify and/or distribute this file.
##

MUPDF_INCLUDES = -I$(abs_top_srcdir)/libs.prebuilt/libmupdf/include
MUPDF_DEPEND = $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmupdf.a \
               $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmupdfthird.a \
               $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmuthreads.a

MUPDF_LIBS = $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmupdf.a \
             $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmupdfthird.a \
             $(abs_top_srcdir)/libs.prebuilt/libmupdf/$(ARCH_LIBS_DIR)/libmuthreads.a

#MUPDF_INCLUDES = -I$(abs_top_srcdir)/../../libs/libmupdf/libmupdf-src/include
#MUPDF_DEPEND = ../../libs/libmupdf/libmupdf.a
#MUPDF_LIBS = ../../libs/libmupdf/libmupdf.a

AM_CPPFLAGS = $(MUPDF_INCLUDES) $(ICU_INCLUDES) -DUNIX -DSHIFTLOWCHARS
make2unc_CXXFLAGS = -std=c++11

bin_PROGRAMS = make2unc
make2unc_SOURCES = src/make2unc.cpp

@ICU_RULE@
## Rebuild libmupdf
#@LIBMUPDF_RULE@

LDADD = $(MUPDF_LIBS)
LDADD += $(ICU_LIBS) $(ICU_LIBS_EXTRA)
#LDADD += ../../libs/icu/icu-build/lib/libicuuc.a ../../libs/icu/icu-build/lib/libicudata.a

$(make2unc_OBJECTS): $(MUPDF_DEPEND) $(ICU_DEPEND)
