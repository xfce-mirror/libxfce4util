SUBDIRS =								\
	libxfce4util							\
	docs								\
	m4								\
	xfce4-kiosk-query

distclean-local:
	rm -rf libxfce4util.spec *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

EXTRA_DIST =								\
	AUTHORS								\
	COPYING								\
	COPYING.BSD							\
	COPYING.LGPL							\
	INSTALL								\
	NEWS								\
	README								\
	README.Kiosk							\
	TODO								\
	gtk-doc.make							\
	libxfce4util.spec

ACLOCAL_AMFLAGS = -I m4

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc
