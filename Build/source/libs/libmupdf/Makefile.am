## Wrapper Makefile.am to build libmupdf for TeX Live.
##
##   Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
##
##   This file is free software; the copyright holder
##   gives unlimited permission to copy and/or distribute it,
##   with or without modifications, as long as this notice is preserved.
##
## We want to re-distribute the whole original libmupdf source tree.
##
EXTRA_DIST = $(LIBMUPDF_TREE)

## Changes applied to the original source tree
##
EXTRA_DIST += $(LIBMUPDF_TREE)-PATCHES

# Files not to be distributed
include $(srcdir)/../../am/dist_hook.am
NEVER_NAMES += $(NEVER_NAMES_SUB)

# SUBDIRS = . include 
SUBDIRS = .


AM_CPPFLAGS = $(LIBMUPDF_DEFINES) $(LIBMUPDF_USER_CPPFLAGS) -DNOCJK -DFZ_ENABLE_JS=0

# AM_CPPFLAGS += $(LIBMUPDF_INCLUDES)
AM_CPPFLAGS += -I$(top_srcdir)/$(LIBMUPDF_TREE)/include -I$(top_srcdir)/$(LIBMUPDF_TREE)

# needed for libmupdf-src/include/mupdf/fitz/system.h
AM_CPPFLAGS += -D__NACL__

# needed for S_IFDIR definition in sys/stat.h / features.h included from libmupdf-src/source/fitz/directory.c
AM_CPPFLAGS += -D_DEFAULT_SOURCE

# AM_CPPFLAGS += -DTOFU_BASE14 -DTOFU -DNOCJK

AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/freetype/include -DFT2_BUILD_LIBRARY
AM_CPPFLAGS += -I@LIBMUPDF_TREE@/thirdparty/libjpeg -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/libjpeg
AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/zlib
AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/jbig2dec
AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/harfbuzz/src -DHAVE_OT -DHAVE_ICU -DHAVE_ICU_BUILTIN -DHB_NO_MT
AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2 -DUSE_JPIP
AM_CPPFLAGS += -I$(top_srcdir)/@LIBMUPDF_TREE@/thirdparty/mujs

# AM_CPPFLAGS += $(ICU_INCLUDES)
AM_CPPFLAGS += -I$(top_srcdir)/../icu/icu-src/source/common

AM_CFLAGS = $(VISIBILITY_CFLAGS) $(WARNING_CFLAGS) -g


noinst_LIBRARIES = libmupdf.a

