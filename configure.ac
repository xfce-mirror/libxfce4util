dnl $Id$
dnl
dnl Copyright (c) 2002-2004
dnl         The Xfce development team. All rights reserved.
dnl
dnl Written for Xfce by Benedikt Meurer <benny@xfce.org>, Jasper
dnl Huijsman <jasper@xfce.org> and Olivier Fourdan <fourdan@xfce.org>.
dnl

dnl Version information
m4_define([libxfce4util_verinfo], [1:0:0])
m4_define([libxfce4util_version_major], [4])
m4_define([libxfce4util_version_minor], [0])
m4_define([libxfce4util_version_micro], [6])
m4_define([libxfce4util_version], [libxfce4util_version_major.libxfce4util_version_minor.libxfce4util_version_micro])
  
dnl # DO NOT MODIFY ANYTHING BELOW THIS LINE, UNLESS YOU KNOW WHAT
dnl # YOU ARE DOING.
                                                                              
                                                                              
dnl Initialize autoconf
AC_COPYRIGHT([Copyright (c) 2002-2004
        The Xfce development team. All rights reserved.
                                                                              
Written for Xfce by Benedikt Meurer <benny@xfce.org>, Jasper
Huijsman <jasper@xfce.org> and Olivier Fourdan <fourdan@xfce.org>.])
AC_INIT([libxfce4util], [libxfce4util_version], [xfce4-dev@xfce.org])
AC_PREREQ([2.50])
AC_REVISION([$Id$])
                                                                              
dnl Initialize automake
LIBXFCE4UTIL_VERSION=libxfce4util_version
AM_INIT_AUTOMAKE([libxfce4util], [$LIBXFCE4UTIL_VERSION])
AM_CONFIG_HEADER([config.h])
AM_MAINTAINER_MODE
                                                                              
dnl libtool versioning
LIBXFCE4UTIL_VERINFO=libxfce4util_verinfo
AC_SUBST([LIBXFCE4UTIL_VERINFO])
                                                                              
dnl Subst libxfce4util version
LIBXFCE4UTIL_VERSION_MAJOR=libxfce4util_version_major
LIBXFCE4UTIL_VERSION_MINOR=libxfce4util_version_minor
LIBXFCE4UTIL_VERSION_MICRO=libxfce4util_version_micro
AC_SUBST([LIBXFCE4UTIL_VERSION_MAJOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MINOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MICRO])
                                                                              
dnl libtool versioning
LIBXFCE4UTIL_VERINFO=1:0:0
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
libxfce4util/libxfce4util-config.h
libxfce4util/Makefile
m4/Makefile
])
