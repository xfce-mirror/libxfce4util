# $Id$

SUBDIRS =								\
	libxfce4util							\
	docs								\
	po								\
	xfce4-kiosk-query

AUTOMAKE_OPTIONS =							\
	1.8								\
	dist-bzip2

distclean-local:
	rm -rf libxfce4util.spec *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

EXTRA_DIST =								\
	ChangeLog.pre-4.2.0						\
	README.Kiosk							\
	THANKS								\
	TODO								\
	gtk-doc.make							\
	intltool-extract.in						\
	intltool-merge.in						\
	intltool-update.in						\
	libxfce4util.spec

DISTCLEANFILES =							\
	intltool-extract						\
	intltool-merge							\
	intltool-update	

DISTCHECK_CONFIGURE_FLAGS =						\
	--enable-gtk-doc

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