libmupdf_a_SOURCES = \
	@LIBMUPDF_TREE@/source/cbz/mucbz.c \
	@LIBMUPDF_TREE@/source/cbz/muimg.c \
	@LIBMUPDF_TREE@/source/cbz/mutiff.c \
	@LIBMUPDF_TREE@/source/svg/svg-color.c \
	@LIBMUPDF_TREE@/source/svg/svg-doc.c \
	@LIBMUPDF_TREE@/source/svg/svg-parse.c \
	@LIBMUPDF_TREE@/source/svg/svg-run.c \
	@LIBMUPDF_TREE@/source/xps/xps-common.c \
	@LIBMUPDF_TREE@/source/xps/xps-doc.c \
	@LIBMUPDF_TREE@/source/xps/xps-glyphs.c \
	@LIBMUPDF_TREE@/source/xps/xps-gradient.c \
	@LIBMUPDF_TREE@/source/xps/xps-image.c \
	@LIBMUPDF_TREE@/source/xps/xps-link.c \
	@LIBMUPDF_TREE@/source/xps/xps-outline.c \
	@LIBMUPDF_TREE@/source/xps/xps-path.c \
	@LIBMUPDF_TREE@/source/xps/xps-resource.c \
	@LIBMUPDF_TREE@/source/xps/xps-tile.c \
	@LIBMUPDF_TREE@/source/xps/xps-util.c \
	@LIBMUPDF_TREE@/source/xps/xps-zip.c \
	@LIBMUPDF_TREE@/source/fitz/archive.c \
	@LIBMUPDF_TREE@/source/fitz/bbox-device.c \
	@LIBMUPDF_TREE@/source/fitz/bidi-std.c \
	@LIBMUPDF_TREE@/source/fitz/bidi.c \
	@LIBMUPDF_TREE@/source/fitz/bitmap.c \
	@LIBMUPDF_TREE@/source/fitz/buffer.c \
	@LIBMUPDF_TREE@/source/fitz/colorspace.c \
	@LIBMUPDF_TREE@/source/fitz/colorspace-imp.h \
	@LIBMUPDF_TREE@/source/fitz/compressed-buffer.c \
	@LIBMUPDF_TREE@/source/fitz/context.c \
	@LIBMUPDF_TREE@/source/fitz/crypt-aes.c \
	@LIBMUPDF_TREE@/source/fitz/crypt-arc4.c \
	@LIBMUPDF_TREE@/source/fitz/crypt-md5.c \
	@LIBMUPDF_TREE@/source/fitz/crypt-sha2.c \
	@LIBMUPDF_TREE@/source/fitz/device.c \
	@LIBMUPDF_TREE@/source/fitz/directory.c \
	@LIBMUPDF_TREE@/source/fitz/document-all.c \
	@LIBMUPDF_TREE@/source/fitz/document.c \
	@LIBMUPDF_TREE@/source/fitz/draw-affine.c \
	@LIBMUPDF_TREE@/source/fitz/draw-blend.c \
	@LIBMUPDF_TREE@/source/fitz/draw-device.c \
	@LIBMUPDF_TREE@/source/fitz/draw-edge.c \
	@LIBMUPDF_TREE@/source/fitz/draw-glyph.c \
	@LIBMUPDF_TREE@/source/fitz/draw-imp.h \
	@LIBMUPDF_TREE@/source/fitz/draw-mesh.c \
	@LIBMUPDF_TREE@/source/fitz/draw-paint.c \
	@LIBMUPDF_TREE@/source/fitz/draw-path.c \
	@LIBMUPDF_TREE@/source/fitz/draw-scale-simple.c \
	@LIBMUPDF_TREE@/source/fitz/draw-unpack.c \
	@LIBMUPDF_TREE@/source/fitz/error.c \
	@LIBMUPDF_TREE@/source/fitz/fitz-imp.h \
	@LIBMUPDF_TREE@/source/fitz/filter-basic.c \
	@LIBMUPDF_TREE@/source/fitz/filter-dct.c \
	@LIBMUPDF_TREE@/source/fitz/filter-fax.c \
	@LIBMUPDF_TREE@/source/fitz/filter-flate.c \
	@LIBMUPDF_TREE@/source/fitz/filter-jbig2.c \
	@LIBMUPDF_TREE@/source/fitz/filter-leech.c \
	@LIBMUPDF_TREE@/source/fitz/filter-lzw.c \
	@LIBMUPDF_TREE@/source/fitz/filter-predict.c \
	@LIBMUPDF_TREE@/source/fitz/filter-sgi.c \
	@LIBMUPDF_TREE@/source/fitz/filter-thunder.c \
	@LIBMUPDF_TREE@/source/fitz/font.c \
	@LIBMUPDF_TREE@/source/fitz/font-imp.h \
	@LIBMUPDF_TREE@/source/fitz/ftoa.c \
	@LIBMUPDF_TREE@/source/fitz/function.c \
	@LIBMUPDF_TREE@/source/fitz/geometry.c \
	@LIBMUPDF_TREE@/source/fitz/getopt.c \
	@LIBMUPDF_TREE@/source/fitz/glyph.c \
	@LIBMUPDF_TREE@/source/fitz/glyph-cache-imp.h \
	@LIBMUPDF_TREE@/source/fitz/halftone.c \
	@LIBMUPDF_TREE@/source/fitz/harfbuzz.c \
	@LIBMUPDF_TREE@/source/fitz/hash.c \
	@LIBMUPDF_TREE@/source/fitz/image.c \
	@LIBMUPDF_TREE@/source/fitz/jmemcust.c \
	@LIBMUPDF_TREE@/source/fitz/jmemcust.h \
	@LIBMUPDF_TREE@/source/fitz/link.c \
	@LIBMUPDF_TREE@/source/fitz/list-device.c \
	@LIBMUPDF_TREE@/source/fitz/load-bmp.c \
	@LIBMUPDF_TREE@/source/fitz/load-gif.c \
	@LIBMUPDF_TREE@/source/fitz/load-jpeg.c \
	@LIBMUPDF_TREE@/source/fitz/load-jpx.c \
	@LIBMUPDF_TREE@/source/fitz/load-jxr.c \
	@LIBMUPDF_TREE@/source/fitz/load-png.c \
	@LIBMUPDF_TREE@/source/fitz/load-pnm.c \
	@LIBMUPDF_TREE@/source/fitz/load-tiff.c \
	@LIBMUPDF_TREE@/source/fitz/memento.c \
	@LIBMUPDF_TREE@/source/fitz/memory.c \
	@LIBMUPDF_TREE@/source/fitz/noto.c \
	@LIBMUPDF_TREE@/source/fitz/outline.c \
	@LIBMUPDF_TREE@/source/fitz/output-cbz.c \
	@LIBMUPDF_TREE@/source/fitz/output-pcl.c \
	@LIBMUPDF_TREE@/source/fitz/output-png.c \
	@LIBMUPDF_TREE@/source/fitz/output-pnm.c \
	@LIBMUPDF_TREE@/source/fitz/output-ps.c \
	@LIBMUPDF_TREE@/source/fitz/output-pwg.c \
	@LIBMUPDF_TREE@/source/fitz/output-svg.c \
	@LIBMUPDF_TREE@/source/fitz/output-tga.c \
	@LIBMUPDF_TREE@/source/fitz/output.c \
	@LIBMUPDF_TREE@/source/fitz/path.c \
	@LIBMUPDF_TREE@/source/fitz/pixmap.c \
	@LIBMUPDF_TREE@/source/fitz/pool.c \
	@LIBMUPDF_TREE@/source/fitz/printf.c \
	@LIBMUPDF_TREE@/source/fitz/separation.c \
	@LIBMUPDF_TREE@/source/fitz/shade.c \
	@LIBMUPDF_TREE@/source/fitz/stext-device.c \
	@LIBMUPDF_TREE@/source/fitz/stext-output.c \
	@LIBMUPDF_TREE@/source/fitz/stext-paragraph.c \
	@LIBMUPDF_TREE@/source/fitz/stext-search.c \
	@LIBMUPDF_TREE@/source/fitz/store.c \
	@LIBMUPDF_TREE@/source/fitz/stream-open.c \
	@LIBMUPDF_TREE@/source/fitz/stream-prog.c \
	@LIBMUPDF_TREE@/source/fitz/stream-read.c \
	@LIBMUPDF_TREE@/source/fitz/string.c \
	@LIBMUPDF_TREE@/source/fitz/strtod.c \
	@LIBMUPDF_TREE@/source/fitz/strtof.c \
	@LIBMUPDF_TREE@/source/fitz/svg-device.c \
	@LIBMUPDF_TREE@/source/fitz/tempfile.c \
	@LIBMUPDF_TREE@/source/fitz/test-device.c \
	@LIBMUPDF_TREE@/source/fitz/text.c \
	@LIBMUPDF_TREE@/source/fitz/time.c \
	@LIBMUPDF_TREE@/source/fitz/trace-device.c \
	@LIBMUPDF_TREE@/source/fitz/track-usage.c \
	@LIBMUPDF_TREE@/source/fitz/transition.c \
	@LIBMUPDF_TREE@/source/fitz/tree.c \
	@LIBMUPDF_TREE@/source/fitz/ucdn.c \
	@LIBMUPDF_TREE@/source/fitz/unicodedata_db.h \
	@LIBMUPDF_TREE@/source/fitz/untar.c \
	@LIBMUPDF_TREE@/source/fitz/unzip.c \
	@LIBMUPDF_TREE@/source/fitz/util.c \
	@LIBMUPDF_TREE@/source/fitz/writer.c \
	@LIBMUPDF_TREE@/source/fitz/xml.c \
	@LIBMUPDF_TREE@/source/fitz/zip.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-annot-edit.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-annot.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-appearance.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-clean-file.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-clean.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-cmap-load.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-cmap-parse.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-cmap-table.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-cmap.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-colorspace.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-crypt.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-device.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-encoding.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-encodings.h \
	@LIBMUPDF_TREE@/source/pdf/pdf-event.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-field.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-font.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-form.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-function.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-glyphlist.h \
	@LIBMUPDF_TREE@/source/pdf/pdf-graft.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-image.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-imp.h \
	@LIBMUPDF_TREE@/source/pdf/pdf-interpret.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-interpret-imp.h \
	@LIBMUPDF_TREE@/source/pdf/pdf-js-util.js \
	@LIBMUPDF_TREE@/source/pdf/pdf-js.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-layer.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-lex.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-metrics.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-nametree.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-object.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-op-buffer.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-op-filter.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-op-run.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-outline.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-page.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-parse.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-pattern.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-pkcs7.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-portfolio.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-repair.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-resources.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-run.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-shade.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-store.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-stream.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-type3.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-unicode.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-write.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-xobject.c \
	@LIBMUPDF_TREE@/source/pdf/pdf-xref.c \
	@LIBMUPDF_TREE@/source/html/css-apply.c \
	@LIBMUPDF_TREE@/source/html/css-parse.c \
	@LIBMUPDF_TREE@/source/html/epub-doc.c \
	@LIBMUPDF_TREE@/source/html/html-doc.c \
	@LIBMUPDF_TREE@/source/html/html-font.c \
	@LIBMUPDF_TREE@/source/html/html-layout.c \
	@LIBMUPDF_TREE@/source/gprf/gprf-doc.c \
	@LIBMUPDF_TREE@/source/gprf/gprf-skeleton.c

