# Public macros for the TeX Live (TL) tree.
# Copyright (C) 2009-2015 Peter Breitenlohner <tex-live@tug.org>
#
# This file is free software; the copyright holder
# gives unlimited permission to copy and/or distribute it,
# with or without modifications, as long as this notice is preserved.

# serial 0

# KPSE_LIBMUPDF_FLAGS
# -----------------
# Provide the configure options '--with-system-libmupdf' (if in the TL tree).
#
# Set the make variables LIBMUPDF_INCLUDES and LIBMUPDF_LIBS to the CPPFLAGS and
# LIBS required for the `-lmupdf' library in libs/libmupdf/ of the TL tree.
AC_DEFUN([KPSE_LIBMUPDF_FLAGS], [dnl
_KPSE_LIB_FLAGS([libmupdf], [mupdf], [],
                [-IBLD/libs/libmupdf/include], [BLD/libs/libmupdf/libmupdf.a], [],
                [], [${top_builddir}/../../libs/libmupdf/include/mupdf/pdf.h])[]dnl
]) # KPSE_LIBMUPDF_FLAGS

# KPSE_LIBMUPDF_OPTIONS([WITH-SYSTEM])
# ----------------------------------
AC_DEFUN([KPSE_LIBMUPDF_OPTIONS], [_KPSE_LIB_OPTIONS([libmupdf], [$1], [pkg-config])])

# KPSE_LIBMUPDF_SYSTEM_FLAGS
# ------------------------
AC_DEFUN([KPSE_LIBMUPDF_SYSTEM_FLAGS], [dnl
_KPSE_PKG_CONFIG_FLAGS([libmupdf], [libmupdf])])
