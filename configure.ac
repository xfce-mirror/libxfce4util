dnl
dnl Copyright (c) 2002-2024
dnl         The Xfce development team. All rights reserved.
dnl
dnl Written for Xfce by Benedikt Meurer <benny@xfce.org>, Jasper
dnl Huijsmans <jasper@xfce.org> and Olivier Fourdan <fourdan@xfce.org>.
dnl

dnl ***************************
dnl *** Version information ***
dnl ***************************
m4_define([libxfce4util_verinfo], [7:0:0])
m4_define([copyright_year], [2025])
XDT_VERSION_INIT([4.20.1], [git])

dnl minimum required versions
m4_define([glib_min_version], [2.72.0])

dnl **************************************************************
dnl *** Overall Xfce version, modify this when making releases ***
dnl **************************************************************
m4_define([xfce_version_string], [4.20])

dnl # DO NOT MODIFY ANYTHING BELOW THIS LINE, UNLESS YOU KNOW WHAT
dnl # YOU ARE DOING.

dnl ***************************
dnl *** Initialize autoconf ***
dnl ***************************
AC_COPYRIGHT([Copyright (c) 2002-copyright_year()
        The Xfce development team. All rights reserved.])
AC_INIT([libxfce4util], [xdt_version], [https://gitlab.xfce.org/xfce/libxfce4util])
AC_PREREQ([2.69])
AC_REVISION([xdt_version_build])
AC_DEFINE([VERSION_FULL], [PACKAGE_VERSION], [Alias for VERSION and PACKAGE_VERSION for meson compatibility])
COPYRIGHT_YEAR=copyright_year()
AC_DEFINE_UNQUOTED([COPYRIGHT_YEAR], ["$COPYRIGHT_YEAR"], [Copyright year])
AC_SUBST([COPYRIGHT_YEAR])

dnl ***************************
dnl *** Initialize automake ***
dnl ***************************
LIBXFCE4UTIL_VERSION=xdt_version()
AM_INIT_AUTOMAKE([1.8 dist-bzip2 tar-ustar no-dist-gzip foreign])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([m4])
AM_MAINTAINER_MODE()
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

dnl **************************
dnl *** Libtool versioning ***
dnl **************************
LIBXFCE4UTIL_VERINFO=libxfce4util_verinfo()
AC_SUBST([LIBXFCE4UTIL_VERINFO])

dnl **********************************
dnl *** Subst overall Xfce version ***
dnl **********************************
AC_DEFINE([VERSION_SHORT], ["xfce_version_string"],
  [Overall Xfce version])

dnl **********************************
dnl *** Subst libxfce4util version ***
dnl **********************************
LIBXFCE4UTIL_VERSION_MAJOR=xdt_version_major()
LIBXFCE4UTIL_VERSION_MINOR=xdt_version_minor()
LIBXFCE4UTIL_VERSION_MICRO=xdt_version_micro()
AC_SUBST([LIBXFCE4UTIL_VERSION_MAJOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MINOR])
AC_SUBST([LIBXFCE4UTIL_VERSION_MICRO])

dnl *******************************
dnl *** Check for UNIX variants ***
dnl *******************************
AC_USE_SYSTEM_EXTENSIONS

dnl ********************************
dnl *** Check for basic programs ***
dnl ********************************
AC_PROG_CC()
AM_PROG_CC_C_O()
AC_PROG_INSTALL()
AC_PATH_PROG([XDT_GEN_VISIBILITY], [xdt-gen-visibility], [], [$PWD:$PATH])
if test x"$XDT_GEN_VISIBILITY" = x"$PWD/xdt-gen-visibility"; then
  XDT_GEN_VISIBILITY_DIST="xdt-gen-visibility"
else
  XDT_GEN_VISIBILITY_DIST=""
fi
AC_SUBST([XDT_GEN_VISIBILITY_DIST])

dnl **************************
dnl *** Initialize libtool ***
dnl **************************
LT_PREREQ([2.2.6])
LT_INIT([disable-static])

dnl ***************************************
dnl *** Check for standard header files ***
dnl ***************************************
AC_CHECK_HEADERS([err.h errno.h grp.h limits.h locale.h pwd.h \
                  signal.h sys/stat.h sys/types.h sys/utsname.h \
                  time.h unistd.h stdarg.h varargs.h libintl.h])
AC_CHECK_FUNCS([gethostname getpwnam setlocale])

dnl ******************************
dnl *** Check for i18n support ***
dnl ******************************
GETTEXT_PACKAGE="$PACKAGE"
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"], [Name of default gettext domain])
AC_SUBST([GETTEXT_PACKAGE])
AM_GNU_GETTEXT([external])
AM_GNU_GETTEXT_VERSION([0.19.8])
AC_SEARCH_LIBS([bind_textdomain_codeset], [intl],
  [AC_DEFINE([HAVE_BIND_TEXTDOMAIN_CODESET], [1], [Define to 1 if you have the 'bind_textdomain_codeset' function.])],
  [])

dnl ***********************************
dnl *** Check for GObject installed ***
dnl ***********************************
XDT_CHECK_PACKAGE([GOBJECT], [gobject-2.0], [glib_min_version])

dnl ********************************
dnl *** Check for GLib installed ***
dnl ********************************
XDT_CHECK_PACKAGE([GLIB], [glib-2.0], [glib_min_version])

dnl ********************************
dnl *** Check for GIO installed ***
dnl ********************************
XDT_CHECK_PACKAGE([GIO], [gio-2.0], [glib_min_version])

dnl *************************
dnl *** Check for gtk-doc ***
dnl *************************
GTK_DOC_CHECK([1.20])

dnl ***************************************
dnl *** Check for gobject-introspection ***
dnl ***************************************
GOBJECT_INTROSPECTION_CHECK([1.72.0])

dnl *************************
dnl *** Check for vapigen ***
dnl *************************
VAPIGEN_CHECK()

dnl ***********************************
dnl *** Check for debugging support ***
dnl ***********************************
XDT_FEATURE_DEBUG([xdt_debug_default])

dnl **************************************
dnl *** Check for linker optimizations ***
dnl **************************************
XDT_FEATURE_LINKER_OPTS()

dnl *****************************************
dnl *** Check for ELF visibility support  ***
dnl *****************************************
dnl *** Do not replace with the Xdt macro ***
dnl *** because it cannot cope with the   ***
dnl *** libxfce4util_*_version variables  ***
dnl *****************************************
AC_ARG_ENABLE([visibility],
              AS_HELP_STRING([--disable-visibility],
                             [Do not use ELF visibility attributes]),
              [enable_visibility=$enableval], [enable_visibility=yes])
have_gnuc_visibility=no
if test "x$enable_visibility" != "xno"; then
  XDT_SUPPORTED_FLAGS([xdt_vis_test_cflags], [-Wall -Werror -Wno-unused-parameter -fvisibility=hidden])
  saved_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $xdt_vis_test_cflags"
  AC_MSG_CHECKING([whether $CC supports the GNUC visibility attribute])
  AC_COMPILE_IFELSE([AC_LANG_SOURCE(
  [
    void test_default(void);
    void test_hidden(void);
    void __attribute__((visibility("default"))) test_default(void) {}
    void __attribute__((visibility("hidden"))) test_hidden(void) {}
    int main(int argc, char **argv) {
      test_default();
      test_hidden();
      return 0;
    }
  ])],
  [
    have_gnuc_visibility=yes
    AC_MSG_RESULT([yes])
  ],
  [
    AC_MSG_RESULT([no])
  ])
  CFLAGS="$saved_CFLAGS"
fi
if test "x$have_gnuc_visibility" = "xyes"; then
  CPPFLAGS="$CPPFLAGS -DENABLE_SYMBOL_VISIBILITY=1"
  CFLAGS="$CFLAGS -fvisibility=hidden"
fi

dnl *****************************
dnl *** Generate output files ***
dnl *****************************
AC_CONFIG_FILES([
Makefile
docs/Makefile
docs/reference/Makefile
libxfce4util/libxfce4util-1.0.pc
libxfce4util/libxfce4util-config.h
libxfce4util/Makefile
po/Makefile.in
xfce4-kiosk-query/Makefile
])
AC_OUTPUT

dnl ***************************
dnl *** Print configuration ***
dnl ***************************
echo
echo "Build Configuration:"
echo
echo "* Debug Support:  $enable_debug"
echo "* GNU Visibility: $have_gnuc_visibility"
echo
