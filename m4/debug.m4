dnl From Benedikt Meurer (benedikt.meurer@unix-ag.uni-siegen.de)
dnl
dnl if debug support is requested:
dnl
dnl   1) defines DEBUG to 1
dnl   2) sets DEBUG_CFLAGS to what debug level is requested
dnl

AC_DEFUN([AM_DEBUG_SUPPORT],
[
  AC_ARG_ENABLE(debug,
AC_HELP_STRING([--enable-debug[=yes|no|full]], [Build with debugging support])
AC_HELP_STRING([--disable-debug], [Include no debugging support [default]]),
    [ac_cv_debug=$enableval], [ac_cv_debug=no])
  AC_MSG_CHECKING([whether to build with debugging support])
  DEBUG_CFLAGS=
  if test "x$ac_cv_debug" != "xno"; then
    AC_DEFINE(DEBUG, 1, Define for debugging support)
    if test "x$ac_cv_debug" == "xfull"; then
      DEBUG_CFLAGS="-g3"
      AC_MSG_RESULT([full])
    else
      DEBUG_CFLAGS="-g"
      AC_MSG_RESULT([yes])
    fi
  else
    AC_MSG_RESULT([no])
  fi
])
