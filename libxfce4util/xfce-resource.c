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
#ifdef HAVE_STRING_H
#include <string.h>
#endif

#include <libxfce4util/libxfce4util.h>


#define TYPE_VALID(t) ((t) >= XFCE_RESOURCE_DATA && (t) <= XFCE_RESOURCE_ICONS)


static gchar*   _save[4] = { NULL, };
static GList*   _list[4] = { NULL, };
static gboolean _inited = FALSE;


static const gchar*
_res_getenv (const gchar *variable,
	     const gchar *fallback)
{
  static gchar buffer[PATH_MAX];
  const gchar *result;
  
  result = g_getenv (variable);
  if (result == NULL)
    {
      if (*fallback == '~')
        {
          g_strlcpy (buffer, xfce_get_homedir(), PATH_MAX);
          g_strlcat (buffer, fallback + 1, PATH_MAX);

          result = buffer;
        }
      else
        {
          result = fallback;
        }
    }
  return result;
}


static void
_res_split_and_append (const gchar     *dir_list,
		       XfceResourceType type)
{
  gchar **dirs;
  gint    n;

  dirs = g_strsplit (dir_list, ":", -1);
  for (n = 0; dirs[n] != NULL; ++n)
    {
      _list[type] = g_list_append (_list[type], dirs[n]);
    }
  g_free (dirs);
}


static GList*
_res_remove_duplicates (GList *list)
{
  GList *ll = NULL;
  GList *pp;
  GList *lp;

  for (lp = list; lp != NULL; lp = lp->next)
    {
      for (pp = ll; pp != NULL; pp = pp->next)
	if (strcmp ((const gchar *) pp->data, (const gchar *) lp->data) == 0)
	  break;

      if (pp == NULL)
	ll = g_list_append (ll, lp->data);
      else
	g_free (lp->data);
    }

  g_list_free (list);
  return ll;;
}