# generated font images
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/generated/CharisSIL-B.c \
	@LIBMUPDF_TREE@/generated/CharisSIL-BI.c \
	@LIBMUPDF_TREE@/generated/CharisSIL-I.c \
	@LIBMUPDF_TREE@/generated/CharisSIL-R.c \
	@LIBMUPDF_TREE@/generated/Dingbats.c \
	@LIBMUPDF_TREE@/generated/DroidSansFallback.c \
	@LIBMUPDF_TREE@/generated/DroidSansFallbackFull.c \
	@LIBMUPDF_TREE@/generated/gen_adobe_ca.h \
	@LIBMUPDF_TREE@/generated/gen_cmap_cjk.h \
	@LIBMUPDF_TREE@/generated/gen_cmap_extra.h \
	@LIBMUPDF_TREE@/generated/gen_cmap_utf32.h \
	@LIBMUPDF_TREE@/generated/gen_cmap_utf8.h \
	@LIBMUPDF_TREE@/generated/gen_js_util.h \
	@LIBMUPDF_TREE@/generated/NimbusMonoPS-Bold.c \
	@LIBMUPDF_TREE@/generated/NimbusMonoPS-BoldItalic.c \
	@LIBMUPDF_TREE@/generated/NimbusMonoPS-Italic.c \
	@LIBMUPDF_TREE@/generated/NimbusMonoPS-Regular.c \
	@LIBMUPDF_TREE@/generated/NimbusRoman-Bold.c \
	@LIBMUPDF_TREE@/generated/NimbusRoman-BoldItalic.c \
	@LIBMUPDF_TREE@/generated/NimbusRoman-Italic.c \
	@LIBMUPDF_TREE@/generated/NimbusRoman-Regular.c \
	@LIBMUPDF_TREE@/generated/NimbusSans-Bold.c \
	@LIBMUPDF_TREE@/generated/NimbusSans-BoldOblique.c \
	@LIBMUPDF_TREE@/generated/NimbusSans-Oblique.c \
	@LIBMUPDF_TREE@/generated/NimbusSans-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoEmoji-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoKufiArabic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoNaskhArabic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoNastaliqUrdu-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansArmenian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansAvestan-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBalinese-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBamum-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBatak-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBengali-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBrahmi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBuginese-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansBuhid-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCanadianAboriginal-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCarian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCham-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCherokee-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCypriot-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCoptic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansCuneiform-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansDeseret-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansDevanagari-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansEgyptianHieroglyphs-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansEthiopic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansGeorgian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansGlagolitic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansGothic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansGujarati-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansGurmukhi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansHanunoo-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansHebrew-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansImperialAramaic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansInscriptionalPahlavi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansInscriptionalParthian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansYi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansJavanese-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansKaithi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansKayahLi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansKannada-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansKharoshthi-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansKhmer-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLao-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLepcha-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLimbu-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLinearB-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLisu-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLycian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansLydian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansMalayalam-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansMandaic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansMeeteiMayek-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansMyanmar-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansMongolian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansNewTaiLue-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansNKo-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOgham-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOlChiki-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOldItalic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOldPersian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOldSouthArabian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOldTurkic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOriya-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansOsmanya-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansPhagsPa-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansPhoenician-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSans-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansRejang-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansRunic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSamaritan-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSaurashtra-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansShavian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSinhala-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSylotiNagri-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSymbols-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSyriacEastern-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSyriacEstrangela-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSyriacWestern-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansSundanese-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTagalog-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTagbanwa-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTaiLe-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTaiTham-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTaiViet-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTamil-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTelugu-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansThaana-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansThai-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTibetan-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansTifinagh-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansUgaritic-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSansVai-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifArmenian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifBengali-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifDevanagari-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifGeorgian-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifGujarati-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifKannada-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifKhmer-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifLao-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifMalayalam-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerif-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifTamil-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifTelugu-Regular.c \
	@LIBMUPDF_TREE@/generated/NotoSerifThai-Regular.c \
	@LIBMUPDF_TREE@/generated/SourceHanSansCN-Regular.c \
	@LIBMUPDF_TREE@/generated/SourceHanSansJP-Regular.c \
	@LIBMUPDF_TREE@/generated/SourceHanSansKR-Regular.c \
	@LIBMUPDF_TREE@/generated/SourceHanSansTW-Regular.c \
	@LIBMUPDF_TREE@/generated/StandardSymbolsPS.c


