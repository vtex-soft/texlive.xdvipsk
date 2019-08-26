dnl Process this file with autoconf to produce a configure script.
dnl
dnl   Copyright (C) 2009-2014 Peter Breitenlohner <tex-live@tug.org>
dnl
dnl   This file is free software; the copyright holder
dnl   gives unlimited permission to copy and/or distribute it,
dnl   with or without modifications, as long as this notice is preserved.
dnl
AC_INIT([make2unc], [1.0.0], [vtex@vtex.lt])
AC_PREREQ([2.65])
AC_CONFIG_SRCDIR([src/make2unc.cpp])
AC_CONFIG_AUX_DIR([../../build-aux])
AC_CONFIG_MACRO_DIR([../../m4])

KPSE_BASIC([make2unc])

AC_CONFIG_HEADERS([config.h])[]dnl unused, to avoid zillions of -D's on command line

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_RANLIB
AM_PROG_AR

if test "x$enable_build" != xno || test -f config.force; then

AC_CHECK_FUNC([strstr], ,
              [AC_MSG_ERROR([Sorry, your system has no strstr function.])])

echo timestamp >config.force
fi

KPSE_ICU_FLAGS

AC_CANONICAL_HOST
ARCH_LIBS_DIR="win32"
AS_CASE([$host_os],
        [linux*],
        [
           AS_CASE([$host_cpu],
                   [x86_64], [ARCH_LIBS_DIR="x86_64-linux"],
                   [i?86], [ARCH_LIBS_DIR="i386-linux"])
        ],
        [darwin*],
        [
           AS_CASE([$host_cpu],
                   [x86_64], [ARCH_LIBS_DIR="x86_64-darwin"],
                   [i?86], [ARCH_LIBS_DIR="universal-darwin"])
        ],
)
AC_SUBST([ARCH_LIBS_DIR])

AC_CONFIG_FILES([Makefile])

AC_OUTPUT