static void
_res_init (void)
{
  const gchar *dirs;
  const gchar *dir;
  gchar       *path;
  GList       *l;

  if (_inited)
    return;

  _inited = TRUE;

  /*
   * Cache
   */
  dir = _res_getenv ("XDG_CACHE_HOME", DEFAULT_XDG_CACHE_HOME);
  if (!xfce_mkdirhier (dir, 0700, NULL))
    {
      g_warning ("Invalid XDG_CACHE_HOME directory `%s', program may "
		 "behave incorrectly.", dir);
    }
  _save[XFCE_RESOURCE_CACHE] = g_strdup (dir);
  _list[XFCE_RESOURCE_CACHE] = g_list_prepend (_list[XFCE_RESOURCE_CACHE],
					       g_strdup (dir));

  /*
   * Data home
   */
  dir = _res_getenv ("XDG_DATA_HOME", DEFAULT_XDG_DATA_HOME);
  if (!xfce_mkdirhier (dir, 0700, NULL))
    {
      g_warning ("Invalid XDG_DATA_HOME directory `%s', program may "
		 "behave incorretly.", dir);
    }
  _save[XFCE_RESOURCE_DATA] = g_strdup (dir);
  _list[XFCE_RESOURCE_DATA] = g_list_prepend (_list[XFCE_RESOURCE_DATA],
					      g_strdup (dir));

  /*
   * Config home
   */
  dir = _res_getenv ("XDG_CONFIG_HOME", DEFAULT_XDG_CONFIG_HOME);
  if (!xfce_mkdirhier (dir, 0700, NULL))
    {
      g_warning ("Invalid XDG_CONFIG_HOME directory `%s', program may "
		 "behave incorrectly.", dir);
    }
  _save[XFCE_RESOURCE_CONFIG] = g_strdup (dir);
  _list[XFCE_RESOURCE_CONFIG] = g_list_prepend (_list[XFCE_RESOURCE_CONFIG],
						g_strdup (dir));

  /*
   * Data dirs
   */
  dirs = _res_getenv ("XDG_DATA_DIRS", DEFAULT_XDG_DATA_DIRS);
  _res_split_and_append (dirs, XFCE_RESOURCE_DATA);

  /*
   * Config dirs
   */
  dirs = _res_getenv ("XDG_CONFIG_DIRS", DEFAULT_XDG_CONFIG_DIRS);
  _res_split_and_append (dirs, XFCE_RESOURCE_CONFIG);

  /*
   * Icons dirs
   */
  /* local icons dir first */
  path = g_build_filename (_save[XFCE_RESOURCE_DATA], "icons", NULL);
  _save[XFCE_RESOURCE_ICONS] = g_strdup (path);
  _list[XFCE_RESOURCE_ICONS] = g_list_prepend (_list[XFCE_RESOURCE_ICONS], path);

  /* backward compatibility */
  path = g_build_filename (xfce_get_homedir (), ".icons", NULL);
  _list[XFCE_RESOURCE_ICONS] = g_list_append (_list[XFCE_RESOURCE_ICONS], path);

  for (l = _list[XFCE_RESOURCE_DATA]; l != NULL; l = l->next)
    {
      path = g_build_filename ((const gchar *) l->data, "icons", NULL);
      _list[XFCE_RESOURCE_ICONS] = g_list_append (_list[XFCE_RESOURCE_ICONS],
						  path);
    }

  /* XDG fallback */
  _list[XFCE_RESOURCE_ICONS] = g_list_append (_list[XFCE_RESOURCE_ICONS],
					      "/usr/share/pixmaps");

  /* fallback for system which that don't install everything in /usr */
  _list[XFCE_RESOURCE_ICONS] = g_list_append (_list[XFCE_RESOURCE_ICONS],
                                              "/usr/local/share/pixmaps");
  _list[XFCE_RESOURCE_ICONS] = g_list_append (_list[XFCE_RESOURCE_ICONS],
                                              DATADIR "/share/pixmaps");

  /* remove duplicates from the lists */
#define REMOVE_DUPLICATES(type) { _list[(type)] = _res_remove_duplicates (_list[(type)]); }
  REMOVE_DUPLICATES (XFCE_RESOURCE_DATA);
  REMOVE_DUPLICATES (XFCE_RESOURCE_CONFIG);
  REMOVE_DUPLICATES (XFCE_RESOURCE_CACHE);
  REMOVE_DUPLICATES (XFCE_RESOURCE_ICONS);
#undef REMOVE_DUPLICATES
}


static gboolean
_res_splitup_pattern (const gchar *pattern,
		      gchar      **current,
		      gchar      **child)
{
  const gchar *p;

  if (*pattern == '\0' || *pattern == '/')
    return FALSE;

  p = strchr (pattern, '/');

  if (p == NULL || *(p + 1) == '\0')
    {
      *current = g_strdup (pattern);
      *child = NULL;
      return TRUE;
    }

  *current = g_new (gchar, p - pattern + 1);
  memcpy (*current, pattern, p - pattern);
  (*current)[p - pattern] = '\0';

  *child = g_strdup (p + 1);
  return TRUE;
}


