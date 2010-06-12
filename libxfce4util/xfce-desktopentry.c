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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif



#include <libxfce4util/libxfce4util.h>
#include <libxfce4util/libxfce4util-alias.h>



/* fallback for g_intern_static_string() with GLib < 2.9.0 */
#if !GLIB_CHECK_VERSION(2,9,0)
#define g_intern_static_string(string) (g_quark_to_string (g_quark_from_static_string ((string))))
#endif



#define XFCE_DESKTOP_ENTRY_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), XFCE_TYPE_DESKTOP_ENTRY, XfceDesktopEntryPrivate))



typedef struct _XfceDesktopEntryItem XfceDesktopEntryItem;



static void                        xfce_desktop_entry_class_init (XfceDesktopEntryClass *klass);
static void                        xfce_desktop_entry_init       (XfceDesktopEntry      *desktop_entry);
static void                        xfce_desktop_entry_finalize   (GObject               *object);
static const XfceDesktopEntryItem *xfce_desktop_entry_lookup     (XfceDesktopEntry      *desktop_entry,
                                                                     const gchar        *key);
static gboolean                    xfce_desktop_entry_parse      (XfceDesktopEntry      *desktop_entry);



struct _XfceDesktopEntryPrivate
{
  gchar                *file;
  gchar                *locale;
  gchar                *data;

  XfceDesktopEntryItem *items;
  gint                  num_items;
};

struct _XfceDesktopEntryItem
{
  gchar *key;
  gchar *value;
  gchar *section;
  gchar *translated_value;
};



static GObject *xfce_desktop_entry_parent_class;



GType
xfce_desktop_entry_get_type (void)
{
  static GType type = G_TYPE_INVALID;

  if (G_UNLIKELY (type == G_TYPE_INVALID))
    {
      static const GTypeInfo info =
      {
        sizeof (XfceDesktopEntryClass),
        NULL,
        NULL,
        (GClassInitFunc) xfce_desktop_entry_class_init,
        NULL, 
        NULL,
        sizeof (XfceDesktopEntry),
        0,
        (GInstanceInitFunc) xfce_desktop_entry_init,
        NULL,
      };

      type = g_type_register_static (G_TYPE_OBJECT, g_intern_static_string ("XfceDesktopEntry"), &info, 0);
    }

  return type;
}



static void
xfce_desktop_entry_class_init (XfceDesktopEntryClass * klass)
{
  GObjectClass *gobject_class;

  /* install private data for the type */
  g_type_class_add_private (klass, sizeof (XfceDesktopEntryPrivate));

  /* determine the parent type class */
  xfce_desktop_entry_parent_class = g_type_class_peek_parent (klass);

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = xfce_desktop_entry_finalize;
}



static void
xfce_desktop_entry_init (XfceDesktopEntry *desktop_entry)
{
  desktop_entry->priv = XFCE_DESKTOP_ENTRY_GET_PRIVATE (desktop_entry);
}



static void
xfce_desktop_entry_finalize (GObject *object)
{
  XfceDesktopEntry *desktop_entry = XFCE_DESKTOP_ENTRY (object);
  gint              n;

  /* release the items */
  for (n = 0; n < desktop_entry->priv->num_items; ++n)
    {
      g_free (desktop_entry->priv->items[n].translated_value);
      g_free (desktop_entry->priv->items[n].section);
      g_free (desktop_entry->priv->items[n].value);
      g_free (desktop_entry->priv->items[n].key);
    }

  g_free (desktop_entry->priv->items);
  g_free (desktop_entry->priv->file);
  g_free (desktop_entry->priv->data);

  (*G_OBJECT_CLASS (xfce_desktop_entry_parent_class)->finalize) (object);
}



static const XfceDesktopEntryItem*
xfce_desktop_entry_lookup (XfceDesktopEntry *desktop_entry,
                           const gchar      *key)
{
  XfceDesktopEntryItem *item;
  gint                  n;

  for (item = desktop_entry->priv->items, n = 0; n < desktop_entry->priv->num_items; ++item, ++n)
    if (strcmp (item->key, key) == 0)
      return item;

  return NULL;
}



