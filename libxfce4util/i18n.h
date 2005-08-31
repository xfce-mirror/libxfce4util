/* $Id$ */
/*-
 * Copyright (c) 2003-2005 Benedikt Meurer <benny@xfce.org>
 * Copyright (c) 2004 Jasper Huijsman <jasper@xfce.org>
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#if !defined(LIBXFCE4UTIL_INSIDE_LIBXFCE4UTIL_H) && !defined(LIBXFCE4UTIL_COMPILATION)
#error "Only <libxfce4util/libxfce4util.h> can be included directly, this file may disappear or change contents"
#endif

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
