INCLUDES =								\
	-DDEFAULT_XDG_CACHE_HOME=\"~/.cache\"				\
	-DDEFAULT_XDG_DATA_HOME=\"~/.local/share\"			\
	-DDEFAULT_XDG_CONFIG_HOME=\"~/.config\"				\
	-DDEFAULT_XDG_DATA_DIRS=\"$(datadir):/usr/local/share:/usr/share\" \
	-DDEFAULT_XDG_CONFIG_DIRS=\"$(sysconfdir)/xdg:$(sysconfdir)/xfce4:/etc/xdg\" \
	-DDATADIR=\"$(datadir)\"					\
	-DSYSCONFDIR=\"$(sysconfdir)\"					\
	-DG_LOG_DOMAIN=\"libxfce4util\"					\
	-DKIOSKDIR=\"$(sysconfdir)/xdg/xfce4/kiosk\"			\
	-DKIOSKDEF=\"ALL\"

lib_LTLIBRARIES = libxfce4util.la

libxfce4utilincludedir = $(includedir)/xfce4/libxfce4util

libxfce4utilinclude_HEADERS =						\
	debug.h								\
	i18n.h								\
	libxfce4util.h							\
	libxfce4util-config.h						\
	utf8.h								\
	util.h								\
	xfce-desktopentry.h						\
	xfce-fileutils.h						\
	xfce-generics.h							\
	xfce-kiosk.h							\
	xfce-miscutils.h						\
	xfce-rc.h							\
	xfce-resource.h

libxfce4util_la_SOURCES =						\
	debug.h								\
	i18n.c								\
	i18n.h								\
	licenses-texts.c						\
	utf8.c								\
	utf8.h								\
	xfce-desktopentry.c						\
	xfce-desktopentry.h						\
	xfce-fileutils.c						\
	xfce-fileutils.h						\
	xfce-generics.h							\
	xfce-kiosk.c							\
	xfce-kiosk.h							\
	xfce-miscutils.c						\
	xfce-miscutils.h						\
	xfce-rc.c							\
	xfce-rc.h							\
	xfce-rc-config.c						\
	xfce-rc-private.h						\
	xfce-rc-simple.c						\
	xfce-resource.c							\
	xfce-resource.h

libxfce4util_la_LDFLAGS =					\
	-export-dynamic						\
	-version-info @LIBXFCE4UTIL_VERINFO@			\
	-export-symbols-regex "^[^_].*"

if HAVE_CYGWIN
libxfce4util_la_LDFLAGS += -no-undefined
endif

libxfce4util_la_LIBADD =					\
	@GOBJECT_LIBS@

libxfce4util_la_CFLAGS = 					\
	-I$(top_srcdir)						\
	@GOBJECT_CFLAGS@

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libxfce4util-1.0.pc

licenses-texts.c: licenses-to-c.sh BSD GPL LGPL
	sh licenses-to-c.sh BSD:BSD GPL:GPL LGPL:LGPL > $@

# required for gtk-doc
dist-hook: all

EXTRA_DIST =							\
	BSD							\
	GPL							\
	LGPL							\
	libxfce4util-config.h.in				\
	licenses-to-c.sh