static gboolean
parse_desktop_entry_line (const gchar *line,
                          gchar      **section_return,
                          gchar      **key_return,
                          gchar      **value_return,
                          gchar      **locale_return)
{
  const gchar *p = line;
  const gchar *q;
  const gchar *r;
  const gchar *s;
 
  /* initialize to NULL, so we don't have tho think about it anymore */
  *section_return = NULL;
  *key_return = NULL;
  *value_return = NULL;
  *locale_return = NULL;
  
  /* skip whitespace */
  while (g_ascii_isspace (*p))
    ++p;

  if (*p == '#' || *p == '\n' || *p == '\0')
    return FALSE;
  
  if (*p == '[')
    {
      ++p;

      q = strchr (p, ']');
      if (G_UNLIKELY (q == NULL))
        return FALSE;

      *section_return = g_strndup (p, q - p);
    }
  else
    {
      q = strchr (p, '=');
      if (G_UNLIKELY (q == NULL))
        return FALSE;

      r = q + 1;

      for (--q; g_ascii_isspace (*q); )
        --q;

      if (*q == ']')
        {
          s = strchr (p, '[');
          if (G_UNLIKELY (s == NULL))
            return FALSE;
          
          *key_return = g_strndup (p, s - p);

          ++s;

          *locale_return = g_strndup (s, q - s);
        }
      else
        {
            *key_return = g_strndup (p, (q + 1) - p);
        }
      
      while (g_ascii_isspace (*r))
        ++r;

      q = r + strlen (r);

      while (q > r && (g_ascii_isspace (*(q - 1)) || ((*(q - 1)) == '\r')))
        --q;

      if (q > r)
        *value_return = g_strndup (r, q - r);
      else
        *value_return = g_new0 (gchar, 1);
    }

  return TRUE;
}



static gboolean
xfce_desktop_entry_parse (XfceDesktopEntry *desktop_entry)
{
  XfceDesktopEntryItem *item;
  const gchar          *current_locale;
  gboolean              in_d_e_section = FALSE;
  gboolean              result = FALSE;
  gchar                *current_section = NULL;
  gchar                *section;
  gchar                *locale;
  gchar                *value;
  gchar                *key;
  gchar               **lines;
  gchar               **p;
  gint                  locale_matched = 0;
  gint                  i;

  current_locale = setlocale (LC_MESSAGES, NULL);

  lines = g_strsplit (desktop_entry->priv->data, "\n", -1);
  for (p = lines; *p != NULL; ++p)
    {
      /* some .desktop files have multiple sections.  we're only interested
       * in the "[Desktop Entry]" section */
      if (!in_d_e_section)
        {
          if (!g_ascii_strncasecmp (*p, "[Desktop Entry]", 15))
            in_d_e_section = TRUE;
        }
      else if (**p == '[' && g_ascii_strncasecmp(*p, "[Desktop Entry]", 15))
        in_d_e_section = FALSE;
  
      if (!in_d_e_section)
        continue;
      
      if (!parse_desktop_entry_line (*p, &section, &key, &value, &locale))
        continue;

      if (G_UNLIKELY (section != NULL))
        {
          g_free (current_section);
          current_section = section;
          continue;
        }
      
      item = desktop_entry->priv->items;

      for (i = 0; i < desktop_entry->priv->num_items; ++i, ++item)
        {
          if (key != NULL && item->key != NULL && strcmp (key, item->key) == 0)
            {
              if (current_locale != NULL && locale != NULL)
                {
                  gint match = xfce_locale_match (current_locale, locale);
                  if (match > locale_matched)
                    {
                      g_free (item->translated_value);
                      item->translated_value = g_strdup (value);
                    }
                }
              else
                {
                  g_free (item->value);
                  item->value = g_strdup (value);
                  result = TRUE;
                }
              
              if (current_section != NULL)
                {
                  g_free (item->section);
                  item->section = g_strdup (current_section);
                }

              break;
            }
        }
      
      g_free (value);
      g_free (key);
      g_free (locale);
    }

  g_free (current_section);
  g_strfreev (lines);

  return result;
}



