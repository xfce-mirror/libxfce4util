SUBDIRS =								\
	docs								\
	libxfce4util							\
	m4								\
	xfce4-kiosk-query

EXTRA_DIST =								\
	AUTHORS								\
	COPYING								\
	COPYING.BSD							\
	COPYING.LGPL							\
	libxfce4util.spec						\
	NEWS								\
	README								\
	TODO

distclean-local:
	rm -rf libxfce4util.spec *.cache *~

rpm: dist
	rpmbuild -ta $(PACKAGE)-$(VERSION).tar.gz
	@rm -f $(PACKAGE)-$(VERSION).tar.gz

ACLOCAL_AMFLAGS = -I m4
