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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#include <stdio.h>
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <libxfce4util/xfce-rc-private.h>


struct _XfceRcConfig
{
  XfceRc  __parent__;

  XfceRc *save;
  GList  *rclist;
};


/* local prototypes */
static gchar** merge_arrays (gchar **source1, gchar **source2);


static gchar**
merge_arrays (gchar **source1, gchar **source2)
{
  gchar **result;
  gchar **rp;
  gchar **sp;
  guint   size;
  guint   pos;

  result = g_new (gchar *, 11);
  size   = 10;
  pos    = 0;

  for (sp = source1; *sp != NULL; ++sp)
    {
      if (pos == size)
	{
	  size *= 2;
	  result = g_realloc (result, (size + 1) * sizeof (*result));
	}
      result[pos] = *sp;
      ++pos;
    }

  for (sp = source2; *sp != NULL; ++sp)
    {
      for (rp = result; rp < result + pos; ++rp)
	if (strcmp (*rp, *sp) == 0)
	  break;

      if (rp == result + pos)
	{
	  g_free (*sp);
	  continue;
	}

      if (pos == size)
	{
	  size += 2;
	  result = g_realloc (result, (size + 1) * sizeof (*result));
	}
      result[pos] = *sp;
      ++pos;
    }

  result[pos] = NULL;

  g_free (source1);
  g_free (source2);

  return result;
}


XfceRcConfig*
_xfce_rc_config_new (XfceResourceType type,
		     const gchar     *resource,
		     gboolean         readonly)
{
  XfceRcConfig *config;
  XfceRcSimple *simple = NULL;
  gboolean      user_present = FALSE;
  gchar        *user;
  gchar       **paths;
  gchar       **p;

  g_return_val_if_fail (resource != NULL, NULL);
  g_return_val_if_fail (strlen (resource) > 0, NULL);
  g_return_val_if_fail (resource[strlen (resource) - 1] != '/', NULL);

  user = xfce_resource_save_location (type, resource, FALSE);
  paths = xfce_resource_lookup_all (type, resource);

  g_assert (user != NULL);
  g_assert (paths != NULL);

  config = g_new0 (XfceRcConfig, 1);

  /* system files first */
  for (p = paths; *p != NULL; ++p)
    {
      if (strcmp (*p, user) == 0)
	{
	  user_present = TRUE;
	  continue;
	}

      simple = _xfce_rc_simple_new (simple, *p, TRUE);
      if (!_xfce_rc_simple_parse (simple))
	{
	  g_critical ("Failed to parse file %s, ignoring.", *p);
	  xfce_rc_close (XFCE_RC (simple));
	  continue;
	}
      config->rclist = g_list_append (config->rclist, simple);
    }

  /* now the user file */
  simple = _xfce_rc_simple_new (simple, user, readonly);
  if (user_present && !_xfce_rc_simple_parse (simple))
    {
      g_critical ("Failed to parse file %s, ignoring.", user);
    }
  config->save   = XFCE_RC (simple);
  config->rclist = g_list_prepend (config->rclist, simple);

  /* attach callbacks */
  config->__parent__.close       = _xfce_rc_config_close;
  config->__parent__.get_groups  = _xfce_rc_config_get_groups;
  config->__parent__.get_entries = _xfce_rc_config_get_entries;
  config->__parent__.get_group   = _xfce_rc_config_get_group;
  config->__parent__.has_group   = _xfce_rc_config_has_group;
  config->__parent__.set_group   = _xfce_rc_config_set_group;
  config->__parent__.has_entry   = _xfce_rc_config_has_entry;
  config->__parent__.read_entry  = _xfce_rc_config_read_entry;

  if (!readonly)
    {
      config->__parent__.flush       = _xfce_rc_config_flush;
      config->__parent__.rollback    = _xfce_rc_config_rollback;
      config->__parent__.is_dirty    = _xfce_rc_config_is_dirty;
      config->__parent__.is_readonly = _xfce_rc_config_is_readonly;
      config->__parent__.write_entry = _xfce_rc_config_write_entry;
    }

  g_strfreev (paths);
  g_free (user);

  return config;
}


void
_xfce_rc_config_close (XfceRc *rc)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);
  GList        *list;

  for (list = config->rclist; list != NULL; list = list->next)
    xfce_rc_close (XFCE_RC (list->data));
  g_list_free (config->rclist);
}


