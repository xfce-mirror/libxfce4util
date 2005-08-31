/* $Id$ */
/*-
 * Copyright (c) 2003-2005 Benedikt Meurer <benny@xfce.org>
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

#ifndef __LIBXFCE4UTIL_XFCE_RESOURCE_H__
#define __LIBXFCE4UTIL_XFCE_RESOURCE_H__

typedef enum /*< prefix=XFCE_RESOURCE_ >*/
{
  XFCE_RESOURCE_DATA   = 0,
  XFCE_RESOURCE_CONFIG = 1,
  XFCE_RESOURCE_CACHE  = 2,
  XFCE_RESOURCE_ICONS  = 3,
  XFCE_RESOURCE_THEMES = 4,
} XfceResourceType;

typedef gboolean (*XfceMatchFunc)   (const gchar *basedir,
				     const gchar *relpath,
				     gpointer     user_data);

gchar** xfce_resource_dirs          (XfceResourceType type);
gchar*  xfce_resource_lookup        (XfceResourceType type,
				     const gchar     *filename);
gchar** xfce_resource_lookup_all    (XfceResourceType type,
				     const gchar     *filename);
gchar** xfce_resource_match         (XfceResourceType type,
				     const gchar     *pattern,
				     gboolean         unique);
gchar** xfce_resource_match_custom  (XfceResourceType type,
				     gboolean         unique,
				     XfceMatchFunc    func,
				     gpointer         user_data);
void    xfce_resource_push_path     (XfceResourceType type,
				     const gchar     *path);
void    xfce_resource_pop_path      (XfceResourceType type);
gchar*  xfce_resource_save_location (XfceResourceType type,
				     const gchar     *relpath,
				     gboolean         create);

#endif /* !__LIBXFCE4UTIL_XFCE_RESOURCE_H__ */
