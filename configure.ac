dnl configure.ac
dnl
dnl Copyright (c) 2003 Benedikt Meurer <benedikt.meurer@unix-ag.uni-siegen.de>
dnl

AC_INIT([libxfce4util])

AM_INIT_AUTOMAKE([libxfce4util], [3.92.0])

AM_CONFIG_HEADER([config.h])

AM_MAINTAINER_MODE

dnl libtool versioning
LIBXFCE4UTIL_VERINFO=0:5:0
AC_SUBST([LIBXFCE4UTIL_VERINFO])

dnl check for UNIX variants
AC_AIX
AC_ISC_POSIX
AC_MINIX

dnl check for basic programs
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LIBTOOL

dnl check for standard header files
AC_HEADER_STDC
AC_CHECK_HEADERS([err.h limits.h locale.h sys/wait.h])
AC_CHECK_FUNCS([__unaliasname setlocale strlcat strlcpy])

dnl Check for Glib >= 2.0 installed
BM_DEPEND([GLIB], [glib-2.0], [2.0.0])

dnl Check for debugging support
BM_DEBUG_SUPPORT

AC_OUTPUT([
Makefile
i18n/Makefile
libxfce4util.spec
libxfce4util/libxfce4util-1.0.pc
libxfce4util/Makefile
m4/Makefile
])
