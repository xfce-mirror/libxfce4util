/* $Id$ */
/*-
 * Copyright (C) 2004 Jasper Huijsmans <jasper@xfce.org>
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

#ifndef __LIBXFCE4UTIL_XFCE_DESKTOPENTRY_H__
#define __LIBXFCE4UTIL_XFCE_DESKTOPENTRY_H__

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define XFCE_TYPE_DESKTOP_ENTRY            (xfce_desktop_entry_get_type ())
#define XFCE_DESKTOP_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntry))
#define XFCE_DESKTOP_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))
#define XFCE_IS_DESKTOP_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_IS_DESKTOP_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_DESKTOP_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))

typedef struct _XfceDesktopEntry XfceDesktopEntry;
typedef struct _XfceDesktopEntryClass XfceDesktopEntryClass;
typedef struct _XfceDesktopEntryPrivate XfceDesktopEntryPrivate;

struct _XfceDesktopEntry
{
    GObject parent;

    /*< private >*/
    XfceDesktopEntryPrivate *priv;
};

struct _XfceDesktopEntryClass
{
    GObjectClass parent_class;

    /* Padding for future expansion */
    void (*_xfce_reserved1) (void);
    void (*_xfce_reserved2) (void);
    void (*_xfce_reserved3) (void);
};

/* XXX - Remove this ASAP */
#if TESTING
void print_desktop_entry_info (XfceDesktopEntry *desktop_entry);
#endif /* TESTING */

GType xfce_desktop_entry_get_type (void) G_GNUC_CONST;

G_CONST_RETURN 
char *xfce_desktop_entry_get_file (XfceDesktopEntry * desktop_entry);

XfceDesktopEntry *xfce_desktop_entry_new_from_data (const char *data,
						    const char **categories,
						    int num_categories);
XfceDesktopEntry *xfce_desktop_entry_new (const char *file, 
    					  const char **categories,
					  int num_categories);

gboolean xfce_desktop_entry_parse (XfceDesktopEntry *desktop_entry);

gboolean xfce_desktop_entry_get_string (XfceDesktopEntry * desktop_entry,
    				        const char *key,
					gboolean translated,
					char **value);

gboolean xfce_desktop_entry_get_int (XfceDesktopEntry * desktop_entry,
    				     const char *key,
				     int *value);

G_END_DECLS

#endif  /* !__LIBXFCE4UTIL_XFCE_DESKTOPENTRY_H__ */

