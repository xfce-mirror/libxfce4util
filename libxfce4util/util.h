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

#ifndef __LIBXFCE4UTIL_UTIL_H__
#define __LIBXFCE4UTIL_UTIL_H__

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


G_CONST_RETURN gchar* xfce_version_string (void) G_GNUC_PURE;

G_CONST_RETURN gchar* xfce_get_homedir (void) G_GNUC_PURE;

#define xfce_get_homefile(first_element...)	\
	(g_build_filename(xfce_get_homedir(),	## first_element))

G_CONST_RETURN gchar* xfce_get_homefile_r (gchar *buffer,
                                           size_t length, 
                                           const gchar *format,
                                           ...) G_GNUC_PURE;

G_CONST_RETURN gchar* xfce_get_userdir (void) G_GNUC_PURE;

#define xfce_get_userfile(first_element...)	\
	(g_build_filename(xfce_get_userdir(),	## first_element))

G_CONST_RETURN gchar* xfce_get_userfile_r (gchar *buffer,
                                           size_t length, 
                                           const gchar *format,
                                           ...) G_GNUC_PURE;

gchar*  xfce_strjoin (const gchar *separator,
                      gchar **strings,
                      gint count) G_GNUC_PURE;

gchar*  xfce_gethostname (void) G_GNUC_PURE;

gint    xfce_putenv (const gchar *string);

gint    xfce_setenv (const gchar *name,
                     const gchar *value,
                     gboolean overwrite);

gboolean xfce_mkdirhier (const gchar   *whole_path,
                         unsigned long  mode,
                         GError       **error);

#endif	/* __LIBXFCE4UTIL_UTIL_H__ */
