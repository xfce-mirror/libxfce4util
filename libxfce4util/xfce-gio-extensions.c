/*
 * Copyright (c) 2021 The Xfce Development Team
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA
 */

/**
 * SECTION:xfce-gio-extensions
 * @title: GIO Extensions
 * @short_description: various extensions to GIO
 * @include: libxfce4util/libxfce4util.h
 *
 * Common used functions for I/O that are not provided by the Glib library
 *
 * Since: 4.17
 **/



#include <libxfce4util/libxfce4util.h>
#include <libxfce4util/libxfce4util-alias.h>



#define XFCE_ATTRIBUTE_EXECUTABLE_CHECKSUM "metadata::xfce-exe-checksum" /* string */



/**
 * xfce_g_file_metadata_is_supported:
 * @file: a #GFile.
 *
 * Checks if GVFS-metadata is available for
 * the @file provided.
 *
 * Returns: %TRUE is GVFS-metadata is supported, %FALSE otherwise.
 *
 * Since: 4.17
 **/
gboolean
xfce_g_file_metadata_is_supported (GFile *file)
{
  GFileAttributeInfoList *info_list;
  gboolean                is_supported;

  g_return_val_if_fail (G_IS_FILE (file), FALSE);

  info_list = g_file_query_writable_namespaces (file, NULL, NULL);
  if (info_list == NULL)
    return FALSE;

  is_supported = (g_file_attribute_info_list_lookup (info_list, "metadata") != NULL);
  g_file_attribute_info_list_unref (info_list);

  return is_supported;
}



/**
 * xfce_g_file_create_checksum:
 * file: a @GFile.
 *
 * Generates an SHA-256 hash of the @file.
 * Utilizes g_compute_checksum_for_data() which
 * is present since Glib 2.16.
 *
 * Returns: (transfer full) (nullable): Checksum of the @file.
 * If file read fails, returns %NULL. Free with g_free().
 *
 * Since: 4.17
 **/
gchar *
xfce_g_file_create_checksum (GFile *file)
{
  GFileInfo        *file_info;
  GFileInputStream *stream;
  guchar           *contents_buffer;
  gchar            *checksum;
  gsize             file_size, file_size_read;
  gboolean          read_successful;

  g_return_val_if_fail (G_IS_FILE (file), NULL);

  /* query size */
  file_info = g_file_query_info (file,
                                 G_FILE_ATTRIBUTE_STANDARD_SIZE,
                                 G_FILE_QUERY_INFO_NONE,
                                 NULL,
                                 NULL);
  if (file_info == NULL)
    return NULL;

  file_size = g_file_info_get_size (file_info);
  g_object_unref (file_info);

  /* special case : SHA-256 hash of NULL */
  /* e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855 */
  if (file_size == 0)
    return g_compute_checksum_for_data (G_CHECKSUM_SHA256, NULL, 0);

  /* allocate buffer */
  contents_buffer = g_malloc (file_size);
  if (contents_buffer == NULL)
    return NULL;

  read_successful = FALSE;
  /* read the actual file */
  if ((stream = g_file_read (file, NULL, NULL)) != NULL)
    {
      read_successful = g_input_stream_read_all (G_INPUT_STREAM (stream),
                                                 contents_buffer,
                                                 file_size,
                                                 &file_size_read,
                                                 NULL,
                                                 NULL);
    }
  if (read_successful)
    checksum = g_compute_checksum_for_data (G_CHECKSUM_SHA256,
                                            contents_buffer,
                                            file_size_read);
  else
    checksum = NULL;

  /* free buffer */
  g_free (contents_buffer);

  return checksum;
}



/**
 * xfce_g_file_set_trusted:
 * @file: a #Gfile.
 * @is_safe: #TRUE if safe, #FALSE otherwise
 *
 * Sets the "safety flag" on if @is_safe.
 *
 * Safety flag is a new concept introduced in
 * XFCE 4.17. It is basically an additional
 * execution flag stored in GVFS-metadata.
 * Unlike the execution flag (+x), it is safe
 * to assume that this flag did not come from
 * foreign location (for example, by downloading
 * an tar archive) and set by user.
 *
 * The checksum of the file is stored, and
 * would be considered "on" only if checksum
 * matches with the file on execution.
 *
 * Since: 4.17
 **/
void
xfce_g_file_set_trusted (GFile    *file,
                         gboolean  is_safe)
{
  gchar   *checksum_string;
  gboolean attr_set;

  g_return_if_fail (G_IS_FILE (file));

  if (!xfce_g_file_metadata_is_supported(file))
    return;

  if (is_safe)
    {
      checksum_string = xfce_g_file_create_checksum (file);
      if (checksum_string == NULL)
        return;
    }
  else
    checksum_string = NULL;

  attr_set = g_file_set_attribute (file,
                                   XFCE_ATTRIBUTE_EXECUTABLE_CHECKSUM,
                                   is_safe ? G_FILE_ATTRIBUTE_TYPE_STRING : G_FILE_ATTRIBUTE_TYPE_INVALID,
                                   checksum_string,
                                   G_FILE_ATTRIBUTE_INFO_COPY_WITH_FILE | G_FILE_ATTRIBUTE_INFO_COPY_WHEN_MOVED,
                                   NULL,
                                   NULL);
  if (!attr_set)
    {
      g_warning ("Failed to set '%s' attribute", XFCE_ATTRIBUTE_EXECUTABLE_CHECKSUM);
      g_warning ("File path : %s", g_file_peek_path (file));
    }
  g_free (checksum_string);
}



/**
 * xfce_g_file_is_trusted:
 * @file: a #GFile.
 *
 * Compares the checksum stored in safety flag
 * with the actual file. If it matches, it
 * is considered safe.
 *
 * Read the documentation of
 * xfce_g_file_set_safety_flag() for details.
 *
 * Returns: %TRUE if safety flag is verified,
 * %FALSE otherwise.
 *
 * Since: 4.17
 **/
gboolean
xfce_g_file_is_trusted (GFile *file)
{
  GFileInfo   *file_info;
  gboolean     is_safe;
  const gchar *attribute_string;
  gchar *checksum_string;

  g_return_val_if_fail (G_IS_FILE (file), FALSE);

  if (!xfce_g_file_metadata_is_supported(file))
    return TRUE;

  file_info = g_file_query_info (file,
                                 XFCE_ATTRIBUTE_EXECUTABLE_CHECKSUM,
                                 G_FILE_QUERY_INFO_NONE,
                                 NULL,
                                 NULL);
  if (file_info == NULL)
    return FALSE;

  attribute_string = g_file_info_get_attribute_string (file_info,
                                                       XFCE_ATTRIBUTE_EXECUTABLE_CHECKSUM);
  if (attribute_string != NULL)
    {
      checksum_string = xfce_g_file_create_checksum (file);
      is_safe = (g_strcmp0 (attribute_string, checksum_string) == 0);
      g_info ("== Safety flag check ==");
      g_info ("Attribute checksum: %s", attribute_string);
      g_info ("File checksum     : %s", checksum_string);
      g_free (checksum_string);
    }
  else
    is_safe = FALSE;

  g_object_unref (file_info);

  return is_safe;
}

#define __XFCE_GIO_EXTENSIONS_C__
#include <libxfce4util/libxfce4util-aliasdef.c>