static GList*
_res_match_path (const gchar *path,
		 const gchar *relpath,
		 const gchar *pattern,
		 GList       *entries)
{
  GPatternSpec *spec;
  const gchar  *entry;
  GFileTest     file_test = G_FILE_TEST_IS_REGULAR;
  gchar        *pattern_this;
  gchar        *pattern_child;
  gchar        *filename;
  gchar        *child_relpath;
  GList        *list = NULL;
  GDir         *dp;

  dp = g_dir_open (path, 0, NULL);
  if (dp == NULL)
    return entries;

  if (!_res_splitup_pattern (pattern, &pattern_this, &pattern_child))
    {
      g_dir_close (dp);
      return entries;
    }

  /* check if the last path component is a dir */
  if (pattern_child == NULL)
    {
      guint end = strlen (pattern_this) - 1;

      if (pattern_this[end] == '/')
	{
	  file_test = G_FILE_TEST_IS_DIR;
	  pattern_this[end] = '\0';
	}
    }

  spec = g_pattern_spec_new (pattern_this);

  while ((entry = g_dir_read_name (dp)) != NULL)
    {
      if (strcmp (entry, ".") == 0 || strcmp (entry, "..") == 0)
	continue;

      if (!g_pattern_match_string (spec, entry))
	continue;

      filename = g_build_filename (path, entry, NULL);

      if (pattern_child != NULL)
	{
	  if (g_file_test (filename, G_FILE_TEST_IS_DIR))
	    {
	      child_relpath = g_strconcat (relpath, entry, "/", NULL);
	      list = _res_match_path (filename, child_relpath, pattern_child, list);
	      g_free (child_relpath);
	    }
	}
      else if (g_file_test (filename, file_test))
	{
	  if (file_test == G_FILE_TEST_IS_DIR)
	    {
	      entries = g_list_append (entries, g_strconcat (relpath, entry, NULL));
	    }
	  else
	    {
	      entries = g_list_append (entries, g_strconcat (relpath, entry, "/", NULL));
	    }
	}

      g_free (filename);
    }

  g_pattern_spec_free (spec);
  g_dir_close (dp);

  if (pattern_child != NULL)
    g_free (pattern_child);
  g_free (pattern_this);

  return g_list_concat (entries, list);
}


/**
 * xfce_resource_dirs:
 * @type : type of the resource.
 *
 * Queries the list of possible directories for the specified @type. The
 * first element of the list is always the save location for @type. None
 * of the directories returned in the list are garantied to exist.
 *
 * This function should be rarely used. You should consider using
 * #xfce_resource_lookup, #xfce_resource_lookup_dirs or
 *  #xfce_resource_match instead.
 *
 * The returned list must be freed using g_strfreev().
 *
 * Return value: list of possible directories for @type.
 *
 * Since: 4.2
 **/
gchar**
xfce_resource_dirs (XfceResourceType type)
{
  gchar **paths;
  guint   size;
  guint   pos;
  GList  *l;

  g_return_val_if_fail (TYPE_VALID (type), NULL);

  _res_init ();

  paths = g_new (gchar *, 11);
  size  = 10;
  pos   = 0;

  for (l = _list[type]; l != NULL; l = l->next)
    {
      if (pos == size)
	{
	  size *= 2;
	  paths = g_realloc (paths, (size + 1) * sizeof (*paths));
	}

      paths[pos] = g_strdup ((const gchar *) l->data);
      ++pos;
    }

  paths[pos] = NULL;

  return paths;
}


/**
 * xfce_resource_lookup:
 * @type     : type of resource to lookup.
 * @filename : relative filename of the resource, e.g. "xfwm4/xfwmrc".
 *
 * Looks for a resource of the specified @type whose relative path matches
 * @filename. @filename can either reference a regular file, in which case
 * it must not end with a slash character ('/'), or a directory, when
 * @filename contains a trailing slash character ('/').
 *
 * Return value: the absolute path to the first file or directory in the
 *               search path, that matches @filename or %NULL if no such
 *               file or directory could be found.
 *
 * Since: 4.2
 **/
gchar*
xfce_resource_lookup (XfceResourceType type,
		      const gchar     *filename)
{
  GFileTest test;
  gchar     path[PATH_MAX];
  GList    *l;

  g_return_val_if_fail (TYPE_VALID (type), NULL);
  g_return_val_if_fail (filename != NULL && strlen (filename) > 0, NULL);

  _res_init ();

  if (filename[strlen (filename) - 1] == G_DIR_SEPARATOR)
    test = G_FILE_TEST_IS_DIR;
  else
    test = G_FILE_TEST_IS_REGULAR;

  for (l = _list[type]; l != NULL; l = l->next)
    {
      g_snprintf (path, PATH_MAX, "%s/%s", (const gchar *) l->data, filename);

      if (g_file_test (path, test))
	{
	  return g_strdup (path);
	}
    }

  return NULL;
}