## freetype sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftbase.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftbbox.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftbitmap.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftfntfmt.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftgasp.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftglyph.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftinit.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftstroke.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftsynth.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftsystem.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/fttype1.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/cff/cff.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/cid/type1cid.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/psaux/psaux.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/pshinter/pshinter.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/psnames/psnames.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/raster/raster.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/sfnt/sfnt.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/smooth/smooth.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/truetype/truetype.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/type1/type1.c

libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftdebug.c

libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/autofit/autofit.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/pfr/pfr.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/type42/type42.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/winfonts/winfnt.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/pcf/pcf.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/bdf/bdf.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/gzip/ftgzip.c \
	@LIBMUPDF_TREE@/thirdparty/freetype/src/lzw/ftlzw.c

# libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftbdf.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftcid.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftfstype.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftgxval.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftlcdfil.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftmm.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftotval.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftpatent.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftpfr.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/base/ftwinfnt.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/bzip2/ftbzip2.c \
#	@LIBMUPDF_TREE@/thirdparty/freetype/src/cache/ftcache.c


## libjpeg sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jaricom.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcapimin.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcapistd.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcarith.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jccoefct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jccolor.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcdctmgr.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jchuff.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcinit.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcmainct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcmarker.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcmaster.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcomapi.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcparam.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcprepct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jcsample.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jctrans.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdapimin.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdapistd.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdarith.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdatadst.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdatasrc.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdcoefct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdcolor.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jddctmgr.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdhuff.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdinput.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdmainct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdmarker.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdmaster.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdmerge.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdpostct.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdsample.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jdtrans.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jerror.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jfdctflt.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jfdctfst.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jfdctint.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jidctflt.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jidctfst.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jidctint.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jquant1.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jquant2.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jutils.c \
	@LIBMUPDF_TREE@/thirdparty/libjpeg/jmemmgr.c

# libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/libjpeg/@MEMORYMGR@.c


## zlib sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/zlib/adler32.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/compress.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/crc32.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/crc32.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/deflate.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/deflate.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/infback.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/gzclose.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/gzguts.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/gzlib.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/gzread.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/gzwrite.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/inffast.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/inffast.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/inffixed.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/inflate.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/inflate.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/inftrees.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/inftrees.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/trees.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/trees.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/uncompr.c \
	@LIBMUPDF_TREE@/thirdparty/zlib/zutil.h \
	@LIBMUPDF_TREE@/thirdparty/zlib/zutil.c


## jbig2dec sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith_int.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith_iaid.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_huffman.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_segment.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_page.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_symbol_dict.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_text.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_generic.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_refinement.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_mmr.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_halftone.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_image.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_image_pbm.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/os_types.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/config_types.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/config_win32.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_priv.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_image.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith_iaid.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_arith_int.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_huffman.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_hufftab.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_mmr.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_generic.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_symbol_dict.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_text.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_metadata.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/jbig2_metadata.h \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/memento.c \
	@LIBMUPDF_TREE@/thirdparty/jbig2dec/memento.h


## harfbuzz sources
## common
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-atomic-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-blob.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-buffer-deserialize-json.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-buffer-deserialize-text.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-buffer-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-buffer-serialize.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-buffer.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-cache-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-common.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-face-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-face.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-font-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-font.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-mutex-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-object-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-open-file-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-open-type-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-cbdt-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-cmap-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-glyf-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-head-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-hhea-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-hmtx-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-maxp-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-name-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-os2-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-post-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-tag.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-set-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-set.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shape.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shape-plan-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shape-plan.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shaper-list.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shaper-impl-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shaper-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-shaper.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-unicode-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-unicode.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-utf-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-warning.cc

