/* $Id$ */
/*-
 * Copyright (c) 2003-2004 Benedikt Meurer <benny@xfce.org>
 * Copyright (c) 2004 Jasper Huijsman <jasper@xfce.org>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * TODO:
 *
 *	- turn xfce_textdomain() into a C function
 */

#ifndef __LIBXFCE4UTIL_I18N_H__
#define __LIBXFCE4UTIL_I18N_H__

#include <glib.h>

#if defined(ENABLE_NLS) && defined(GETTEXT_PACKAGE)

#if GLIB_CHECK_VERSION(2,4,0)

#include <glib/gi18n-lib.h>

#else

#include <libintl.h>

#ifdef _
#undef _
#endif
#define	_(s)                            (dgettext(GETTEXT_PACKAGE, s))

#ifdef gettext_noop
#ifdef N_
#undef N_
#endif
#define N_(s)                           (gettext_noop (s))
#else
#define N_(s)                           (s)
#endif

#ifdef Q_
#undef Q_
#endif
#define Q_(s)                           (xfce_strip_context ((s), gettext ((s))))

#define g_strip_context(msgid, msgval)  (xfce_strip_context ((msgid), (msgval)))

#endif /* !GLIB_CHECK_VERSION(2,4,0) */


#ifdef HAVE_BIND_TEXTDOMAIN_CODESET

#define	xfce_textdomain(package, localedir, encoding)			\
do {									\
	bindtextdomain(package, localedir);				\
	bind_textdomain_codeset(package, encoding);			\
	textdomain(package);						\
} while(0)

#else /* !defined(HAVE_BIND_TEXTDOMAIN_CODESET) */

#define	xfce_textdomain(package, localedir, encoding)			\
do {									\
	bindtextdomain(package, localedir);				\
	textdomain(package);						\
} while(0)

#endif

#else /* !defined(ENABLE_NLS) || !defined(GETTEXT_PACKAGE) */

#if defined(ENABLE_NLS)
#warning "National language support requested but GETTEXT_PACKAGE undefined"
#endif

#ifdef _
#undef _
#endif
#define _(s)	(s)

#ifdef N_
#undef N_
#endif
#define N_(s)	(s)

#ifdef Q_
#undef Q_
#endif
#define Q_(s)   (s)

#ifdef textdomain
#undef textdomain
#endif
#ifdef bindtextdomain
#undef bindtextdomain
#endif
#ifdef gettext
#undef gettext
#endif
#ifdef dgettext
#undef dgettext
#endif
#ifdef dcgettext
#undef dcgettext
#endif

#define textdomain(s)				(s)
#define gettext(s)				(s)
#define dgettext(domain,s)			(s)
#define dcgettext(domain,s,type)		(s)
#define bindtextdomain(domain,directory) \
do { \
  /* nothing to do here */ \
} while(0)

#define	xfce_textdomain(package, localedir, encoding)			\
do {									\
	/* nothing to do here */						\
} while(0)

#endif /* !defined(ENABLE_NLS) || !defined(GETTEXT_PACKAGE) */

G_CONST_RETURN gchar* xfce_strip_context        (const gchar *msgid,
						 const gchar *msgval) G_GNUC_PURE;

gchar*	              xfce_get_file_localized   (const gchar *filename) G_GNUC_PURE;
gchar*	              xfce_get_dir_localized    (const gchar *directory) G_GNUC_PURE;
gchar*	              xfce_get_file_localized_r (gchar       *buffer,
						 gsize        length,
						 const gchar *filename) G_GNUC_PURE;
gchar*	              xfce_get_dir_localized_r  (gchar       *buffer,
						 gsize        length,
						 const gchar *directory) G_GNUC_PURE;

gchar*	              xfce_get_path_localized (gchar       *dst,
					       gsize        size,
					       const gchar *paths,
					       const gchar *filename,
					       GFileTest    test);

#define XFCE_LOCALE_FULL_MATCH 50
#define XFCE_LOCALE_NO_MATCH    0

guint                 xfce_locale_match       (const gchar *locale1,
					       const gchar *locale2) G_GNUC_PURE;

#endif	/* !__LIBXFCE4UTIL_I18N_H__ */
