
lib_LTLIBRARIES = libxfce4util.la

libxfce4utilincludedir = $(includedir)/xfce4/libxfce4util

libxfce4utilinclude_HEADERS =					\
	debug.h							\
	i18n.h							\
	libxfce4util.h						\
	utf8.h							\
	util.h							\
	xdg.h							\
	xfce-desktopentry.h					\
	xfce_desktop_entry.h

libxfce4util_la_SOURCES =					\
	debug.h							\
	i18n.c							\
	i18n.h							\
	licenses-texts.c					\
	utf8.c							\
	utf8.h							\
	util.c							\
	util.h							\
	xdg.c							\
	xdg.h							\
	xfce-desktopentry.c					\
	xfce-desktopentry.h

libxfce4util_la_LDFLAGS =					\
	-export-dynamic						\
	-version-info @LIBXFCE4UTIL_VERINFO@			\
	-export-symbols-regex "^[^_].*"

libxfce4util_la_LIBADD =					\
	@GOBJECT_LIBS@

libxfce4util_la_CFLAGS = 					\
	-I$(top_srcdir)						\
	-DDEFAULT_XDG_DATA_DIRS=\"$(datadir):/usr/local/share:/usr/share\" \
	-DDEFAULT_XDG_CONFIG_DIRS=\"$(sysconfdir)/xfce4:/etc/xdg\" \
	-DNLS_ALIAS_DB=\"$(datadir)/xfce4/i18n/nls.alias\"	\
	@GOBJECT_CFLAGS@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxfce4util-1.0.pc

licenses-texts.c: licenses-to-c.sh BSD GPL LGPL
	sh licenses-to-c.sh BSD:BSD GPL:GPL LGPL:LGPL > $@

EXTRA_DIST =							\
	BSD							\
	GPL							\
	LGPL							\
	licenses-to-c.sh
