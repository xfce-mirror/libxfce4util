
lib_LTLIBRARIES = libxfce4util.la

libxfce4utilincludedir = $(includedir)/xfce4/libxfce4util

libxfce4utilinclude_HEADERS =					\
	debug.h							\
	filter.h						\
	i18n.h							\
	option.h						\
	util.h

libxfce4util_la_SOURCES =					\
	filter.c						\
	filter.h						\
	i18n.c							\
	i18n.h							\
	option.c						\
	option.h						\
	util.c							\
	util.h

libxfce4util_la_LDFLAGS =					\
	-export-dynamic						\
	-version-info @LIBXFCE4UTIL_VERINFO@			\
	-export-symbols-regex "^[^_].*"

libxfce4util_la_LIBADD =					\
	@GLIB_LIBS@

libxfce4util_la_CFLAGS = 					\
	-DNLS_ALIAS_DB=\"$(datadir)/xfce4/i18n/nls.alias\"	\
	@GLIB_CFLAGS@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxfce4util-1.0.pc