void
_xfce_rc_config_flush (XfceRc *rc)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);
  const gchar  *filename;
  gchar        *dir;

  /* create the base directory for the local rc file on demand */
  if (!_xfce_rc_simple_is_readonly (XFCE_RC_CONST (config->save)))
    {
      filename = _xfce_rc_simple_get_filename (XFCE_RC_CONST (config->save));
      dir = g_path_get_dirname (filename);
      if (!xfce_mkdirhier (dir, 0700, NULL))
	{
	  g_critical ("Unable to create base directory %s. "
		      "Saving to file %s is likely to fail.",
		      dir, filename);
	}
      g_free (dir);
    }

  _xfce_rc_simple_flush (XFCE_RC (config->save));
}


void
_xfce_rc_config_rollback (XfceRc *rc)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);

  _xfce_rc_simple_rollback (XFCE_RC (config->save));
}


gboolean
_xfce_rc_config_is_dirty (const XfceRc *rc)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);

  return _xfce_rc_simple_is_dirty (XFCE_RC_CONST (config->save));
}


gboolean
_xfce_rc_config_is_readonly (const XfceRc *rc)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);

  return _xfce_rc_simple_is_readonly (XFCE_RC_CONST (config->save));
}


gchar**
_xfce_rc_config_get_groups (const XfceRc *rc)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);
  gchar             **result = NULL;
  gchar             **tmp;
  GList              *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      tmp = _xfce_rc_simple_get_groups (XFCE_RC_CONST (list->data));
      if (tmp == NULL)
	continue;
      
      if (result == NULL)
	result = tmp;
      else
	result = merge_arrays (result, tmp);
    }

  return result;
}


gchar**
_xfce_rc_config_get_entries (const XfceRc *rc, const gchar *name)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);
  gchar             **result = NULL;
  gchar             **tmp;
  GList              *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      tmp = _xfce_rc_simple_get_entries (XFCE_RC_CONST (list->data), name);
      if (tmp == NULL)
	continue;

      if (result == NULL)
	result = tmp;
      else
	result = merge_arrays (result, tmp);
    }

  return result;
}


const gchar*
_xfce_rc_config_get_group (const XfceRc *rc)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);

  return _xfce_rc_simple_get_group (XFCE_RC_CONST (config->save));
}


gboolean
_xfce_rc_config_has_group (const XfceRc *rc, const gchar *name)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);
  GList              *list;

  /* atleast one has to have the specified group! */
  for (list = config->rclist; list != NULL; list = list->next)
    if (_xfce_rc_simple_has_group (XFCE_RC_CONST (list->data), name))
      return TRUE;

  return FALSE;
}


void
_xfce_rc_config_set_group (XfceRc *rc, const gchar *name)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);
  GList        *list;

  for (list = config->rclist; list != NULL; list = list->next)
    _xfce_rc_simple_set_group (XFCE_RC (list->data), name);
}


gboolean
_xfce_rc_config_has_entry (const XfceRc *rc, const gchar *key)
{
  const XfceRcConfig *config = XFCE_RC_CONFIG_CONST (rc);
  GList              *list;

  /* atleast one has to have the specified entry! */
  for (list = config->rclist; list != NULL; list = list->next)
    if (_xfce_rc_simple_has_entry (XFCE_RC_CONST (list->data), key))
      return TRUE;

  return FALSE;
}


const gchar*
_xfce_rc_config_read_entry (const XfceRc *rc,
			    const gchar  *key,
			    gboolean      translated)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);
  const gchar  *value;
  GList        *list;

  for (list = config->rclist; list != NULL; list = list->next)
    {
      value = _xfce_rc_simple_read_entry (XFCE_RC_CONST (list->data),
					  key,
					  translated);
      if (value != NULL)
	return value;
    }

  return NULL;
}


void
_xfce_rc_config_write_entry (XfceRc      *rc,
			     const gchar *key,
			     const gchar *value)
{
  XfceRcConfig *config = XFCE_RC_CONFIG (rc);

  /* XXX - don't write default values */
  if (!_xfce_rc_simple_is_readonly (XFCE_RC (config->save)))
    _xfce_rc_simple_write_entry (XFCE_RC (config->save), key, value);
}