/**
 * xfce_desktop_entry_new:
 * @file           : full path to the desktop entry file to use.
 * @categories     : array of categories, not necessarily NULL terminated.
 * @num_categories : number of items in @categories.
 *
 * Create a new #XfceDesktopEntry object from a desktop entry stored in a file on disk.
 *
 * The caller is responsible to free the returned object (if any) using
 * g_object_unref() when no longer needed.
 *
 * Return value: newly created #XfceDesktopEntry or NULL if something goes wrong.
 *
 * Since: 4.2
 *
 * Deprecated: 4.8: Use #XfceRc instead.
 **/
XfceDesktopEntry*
xfce_desktop_entry_new (const gchar  *file,
                        const gchar **categories,
                        gint          num_categories)
{
  XfceDesktopEntryItem *item;
  XfceDesktopEntry     *desktop_entry;
  gint                  n;

  g_return_val_if_fail (file != NULL, NULL);
  g_return_val_if_fail (categories != NULL, NULL);

  desktop_entry = g_object_new (XFCE_TYPE_DESKTOP_ENTRY, NULL);
  desktop_entry->priv->file = g_strdup (file);
  desktop_entry->priv->items = g_new0 (XfceDesktopEntryItem, num_categories);
  desktop_entry->priv->num_items = num_categories;

  if (!g_file_get_contents (desktop_entry->priv->file, &(desktop_entry->priv->data), NULL, NULL))
    {
      g_warning ("Could not get contents of file %s", desktop_entry->priv->file);
      g_object_unref (G_OBJECT (desktop_entry));
      return NULL;
    }

  for (item = desktop_entry->priv->items, n = 0; n < desktop_entry->priv->num_items; ++item, ++n)
    item->key = g_strdup (categories[n]);

  if (!xfce_desktop_entry_parse (desktop_entry))
    {
      g_object_unref (G_OBJECT (desktop_entry));
      desktop_entry = NULL;
    }

  return desktop_entry;
}



/**
 * xfce_desktop_entry_new_from_data:
 * @data           : pointer to the desktop entry inline data.
 * @categories     : array of categories, not necessarily NULL terminated.
 * @num_categories : number of items in @categories.
 *
 * Create a new #XfceDesktopEntry object from a desktop entry stored in memory.
 *
 * The caller is responsible to free the returned object (if any) using
 * g_object_unref() when no longer needed.
 *
 * Return value: newly created #XfceDesktopEntry or NULL if something goes wrong.
 *
 * Since: 4.2
 *
 * Deprecated: 4.8: Use #XfceRc instead.
 **/
XfceDesktopEntry*
xfce_desktop_entry_new_from_data (const gchar  *data,
                                  const gchar **categories,
                                  gint          num_categories)
{
  XfceDesktopEntryItem *item;
  XfceDesktopEntry     *desktop_entry;
  gint                  n;

  g_return_val_if_fail (data != NULL, NULL);
  g_return_val_if_fail (categories != NULL, NULL);

  desktop_entry = g_object_new (XFCE_TYPE_DESKTOP_ENTRY, NULL);
  desktop_entry->priv->file = g_strdup (""); /* or "data" or ... */
  desktop_entry->priv->data = g_strdup (data);
  desktop_entry->priv->items = g_new0 (XfceDesktopEntryItem, num_categories);
  desktop_entry->priv->num_items = num_categories;

  for (item = desktop_entry->priv->items, n = 0; n < desktop_entry->priv->num_items; ++item, ++n)
    item->key = g_strdup (categories[n]);

  if (!xfce_desktop_entry_parse (desktop_entry))
    {
      g_object_unref (G_OBJECT (desktop_entry));
      desktop_entry = NULL;
    }

  return desktop_entry;
}



/**
 * xfce_desktop_entry_get_file:
 * @desktop_entry: an #XfceDesktopEntry.
 *
 * Obtain the path to the desktop entry file associated with the
 * #XfceDesktopEntry.
 *
 * Return value: path of the desktop entry file used to create @desktop_entry. The
 *               return value should be considered read-only and must not be freed
 *               by the caller.
 * Since: 4.2
 *
 * Deprecated: 4.8: Use #XfceRc instead.
 **/
G_CONST_RETURN gchar*
xfce_desktop_entry_get_file (XfceDesktopEntry *desktop_entry)
{
    g_return_val_if_fail (XFCE_IS_DESKTOP_ENTRY (desktop_entry), NULL);
    return desktop_entry->priv->file;
}



