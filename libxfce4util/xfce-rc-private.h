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

#ifndef __LIBXFCE4UTIL_XFCE_RC_PRIVATE_H__
#define __LIBXFCE4UTIL_XFCE_RC_PRIVATE_H__

#include <libxfce4util/libxfce4util.h>

typedef struct _XfceRcConfig XfceRcConfig;
typedef struct _XfceRcSimple XfceRcSimple;

struct _XfceRc
{
  void (*close) (XfceRc *rc);
  void (*flush) (XfceRc *rc);
  void (*rollback) (XfceRc *rc);
  gboolean (*is_dirty) (const XfceRc *rc);
  gboolean (*is_readonly) (const XfceRc *rc);
  gchar** (*get_groups) (const XfceRc *rc);
  gchar** (*get_entries) (const XfceRc *rc, const gchar *group);
  void (*delete_group) (XfceRc *rc, const gchar *group, gboolean global);
  const gchar* (*get_group) (const XfceRc *rc);
  gboolean (*has_group) (const XfceRc *rc, const gchar *group);
  void (*set_group) (XfceRc *rc, const gchar *group);
  void (*delete_entry) (XfceRc *rc, const gchar *key, gboolean global);
  gboolean (*has_entry) (const XfceRc *rc, const gchar *key);
  const gchar* (*read_entry) (const XfceRc *rc, const gchar *key,
			      gboolean translated);
  /* write_entry == NULL means readonly */
  void (*write_entry) (XfceRc *rc, const gchar *key, const gchar *value);
  void (*reserved1) (void);
  void (*reserved2) (void);
  void (*reserved3) (void);
  void (*reserved4) (void);

  gchar   *locale;
};

#define XFCE_RC_CONFIG(obj)       ((XfceRcConfig *) (obj))
#define XFCE_RC_CONFIG_CONST(obj) ((const XfceRcConfig *) (obj))

#define XFCE_RC_SIMPLE(obj)       ((XfceRcSimple *) (obj))
#define XFCE_RC_SIMPLE_CONST(obj) ((const XfceRcSimple *) (obj))


void          _xfce_rc_init               (XfceRc       *rc);

XfceRcSimple* _xfce_rc_simple_new         (XfceRcSimple *shared,
					   const gchar  *filename,
					   gboolean      readonly);
gboolean      _xfce_rc_simple_parse       (XfceRcSimple *simple);
void          _xfce_rc_simple_close       (XfceRc       *rc);
void          _xfce_rc_simple_flush       (XfceRc       *rc);
void          _xfce_rc_simple_rollback    (XfceRc       *rc);
gboolean      _xfce_rc_simple_is_dirty    (const XfceRc *rc) G_GNUC_CONST;
gboolean      _xfce_rc_simple_is_readonly (const XfceRc *rc) G_GNUC_CONST;
const gchar*  _xfce_rc_simple_get_filename(const XfceRc *rc) G_GNUC_CONST;
gchar**       _xfce_rc_simple_get_groups  (const XfceRc *rc) G_GNUC_CONST;
gchar**       _xfce_rc_simple_get_entries (const XfceRc *rc,
					   const gchar  *name) G_GNUC_CONST;
void          _xfce_rc_simple_delete_group(XfceRc       *rc,
                                           const gchar  *name,
                                           gboolean      global);
const gchar*  _xfce_rc_simple_get_group   (const XfceRc *rc) G_GNUC_CONST;
gboolean      _xfce_rc_simple_has_group   (const XfceRc *rc,
					   const gchar  *name) G_GNUC_CONST;
void          _xfce_rc_simple_set_group   (XfceRc       *rc,
					   const gchar  *name);
void          _xfce_rc_simple_delete_entry(XfceRc       *rc,
                                           const gchar  *key,
                                           gboolean      global);
gboolean      _xfce_rc_simple_has_entry   (const XfceRc *rc,
					   const gchar  *key) G_GNUC_CONST;
const gchar*  _xfce_rc_simple_read_entry  (const XfceRc *rc,
					   const gchar  *key,
					   gboolean      translated) G_GNUC_CONST;
void          _xfce_rc_simple_write_entry (XfceRc       *rc,
					   const gchar  *key,
					   const gchar  *value);

XfceRcConfig* _xfce_rc_config_new         (XfceResourceType type,
					   const gchar  *resource,
					   gboolean      readonly);
void          _xfce_rc_config_close       (XfceRc       *rc);
void          _xfce_rc_config_flush       (XfceRc       *rc);
void          _xfce_rc_config_rollback    (XfceRc       *rc);
gboolean      _xfce_rc_config_is_dirty    (const XfceRc *rc) G_GNUC_CONST;
gboolean      _xfce_rc_config_is_readonly (const XfceRc *rc) G_GNUC_CONST;
gchar**       _xfce_rc_config_get_groups  (const XfceRc *rc) G_GNUC_CONST;
gchar**       _xfce_rc_config_get_entries (const XfceRc *rc,
					   const gchar  *name) G_GNUC_CONST;
void          _xfce_rc_config_delete_group(XfceRc       *rc,
                                           const gchar  *name,
                                           gboolean      global);
const gchar*  _xfce_rc_config_get_group   (const XfceRc *rc) G_GNUC_CONST;
gboolean      _xfce_rc_config_has_group   (const XfceRc *rc,
					   const gchar  *name) G_GNUC_CONST;
void          _xfce_rc_config_set_group   (XfceRc       *rc,
					   const gchar  *name);
void          _xfce_rc_config_delete_entry(XfceRc       *rc,
					   const gchar  *key,
                                           gboolean      global);
gboolean      _xfce_rc_config_has_entry   (const XfceRc *rc,
					   const gchar  *key) G_GNUC_CONST;
const gchar*  _xfce_rc_config_read_entry  (const XfceRc *rc,
					   const gchar  *key,
					   gboolean      translated) G_GNUC_CONST;
void          _xfce_rc_config_write_entry (XfceRc       *rc,
					   const gchar  *key,
					   const gchar  *value);


#endif /* !__LIBXFCE4UTIL_XFCE_RC_PRIVATE_H__ */