/**
 * xfce_resource_lookup_all:
 * @type     : type of the resource to lookup.
 * @filename : relative file path. If @filename contains a trailing slash character
 *             it is taken to reference a directory, else it is taken to reference
 *             a file.
 *
 * Similar to #xfce_resource_lookup, but returns all resource of the specified @type,
 * that whose name is @filename.
 *
 * Return value:
 *
 * Since: 4.2
 **/
gchar**
xfce_resource_lookup_all (XfceResourceType type,
			  const gchar     *filename)
{
  GFileTest test;
  gchar     path[PATH_MAX];
  gchar   **paths;
  guint     size;
  guint     pos;
  GList    *l;

  g_return_val_if_fail (TYPE_VALID (type), NULL);
  g_return_val_if_fail (filename != NULL && strlen (filename) > 0, NULL);

  _res_init ();

  if (filename[strlen (filename) - 1] == G_DIR_SEPARATOR)
    test = G_FILE_TEST_IS_DIR;
  else
    test = G_FILE_TEST_IS_REGULAR;

  paths = g_new (gchar *, 11);
  size  = 10;
  pos   = 0;

  for (l = _list[type]; l != NULL; l = l->next)
    {
      g_snprintf (path, PATH_MAX, "%s/%s", (const gchar *) l->data, filename);

      if (g_file_test (path, test))
	{
	  if (pos == size)
	    {
	      size *= 2;
	      paths = g_realloc (paths, (size + 1) * sizeof (*paths));
	    }

	  paths[pos] = g_strdup (path);
	  ++pos;
	}
    }

  paths[pos] = NULL;

  return paths;
}


/**
 * xfce_resource_match:
 * @type      : type of the resource to locate directories for.
 * @pattern   : only accept filenames that fit to the pattern. The pattern
 *              needs to be a valid GPattern.
 * @unique    : if %TRUE, only return items which have unique suffixes.
 *
 * Tries to find all resources with the specified @type. The function will
 * look into all specified directories and return all filenames in these
 * directories. The returned filenames are given relative the base directories
 * specified by @type.
 *
 * If @pattern contains a trailing slash, #xfce_resource_match looks only for
 * directories that match @pattern, else it'll only look for regular files. In
 * case you are looking for directories, the returned entries will contain a
 * trailing slash as well, so you can easily use them with other resource
 * functions like #xfce_resource_lookup or #xfce_resource_save_location.
 *
 * Example: xfce_resource_match (XFCE_RESOURCE_CONFIG, "foo/bar*") will probably
 * return ("foo/bar", "foo/barbaz", ...).
 *
 * Return value: string array of all the relative paths whose names matches the
 *               given @pattern. The return value has to be freed using
 *               g_strfreev() when no longer needed.
 *
 * Since: 4.2
 **/
gchar**
xfce_resource_match (XfceResourceType type,
		     const gchar     *pattern,
		     gboolean         unique)
{
  gchar **paths;
  GList  *result = NULL;
  GList  *l;
  guint   n;

  g_return_val_if_fail (TYPE_VALID (type), NULL);
  g_return_val_if_fail (pattern != NULL, NULL);

  _res_init ();

  for (l = _list[type]; l != NULL; l = l->next)
    result = _res_match_path ((const gchar *) l->data, "", pattern, result);

  if (unique)
    result = _res_remove_duplicates (result);

  paths = g_new (gchar *, g_list_length (result) + 1);
  for (l = result, n = 0; l != NULL; l = l->next, ++n)
    paths[n] = (gchar *) l->data;
  paths[n] = NULL;
  g_list_free (result);

  return paths;
}


/**
 * xfce_resource_match_custom:
 * @type      :
 * @unique    :
 * @func      :
 * @user_data :
 *
 * Yet to be implemented!
 *
 * Return value:
 *
 * Since: 4.2
 **/
