/* $Id$ */
/*-
 * Copyright (c) 2003-2004 Benedikt Meurer <benny@xfce.org>
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
 */

#ifndef __LIBXFCE4UTIL_XFCE_RESOURCE_H__
#define __LIBXFCE4UTIL_XFCE_RESOURCE_H__

typedef enum
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
