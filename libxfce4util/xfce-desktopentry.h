/* $Id$ */
/*-
 * Copyright (c) 2004 Jasper Huijsmans <jasper@xfce.org>
 * Copyright (c) 2006 Benedikt Meurer <benny@xfce.org>
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

#ifndef __XFCE_DESKTOPENTRY_H__
#define __XFCE_DESKTOPENTRY_H__

#if defined(LIBXFCE4UTIL_COMPILATION) || !defined(XFCE_DISABLE_DEPRECATED)

#include <glib-object.h>

G_BEGIN_DECLS

typedef struct _XfceDesktopEntryPrivate XfceDesktopEntryPrivate;
typedef struct _XfceDesktopEntryClass   XfceDesktopEntryClass;
typedef struct _XfceDesktopEntry        XfceDesktopEntry;

#define XFCE_TYPE_DESKTOP_ENTRY            (xfce_desktop_entry_get_type ())
#define XFCE_DESKTOP_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntry))
#define XFCE_DESKTOP_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))
#define XFCE_IS_DESKTOP_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_IS_DESKTOP_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_DESKTOP_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))

struct _XfceDesktopEntry
{
  /*< private >*/
  GObject                  __parent__;
  XfceDesktopEntryPrivate *priv;
};

struct _XfceDesktopEntryClass
{
  /*< private >*/
  GObjectClass __parent__;

  /* Padding for future expansion */
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
};

GType                  xfce_desktop_entry_get_type              (void) G_GNUC_CONST;

XfceDesktopEntry      *xfce_desktop_entry_new                   (const gchar      *file,
                                                                 const gchar     **categories,
                                                                 gint              num_categories) G_GNUC_MALLOC;
XfceDesktopEntry      *xfce_desktop_entry_new_from_data         (const gchar      *data,
                                                                 const gchar     **categories,
                                                                 gint              num_categories) G_GNUC_MALLOC;

G_CONST_RETURN gchar  *xfce_desktop_entry_get_file              (XfceDesktopEntry *desktop_entry);

gboolean               xfce_desktop_entry_get_int               (XfceDesktopEntry *desktop_entry,
                                                                 const gchar      *key,
                                                                 gint             *value_return);
gboolean               xfce_desktop_entry_get_string            (XfceDesktopEntry *desktop_entry,
                                                                 const gchar      *key,
                                                                 gboolean          translated,
                                                                 gchar           **value_return);

gboolean               xfce_desktop_entry_has_translated_entry  (XfceDesktopEntry *desktop_entry,
                                                                 const gchar      *key);

G_END_DECLS

#endif /* !XFCE_DISABLE_DEPRECATED */

#endif /* !__XFCE_DESKTOPENTRY_H__ */

