SUBDIRS =								\
	libxfce4util							\
	docs								\
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
	ChangeLog							\
	ChangeLog.pre-4.2.0						\
	INSTALL								\
	NEWS								\
	README								\
	README.Kiosk							\
	TODO								\
	gtk-doc.make							\
	libxfce4util.spec

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc

dist-bz2: dist
	zcat $(PACKAGE)-$(VERSION).tar.gz | \
	 bzip2 --best -c > $(PACKAGE)-$(VERSION).tar.bz2

distcheck-bz2: distcheck
	zcat $(PACKAGE)-$(VERSION).tar.gz | \
	 bzip2 --best -c > $(PACKAGE)-$(VERSION).tar.bz2
