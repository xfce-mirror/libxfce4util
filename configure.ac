dnl $Id$
dnl
dnl Copyright (c) 2002-2004
dnl         The Xfce development team. All rights reserved.
dnl
dnl Written for Xfce by Benedikt Meurer <benny@xfce.org>, Jasper
dnl Huijsman <jasper@xfce.org> and Olivier Fourdan <fourdan@xfce.org>.
dnl

dnl Version information
m4_define([libxfce4util_verinfo], [1:4:0])
m4_define([libxfce4util_version_major], [4])
m4_define([libxfce4util_version_minor], [1])
m4_define([libxfce4util_version_micro], [15])
m4_define([libxfce4util_version], [libxfce4util_version_major.libxfce4util_version_minor.libxfce4util_version_micro])

dnl Overall XFce version, modify this when making releases
m4_define([xfce_version_string], [4.1.3])


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

dnl Subst overall XFce version
AC_DEFINE([XFCE_VERSION_STRING], ["xfce_version_string"],
  [Overall XFce version])

dnl Subst libxfce4util version
LIBXFCE4UTIL_VERSION_MAJOR=libxfce4util_version_major
LIBXFCE4UTIL_VERSION_MINOR=libxfce4util_version_minor
LIBXFCE4UTIL_VERSION_MICRO=libxfce4util_version_micro
AC_SUBST([LIBXFCE4UTIL_VERSION_MAJOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MINOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MICRO])

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
AC_CHECK_HEADERS([err.h errno.h grp.h limits.h locale.h pwd.h \
                  sys/stat.h sys/types.h sys/utsname.h \
                  sys/wait.h time.h unistd.h])
AC_CHECK_FUNCS([gethostname getpwnam setenv setlocale strdup unsetenv])

dnl Check for GObject >= 2.2.0 installed (GObject in turn depends on GLib)
BM_DEPEND([GOBJECT], [gobject-2.0], [2.2.0])

dnl Check for gtk-doc
GTK_DOC_CHECK([1.0])

dnl Check for debugging support
BM_DEBUG_SUPPORT

# Check for a broken putenv(3) implementation that requires strings
# to be malloc'd and not freed after a successfully call to putenv(3).
# Solaris is known to have such an implementation, also glibc seems to
# be hit by this problem.
AC_MSG_CHECKING([whether putenv() implementation is broken])
AC_ARG_WITH([broken-putenv],
  [  --with-broken-putenv=[[auto/yes/no]] broken putenv(3) [default=auto]])
if test x"$with_broken_putenv" != x"yes" -a x"$with_broken_putenv" != x"no"; then
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
  [with_broken_putenv=no],
  [with_broken_putenv=yes])
fi
if test x"$with_broken_putenv" = x"yes"; then
  AC_DEFINE([HAVE_BROKEN_PUTENV], [1], [Define if putenv(3) is broken])
fi
AC_MSG_RESULT([$with_broken_putenv])

AC_OUTPUT([
Makefile
docs/version.xml
docs/Makefile
libxfce4util.spec
libxfce4util/libxfce4util-1.0.pc
libxfce4util/libxfce4util-config.h
libxfce4util/Makefile
m4/Makefile
xfce4-kiosk-query/Makefile
])
