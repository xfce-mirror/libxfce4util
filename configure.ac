dnl $Id$
dnl
dnl Copyright (c) 2003,2004
dnl         The XFce development team. All rights reserved.
dnl

dnl Version information
m4_define([libxfce4util_verinfo], [1:1:0])
m4_define([libxfce4util_version_major], [4])
m4_define([libxfce4util_version_minor], [1])
m4_define([libxfce4util_version_micro], [1])
m4_define([libxfce4util_version], [libxfce4util_version_major.libxfce4util_version_minor.libxfce4util_version_micro])

dnl Initialize autoconf
AC_COPYRIGHT([Copyright (c) 2003,2004
        The XFce development team. All rights reserved.])
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
AC_CHECK_HEADERS([err.h limits.h locale.h sys/utsname.h sys/wait.h])
AC_CHECK_FUNCS([__unaliasname gethostname setenv setlocale strlcat strlcpy])

dnl Check for GObject >= 2.0.0 installed (GObject in turn depends on GLib)
BM_DEPEND([GOBJECT], [gobject-2.0], [2.0.0])

dnl Check for debugging support
BM_DEBUG_SUPPORT

# Check for a broken putenv(3) implementation that requires strings
# to be malloc'd and not freed after a successfully call to putenv(3).
# Solaris is known to have such an implementation, also glibc seems to
# be hit by this problem.
AC_MSG_CHECKING([whether putenv() implementation is broken])
AC_RUN_IFELSE([
#include <stdlib.h>
#include <string.h>
int main(int argc, char *argv[])
{
  char *buffer = (char *)malloc(8);
  strcpy(buffer, "foo=bar");
  putenv(buffer);
  strcpy(buffer, "foo=rab");
  return(strcmp(getenv("foo"), "bar") == 0 ? 0 : 1);
}],
[AC_MSG_RESULT([no])],
[
  AC_DEFINE([HAVE_BROKEN_PUTENV], [1], [Define if putenv() is broken])
  AC_MSG_RESULT([yes])
])

AC_OUTPUT([
Makefile
i18n/Makefile
libxfce4util.spec
libxfce4util/libxfce4util-1.0.pc
libxfce4util/Makefile
m4/Makefile
])