gchar**
xfce_resource_match_custom (XfceResourceType type,
			    gboolean         unique,
			    XfceMatchFunc    func,
			    gpointer         user_data)
{
  gchar **paths;
  GList  *result = NULL;
  GList  *l;
  guint   n;

  g_return_val_if_fail (TYPE_VALID (type), NULL);
  g_return_val_if_fail (func != NULL, NULL);

  if (unique)
    result = _res_remove_duplicates (result);

  paths = g_new (gchar *, g_list_length (result) + 1);
  for (l = result, n = 0; l != NULL; l = l->next, ++n)
    paths[n] = (gchar *) l->data;
  paths[n] = NULL;
  g_list_free (result);

  return paths;
}


/**
 * xfce_resource_push_path:
 * @type : type of the resource which search list should be expanded.
 * @path : search path to add.
 *
 * Appends @path to the search path list for @type. This function was
 * written primary for use within modules in larger applications, for example
 * MCS plugins.
 *
 * For example, if you need to add a specific path to the search path list
 * in your MCS, you should call #xfce_resource_push_path prior to calling
 * one of the resource search functions and call #xfce_resource_pop_path
 * right afterwards.
 *
 * Since: 4.2
 **/
void
xfce_resource_push_path (XfceResourceType type,
			 const gchar     *path)
{
  g_return_if_fail (TYPE_VALID (type));
  g_return_if_fail (path != NULL);

  _list[type] = g_list_append (_list[type], g_strdup (path));
}


/**
 * xfce_resource_pop_path:
 * @type : type of the resource which search list should be shrinked.
 *
 * Undoes the effect of the latest call to #xfce_resource_push_path. You
 * should take special care to call #xfce_resource_pop_path exactly same
 * times as #xfce_resource_push_path, everything else might result in
 * unwanted and maybe even undefined behaviour. You have been warned!
 *
 * Since: 4.2
 **/
void
xfce_resource_pop_path (XfceResourceType type)
{
  GList *l;

  g_return_if_fail (TYPE_VALID (type));

  l = g_list_last (_list[type]);
  if (G_LIKELY (l != NULL))
    {
      g_free (l->data);
      _list[type] = g_list_delete_link (_list[type], l);
    }
}


/**
 * xfce_resource_save_location:
 * @type    : type of location to return.
 * @relpath : relative path of the resource.
 * @create  : whether to create missing directory.
 *
 * If @relpath contains a trailing slash ('/') character, #xfce_resource_save_location 
 * finds the directory to save files into for the given type in the user's
 * home directory. All directories needed (including those given by
 * @relpath) will be created on demand if @create if %TRUE.
 *
 * If @relpath does not end with a slash ('/') character, it is taken to be
 * the name of a file to return the save location for. All the directories
 * needed will be created on demand if @create is %TRUE.
 *
 * Specifying %NULL or the empty string for @relpath allows you to discover
 * the base path for saving files of the specified @type, though normally
 * you should not need this.
 *
 * Return value: the path where resources of the specified @type should be
 *               saved or %NULL on error. The returned string should be freed
 *               when no longer needed.
 *
 * Since: 4.2
 **/
gchar*
xfce_resource_save_location (XfceResourceType type,
			     const gchar     *relpath,
			     gboolean         create)
{
  gchar *path;
  gchar *dir;

  g_return_val_if_fail (TYPE_VALID (type), NULL);

  _res_init ();

  if (G_UNLIKELY (relpath == NULL || *relpath == '\0'))
    return g_strdup (_save[type]);

  path = g_build_filename (_save[type], relpath, NULL);

  if (relpath[strlen (relpath) - 1] == G_DIR_SEPARATOR)
    {
      if (create && !xfce_mkdirhier (path, 0700, NULL))
	{
	  g_free (path);
	  path = NULL;
	}
    }
  else
    {
      dir = g_path_get_dirname (path);
      if (create && !xfce_mkdirhier (dir, 0700, NULL))
	{
	  g_free (path);
	  path = NULL;
	}
      g_free (dir);
    }

  return path;
}
