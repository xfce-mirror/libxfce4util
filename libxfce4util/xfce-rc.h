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

#ifndef __LIBXFCE4UTIL_XFCE_RC_H__
#define __LIBXFCE4UTIL_XFCE_RC_H__

#include <libxfce4util/xfce-resource.h>

typedef struct _XfceRc XfceRc;

#define XFCE_RC(obj)       ((XfceRc *) (obj))
#define XFCE_RC_CONST(obj) ((const XfceRc *) (obj))

XfceRc*      xfce_rc_simple_open             (const gchar     *filename,
					      gboolean         readonly);

XfceRc*      xfce_rc_config_open             (XfceResourceType type,
					      const gchar     *resource,
					      gboolean         readonly);

void         xfce_rc_close                   (XfceRc *rc);
void         xfce_rc_flush                   (XfceRc *rc);
void         xfce_rc_rollback                (XfceRc *rc);

gboolean     xfce_rc_is_dirty                (const XfceRc *rc) G_GNUC_CONST;
gboolean     xfce_rc_is_readonly             (const XfceRc *rc) G_GNUC_CONST;

const gchar* xfce_rc_get_locale              (const XfceRc *rc) G_GNUC_CONST;

gchar**      xfce_rc_get_groups              (const XfceRc *rc) G_GNUC_CONST;
gchar**      xfce_rc_get_entries             (const XfceRc *rc,
					      const gchar  *group) G_GNUC_CONST;
const gchar* xfce_rc_get_group               (const XfceRc *rc) G_GNUC_CONST;
gboolean     xfce_rc_has_group               (const XfceRc *rc,
					      const gchar  *group) G_GNUC_CONST;
void         xfce_rc_set_group               (XfceRc       *rc,
					      const gchar  *group);

gboolean     xfce_rc_has_entry               (const XfceRc *rc,
					      const gchar  *key) G_GNUC_CONST;
const gchar* xfce_rc_read_entry              (const XfceRc *rc,
					      const gchar  *key,
					      const gchar  *fallback) G_GNUC_CONST;
const gchar* xfce_rc_read_entry_untranslated (const XfceRc *rc,
					      const gchar  *key,
					      const gchar  *fallback) G_GNUC_CONST;
gboolean     xfce_rc_read_bool_entry         (const XfceRc *rc,
					      const gchar  *key,
					      gboolean      fallback) G_GNUC_CONST;
gint         xfce_rc_read_int_entry          (const XfceRc *rc,
					      const gchar  *key,
					      gboolean      fallback) G_GNUC_CONST;
gchar**      xfce_rc_read_list_entry         (const XfceRc *rc,
					      const gchar  *key,
					      const gchar  *delimiter) G_GNUC_CONST;
void         xfce_rc_write_entry             (XfceRc       *rc,
					      const gchar  *key,
					      const gchar  *value);
void         xfce_rc_write_bool_entry        (XfceRc       *rc,
					      const gchar  *key,
					      gboolean      value);
void         xfce_rc_write_int_entry         (XfceRc       *rc,
					      const gchar  *key,
					      gint          value);
void         xfce_rc_write_list_entry        (XfceRc       *rc,
					      const gchar  *key,
					      gchar       **value,
					      const gchar  *separator);
					      

#endif /* !__LIBXFCE4UTIL_XFCE_RC_H__ */

