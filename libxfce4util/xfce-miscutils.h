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

#ifndef __LIBXFCE4UTIL_MISCUTILS_H__
#define __LIBXFCE4UTIL_MISCUTILS_H__

#include <glib.h>

#include <libxfce4util/debug.h>

/* This looks a bit "hacky", but its ok */
#define _BUILTIN_LICENSE_TEXT(name) \
({ \
  extern const char xfce_builtin_license_##name[]; \
  xfce_builtin_license_##name; \
})

#define XFCE_LICENSE_BSD  _BUILTIN_LICENSE_TEXT(BSD)

#define XFCE_LICENSE_GPL  _BUILTIN_LICENSE_TEXT(GPL)

#define XFCE_LICENSE_LGPL _BUILTIN_LICENSE_TEXT(LGPL)


const gchar* xfce_version_string (void) G_GNUC_PURE;

const gchar* xfce_get_homedir    (void) G_GNUC_PURE;

gchar* xfce_get_homefile_r       (gchar *buffer,
                                  size_t length, 
                                  const gchar *format,
                                  ...);

const gchar* xfce_get_userdir    (void) G_GNUC_PURE;

gchar* xfce_get_userfile_r       (gchar *buffer,
                                  size_t length, 
                                  const gchar *format,
                                  ...);

#if defined(G_HAVE_ISO_VARARGS)

#define xfce_get_homefile(...)                             \
  (g_build_filename (xfce_get_homedir (), __VA_ARGS__))

#define xfce_get_userfile(...)                             \
  (g_build_filename (xfce_get_userdir (), __VA_ARGS__))

#elif defined(G_HAVE_GNUC_VARARGS)

#define xfce_get_homefile(first_element...)	               \
  (g_build_filename (xfce_get_homedir (), ## first_element))

#define xfce_get_userfile(first_element...)	               \
  (g_build_filename (xfce_get_userdir (), ## first_element))

#else

static gchar*
xfce_get_homefile (const gchar *first_element, ...)
{
#error "Implement this, if you see this error!"
}

static gchar*
xfce_get_userfile (const gchar *first_element, ...)
{
#error "Implement this, if you see this error!"
}

#endif

gchar*  xfce_strjoin     (const gchar *separator,
                          gchar **strings,
                          gint count);

gchar*  xfce_gethostname (void) G_GNUC_PURE;

gint    xfce_putenv      (const gchar *string);

gint    xfce_setenv      (const gchar *name,
                          const gchar *value,
                          gboolean overwrite);
void    xfce_unsetenv    (const gchar *name);

gchar*  xfce_expand_variables (const gchar *command,
                               gchar      **envp);

#endif	/* __LIBXFCE4UTIL_MISCUTILS_H__ */
