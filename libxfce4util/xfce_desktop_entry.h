/* xfce4
 *  
 * Copyright (C) 2004 Jasper Huijsmans (jasper@xfce.org)
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
 * 
*/

/**
 * @file libxfce4util/xfce_desktop_entry.h GObject wrapper to handle .desktop
 * files.
 * 
 * @author Jasper Huijsmans <jasper@xfce.org>
 */

#ifndef _XFCE_DESKTOP_ENTRY_H
#define _XFCE_DESKTOP_ENTRY_H

#include <glib.h>
#include <glib-object.h>

/**
 * @brief Set to 1 for debugging purposes.
 */
#define TESTING 0

G_BEGIN_DECLS

/** @{ */
/** 
 * Common GObject macro
 */
#define XFCE_TYPE_DESKTOP_ENTRY            (xfce_desktop_entry_get_type ())
#define XFCE_DESKTOP_ENTRY(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntry))
#define XFCE_DESKTOP_ENTRY_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))
#define XFCE_IS_DESKTOP_ENTRY(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_IS_DESKTOP_ENTRY_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), XFCE_TYPE_DESKTOP_ENTRY))
#define XFCE_DESKTOP_ENTRY_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryClass))
/** @} */

/**
 * @defgroup dentry Handling .desktop files
 * @{
 */

/**
 * @brief GObject that wraps a .desktop file.
 *
 * XfceDesktopEntry is a wrapper for a so-called desktop entry file. 
 * It contains only private data. The desktop entry format is defined on
 * http://freedesktop.org .
 */
typedef struct _XfceDesktopEntry XfceDesktopEntry;

/**
 * @brief Class for XfceDesktopEntry.
 */
typedef struct _XfceDesktopEntryClass XfceDesktopEntryClass;

/**
 * @brief Private data of XfceDesktopEntry
 */
typedef struct _XfceDesktopEntryPrivate XfceDesktopEntryPrivate;

struct _XfceDesktopEntry
{
    GObject parent;

    /* < private > */
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

#if TESTING
/**
 * @brief print contents of XfceDesktopEntry object.
 *
 * Meant for debugging only.
 */
void print_desktop_entry_info (XfceDesktopEntry *desktop_entry);
#endif /* TESTING */

/**
 * @brief GObject creation function.
 */
GType xfce_desktop_entry_get_type (void) G_GNUC_CONST;

/**
 * @brief Obtain the path to the desktop entry file associated with the 
 *        XfceDesktopEntry.
 * 
 * @param desktop_entry : an XfceDesktopEntry
 *
 * @return Path of the desktop entry file used to create @p desktop_entry.
 *
 * The return value should be considered read-only and must not be freed.
 **/
G_CONST_RETURN 
char *xfce_desktop_entry_get_file (XfceDesktopEntry * desktop_entry);

/**
 * @brief Create new XfceDesktopEntry object.
 * 
 * @param file           : full path to the desktop entry file to use
 * @param categories     : array of categories (does not have to be NULL
 * 			   terminated)
 * @param num_categories : number of categories in the array
 *
 * @return Newly created XfceDesktopEntry or NULL if it fails.
 **/
XfceDesktopEntry *xfce_desktop_entry_new (const char *file, 
    					  const char **categories,
					  int num_categories);

/**
 * @brief Parse desktop entry file.
 * 
 * @param desktop_entry : an XfceDesktopEntry
 *
 * Parses the desktop entry file and fills in the values for all categories.
 *
 * @return TRUE on success, FALSE on failure.
 **/
gboolean xfce_desktop_entry_parse (XfceDesktopEntry *desktop_entry);

/**
 * @brief Get value from @p desktop_entry as string.
 * 
 * @param desktop_entry : an XfceDesktopEntry
 * @param key           : category to find value for
 * @param translated    : set to TRUE if the translated value is preferred
 * @param value         : location for the value, which will be newly allocated
 *
 * Finds the value for @p key. When @p translated is TRUE the function will use
 * the translated value (using the current locale settings) if available or
 * the untranslated value if no translation can be found.
 *
 * @return TRUE on success, FALSE on failure. @p value must be freed.
 **/
gboolean xfce_desktop_entry_get_string (XfceDesktopEntry * desktop_entry,
    				        const char *key,
					gboolean translated,
					char **value);

/**
 * @brief Get value from @p desktop_entry as integer.
 * 
 * @param desktop_entry : an XfceDesktopEntry
 * @param key           : category to find value for
 * @param value         : location for the value
 * 
 * Finds the value for @p key as an integer.
 *
 * @return TRUE on success, FALSE on failure.
 **/
gboolean xfce_desktop_entry_get_int (XfceDesktopEntry * desktop_entry,
    				     const char *key,
				     int *value);

G_END_DECLS

/** @} */
/* end group dentry */

#endif /* _XFCE_DESKTOP_ENTRY_H */
