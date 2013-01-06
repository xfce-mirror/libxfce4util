# $Id$

SUBDIRS =								\
	libxfce4util							\
	docs								\
	po								\
	xfce4-kiosk-query

distclean-local:
	rm -rf *.cache *~

.PHONY: ChangeLog

ChangeLog: Makefile
	(GIT_DIR=$(top_srcdir)/.git git log > .changelog.tmp \
	&& mv .changelog.tmp ChangeLog; rm -f .changelog.tmp) \
	|| (touch ChangeLog; echo 'Git directory not found: installing possibly empty changelog.' >&2)

dist-hook: ChangeLog

EXTRA_DIST =								\
	README.Kiosk							\
	THANKS								\
	TODO								\
	gtk-doc.make							\
	intltool-extract.in						\
	intltool-merge.in						\
	intltool-update.in

DISTCLEANFILES =							\
	intltool-extract						\
	intltool-merge							\
	intltool-update	

DISTCHECK_CONFIGURE_FLAGS =						\
	--enable-gtk-doc

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
