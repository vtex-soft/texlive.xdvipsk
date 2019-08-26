# Public macros for the TeX Live (TL) tree.
# Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
#
# This file is free software; the copyright holder
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 0

# KPSE_LIBTIFF_FLAGS
# -----------------
# Provide the configure options '--with-system-libtiff' (if in the TL tree).
#
# Set the make variables LIBTIFF_INCLUDES and LIBTIFF_LIBS to the CPPFLAGS and
# LIBS required for the `-ltiff' library in libs/libtiff/ of the TL tree.
AC_DEFUN([KPSE_LIBTIFF_FLAGS], [dnl
_KPSE_LIB_FLAGS([libtiff], [tiff], [],
                [-IBLD/libs/libtiff/include], [BLD/libs/libtiff/libtiff.a], [],
                [], [${top_builddir}/../../libs/libtiff/libtiff-src/tiff.h])[]dnl
]) # KPSE_LIBTIFF_FLAGS

# KPSE_LIBTIFF_OPTIONS([WITH-SYSTEM])
# ----------------------------------
AC_DEFUN([KPSE_LIBTIFF_OPTIONS], [_KPSE_LIB_OPTIONS([libtiff], [$1], [pkg-config])])

# KPSE_LIBTIFF_SYSTEM_FLAGS
# ------------------------
AC_DEFUN([KPSE_LIBTIFF_SYSTEM_FLAGS], [dnl
_KPSE_PKG_CONFIG_FLAGS([libtiff], [libtiff])])