## fallback
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-fallback-shape.cc

## native OpenType Layout backend
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-font.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-common-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-gdef-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-gpos-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-gsubgpos-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-gsub-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-jstf-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-layout-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-map.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-map-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-arabic.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-arabic-fallback.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-arabic-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-arabic-table.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-arabic-win1256.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-default.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-hangul.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-hebrew.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-indic.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-indic-machine.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-indic-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-indic-table.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-myanmar.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-myanmar-machine.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-thai.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-tibetan.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-use.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-use-machine.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-use-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-complex-use-table.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-fallback.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-fallback-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-normalize.cc \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-normalize-private.hh \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ot-shape-private.hh

## Optional Sources and Headers with external deps
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ft.cc

# libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-glib.cc

## Graphite library
# libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-graphite2.cc

## System-dependent sources and headers
#libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-coretext.cc \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-directwrite.cc \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-uniscribe.cc

## Additional supplemental sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-ucdn.cc

## Sources for libharfbuzz-gobject and libharfbuzz-icu
## ICU library
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-icu.cc

# libmupdf_a_SOURCES += \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-gobject-structs.cc \
#	@LIBMUPDF_TREE@/thirdparty/harfbuzz/src/hb-gobject-enums.cc


## openjp2 sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/bio.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/cidx_manager.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/cio.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/dwt.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/event.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/function_list.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/image.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/invert.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/j2k.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/jp2.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/mct.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/mqc.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/openjpeg.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/opj_clock.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/opj_malloc.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/phix_manager.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/pi.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/ppix_manager.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/raw.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/t1.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/t1_generate_luts.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/t2.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/tcd.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/tgt.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/thix_manager.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/thread.c \
	@LIBMUPDF_TREE@/thirdparty/openjpeg/src/lib/openjp2/tpix_manager.c


## mujs sources
libmupdf_a_SOURCES += \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsarray.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsboolean.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsbuiltin.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jscompile.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsdate.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsdtoa.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsdump.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jserror.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsfunction.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsgc.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsintern.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jslex.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsmath.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsnumber.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsobject.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/json.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsparse.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsproperty.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsregexp.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsrun.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsstate.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsstring.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/jsvalue.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/main.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/one.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/regexp.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/utf.c \
	@LIBMUPDF_TREE@/thirdparty/mujs/utftype.c


$(libmupdf_a_OBJECTS): config.force

CLEANFILES = 

LDADD = libmupdf.a

clean-local:
	rm -rf libmupdf libmupdf.* 

# Rebuild libmupdf
@LIBMUPDF_RULE@

#Reconfig
reconfig_prereq = $(LIBMUPDF_DEPEND)
DISTCLEANFILES = 

#Rebuild
rebuild_prereq =
rebuild_target = all

include $(srcdir)/../../am/rebuild.am