/**
 * xfce_desktop_entry_get_string:
 * @desktop_entry : an #XfceDesktopEntry
 * @key           : category to find value for.
 * @translated    : set to TRUE if the translated value is preferred.
 * @value_return  : location for the value, which will be newly allocated.
 *
 * Finds the value for @key. When @translated is TRUE the function will use
 * the translated value (using the current locale settings) if available or
 * the untranslated value if no translation can be found.
 *
 * Return value: TRUE on success, FALSE on failure. @value_return must be freed.
 *
 * Since: 4.2
 *
 * Deprecated: 4.8: Use #XfceRc instead.
**/
gboolean
xfce_desktop_entry_get_string (XfceDesktopEntry *desktop_entry,
                               const gchar      *key,
                               gboolean          translated,
                               gchar           **value_return)
{
  const XfceDesktopEntryItem *item;

  g_return_val_if_fail (XFCE_IS_DESKTOP_ENTRY (desktop_entry), FALSE);
  g_return_val_if_fail (value_return != NULL, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  item = xfce_desktop_entry_lookup (desktop_entry, key);
  if (G_UNLIKELY (item == NULL || item->value == NULL || *item->value == '\0'))
    return FALSE;

  if (translated && item->translated_value != NULL)
    *value_return = g_strdup (item->translated_value);
  else
    *value_return = g_strdup (item->value);

  return TRUE;
}



/**
 * xfce_desktop_entry_get_int:
 * @desktop_entry : an #XfceDesktopEntry.
 * @key           : category to find value for.
 * @value_return  : location for the value.
 *
 * Gets a value from @desktop_entry as integer. Therefore finds the value for
 * @key and returns its integer representation.
 *
 * Return value: TRUE on success, FALSE on failure.
 *
 * Since: 4.2
 *
 * Deprecated: 4.8: Use #XfceRc instead.
 **/
gboolean
xfce_desktop_entry_get_int (XfceDesktopEntry *desktop_entry,
                            const gchar      *key,
                            gint             *value_return)
{
  const XfceDesktopEntryItem *item;
  gchar                      *endptr;

  g_return_val_if_fail (XFCE_IS_DESKTOP_ENTRY (desktop_entry), FALSE);
  g_return_val_if_fail (value_return != NULL, FALSE);
  g_return_val_if_fail (key != NULL, FALSE);

  item = xfce_desktop_entry_lookup (desktop_entry, key);
  if (G_UNLIKELY (item == NULL || item->value == NULL || *item->value == '\0'))
    return FALSE;

  *value_return = strtol (item->value, &endptr, 10);
  if (*endptr == '\0')
    return TRUE;

  return FALSE;
}



/**
 * xfce_desktop_entry_has_translated_entry:
 * @desktop_entry : an #XfceDesktopEntry.
 * @key           : the key to check.
 *
 * Checks to see if @desktop_entry has a value for @key translated into
 * the current locale.
 *
 * Return Value: %TRUE if there is a translated key, %FALSE otherwise.
 *
 * Since: 4.3
 *
 * Deprecated: 4.8: Use #XfceRc instead.
 **/
gboolean
xfce_desktop_entry_has_translated_entry (XfceDesktopEntry *desktop_entry,
                                         const gchar      *key)
{
  const XfceDesktopEntryItem *item;
  const gchar                *current_locale;
  
  g_return_val_if_fail (XFCE_IS_DESKTOP_ENTRY (desktop_entry), FALSE);
  g_return_val_if_fail (key != NULL, FALSE);
  
  item = xfce_desktop_entry_lookup (desktop_entry, key);
  if (G_UNLIKELY (item == NULL || item->value == NULL || *item->value == '\0'))
    return FALSE;

  current_locale = setlocale (LC_MESSAGES, NULL);
  
  if (item->translated_value == NULL
      && !xfce_locale_match (current_locale, "C")
      && !xfce_locale_match (current_locale, "POSIX"))
  {
      return FALSE;
  }
  
  return TRUE;
}



#define __XFCE_DESKTOPENTRY_C__
#include <libxfce4util/libxfce4util-aliasdef.c>
