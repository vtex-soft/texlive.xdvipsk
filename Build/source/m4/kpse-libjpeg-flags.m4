# Public macros for the TeX Live (TL) tree.
# Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
#
# This file is free software; the copyright holder
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 0

# KPSE_LIBJPEG_FLAGS
# -----------------
# Provide the configure options '--with-system-libjpeg' (if in the TL tree).
#
# Set the make variables LIBJPEG_INCLUDES and LIBJPEG_LIBS to the CPPFLAGS and
# LIBS required for the `-ljpeg' library in libs/libjpeg/ of the TL tree.
AC_DEFUN([KPSE_LIBJPEG_FLAGS], [dnl
_KPSE_LIB_FLAGS([libjpeg], [jpeg], [],
                [-IBLD/libs/libjpeg/include], [BLD/libs/libjpeg/libjpeg.a], [],
                [], [${top_builddir}/../../libs/libjpeg/include/jpeglib.h])[]dnl
]) # KPSE_LIBJPEG_FLAGS

# KPSE_LIBJPEG_OPTIONS([WITH-SYSTEM])
# ----------------------------------
AC_DEFUN([KPSE_LIBJPEG_OPTIONS], [_KPSE_LIB_OPTIONS([libjpeg], [$1], [pkg-config])])

# KPSE_LIBJPEG_SYSTEM_FLAGS
# ------------------------
AC_DEFUN([KPSE_LIBJPEG_SYSTEM_FLAGS], [dnl
_KPSE_PKG_CONFIG_FLAGS([libjpeg], [libjpeg])])
