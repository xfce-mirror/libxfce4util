/* $Id$ */
/*-
 * Copyright (c) 2003,2004 Benedikt Meurer <benny@xfce.org>
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

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libxfce4util/libxfce4util.h>

G_LOCK_DEFINE_STATIC (xdg_lock);

static gboolean   xdg_inited = FALSE;
static gchar     *xdg_cache_home;
static gchar     *xdg_data_home;
static gchar    **xdg_data_dirs;
static gchar     *xdg_config_home;
static gchar    **xdg_config_dirs;

/* Stolen from FreeBSD's mkdir(1) */
static int
xdg_build (const char *whole_path, mode_t omode)
{
  char path[1024];
	struct stat sb;
	mode_t numask, oumask;
	int first, last, retval;
	char *p;

  g_strlcpy (path, whole_path, sizeof (path));
	p = path;
	oumask = 0;
	retval = 0;
	if (p[0] == '/')		/* Skip leading '/'. */
		++p;
	for (first = 1, last = 0; !last ; ++p) {
		if (p[0] == '\0')
			last = 1;
		else if (p[0] != '/')
			continue;
		*p = '\0';
		if (p[1] == '\0')
			last = 1;
		if (first) {
			/*
			 * POSIX 1003.2:
			 * For each dir operand that does not name an existing
			 * directory, effects equivalent to those cased by the
			 * following command shall occcur:
			 *
			 * mkdir -p -m $(umask -S),u+wx $(dirname dir) &&
			 *    mkdir [-m mode] dir
			 *
			 * We change the user's umask and then restore it,
			 * instead of doing chmod's.
			 */
			oumask = umask(0);
			numask = oumask & ~(S_IWUSR | S_IXUSR);
			(void)umask(numask);
			first = 0;
		}
		if (last)
			(void)umask(oumask);
		if (mkdir(path, last ? omode : S_IRWXU | S_IRWXG | S_IRWXO) < 0) {
			if (errno == EEXIST || errno == EISDIR) {
				if (stat(path, &sb) < 0) {
					retval = 1;
					break;
				} else if (!S_ISDIR(sb.st_mode)) {
					if (last)
						errno = EEXIST;
					else
						errno = ENOTDIR;
					retval = 1;
					break;
				}
			} else {
				retval = 1;
				break;
			}
    }
		if (!last)
		    *p = '/';
	}
	if (!first && !last)
		(void)umask(oumask);
	return (retval);
}

static const gchar*
xdg_getenv (const gchar *variable, const gchar *fallback)
{
  static gchar buffer[1024];
  const gchar *result;
  
  result = g_getenv (variable);
  if (result == NULL)
    {
      if (*fallback == '~')
        {
          g_strlcpy (buffer, xfce_get_homedir(), sizeof (buffer));
          g_strlcat (buffer, fallback + 1, sizeof (buffer));

          result = buffer;
        }
      else
        {
          result = fallback;
        }
    }
  return result;
}

static gchar**
xdg_split_and_prepend (const gchar *dir_list, gchar *dir_first)
{
  gchar **result;
  gchar **dirs;
  gint n;

  dirs = g_strsplit (dir_list, ":", -1);
  for (n = 0; dirs[n] != NULL; ++n)
    ;

  result = g_new0 (gchar*, n + 2);
  result[0] = dir_first;

  for (n = 0; dirs[n] != NULL; ++n)
    result[n + 1] = g_strdup (dirs[n]);

  g_strfreev (dirs);

  return result;
}

static void
xdg_init (void)
{
  const gchar *dirs;
  const gchar *dir;

  G_LOCK (xdg_lock);

  if (xdg_inited)
    {
      G_UNLOCK (xdg_lock);
      return;
    }
  else
    xdg_inited = TRUE;

  /*
   * Cache
   */
  dir = xdg_getenv ("XDG_CACHE_HOME", "~/.cache");
  if (xdg_build (dir, 0700) != 0)
    {
      g_warning ("Invalid XDG_CACHE_HOME directory `%s', program may "
                 "behave incorrectly: %s", dir, g_strerror (errno));
    }
  xdg_cache_home = g_strdup (dir);

  /*
   * Data home
   */
  dir = xdg_getenv ("XDG_DATA_HOME", "~/.local/share");
  if (xdg_build (dir, 0700) != 0)
    {
      g_warning ("Invalid XDG_DATA_HOME directory `%s', program may "
                 "behave incorrectly: %s", dir, g_strerror (errno));
    }
  xdg_data_home = g_strdup (dir);

  /*
   * Config home
   */
  dir = xdg_getenv ("XDG_CONFIG_HOME", "~/.config");
  if (xdg_build (dir, 0700) != 0)
    {
      g_warning ("Invalid XDG_CONFIG_HOME directory `%s', program may "
                 "behave incorrectly: %s", dir, g_strerror (errno));
    }
  xdg_config_home = g_strdup (dir);

  /*
   * Data dirs
   */
  dirs = xdg_getenv ("XDG_DATA_DIRS", DEFAULT_XDG_DATA_DIRS);
  xdg_data_dirs = xdg_split_and_prepend (dirs, xdg_data_home);

  /*
   * Config dirs
   */
  dirs = xdg_getenv ("XDG_CONFIG_DIRS", DEFAULT_XDG_CONFIG_DIRS);
  xdg_config_dirs = xdg_split_and_prepend (dirs, xdg_config_home);

  G_UNLOCK (xdg_lock);
}

static gchar*
xdg_resource_path_local (const gchar *basedir, const gchar *resource)
{
  gchar *filename;

  filename = g_build_filename (basedir, resource, NULL);
  if (xdg_build (filename, 0700) != 0)
    {
      g_warning ("Unable to access or create directory `%s': %s",
                 filename, g_strerror (errno));
      g_free (filename);
      filename = NULL;
    }

  return filename;
}

static gchar*
xdg_resource_path (gchar **basedirs, const gchar *resource)
{
  gchar *path;

  for (; *basedirs != NULL; ++basedirs)
    {
      path = g_build_filename (*basedirs, resource, NULL);

      if (g_file_test (path, G_FILE_TEST_IS_DIR))
        return path;

      g_free (path);
    }

  return NULL;
}

static gchar**
xdg_resource_paths (gchar **basedirs, const gchar *resource)
{
  gchar **result;
  gchar *path;
  guint n;

  result = g_new0 (gchar*, 1);

  for (n = 0; *basedirs != NULL; ++basedirs)
    {
      path = g_build_filename (*basedirs, resource, NULL);

      if (g_file_test (path, G_FILE_TEST_IS_DIR))
        {
          result = g_realloc (result, (n + 2) * sizeof (gchar *));
          result[n] = path;
          ++n;
          result[n] = NULL;
        }
      else
        g_free (path);
    }

  return result;
}

/**
 * xfce_xdg_save_cache_path:
 * @resource: resource path below the cache dir. The string value must neither
 *            begin nor end with a slash '/'.
 *
 * Ensures that $XDG_CACHE_HOME/resource/ exist and returns its path.
 * @resource should normally be the name of your application. Use this
 * to store user specific non-essential data files.
 *
 * Return value: the path to the cache directory for @resource or NULL
 *               on error.
 **/
gchar*
xfce_xdg_save_cache_path (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_path_local (xdg_cache_home, resource);
}

/**
 * xfce_xdg_save_config_path:
 * @resource: resource path below the users config directory. The string
 *            must neither begin nor end with a slash '/'.
 *
 * Ensures that $XDG_CONFIG_HOME/resource/ exist and returns its path.
 * @resource should normally be the name of your application. Use this
 * when saving configuration settings. Use the xfce_xdg_load_config_path()
 * or xfce_xdg_load_config_paths() functions for loading.
 *
 * Return value: the path to the users config directory for @resource or
 *               NULL on error.
 **/
gchar*
xfce_xdg_save_config_path (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_path_local (xdg_config_home, resource);
}

/**
 * xfce_xdg_save_data_path:
 * @resource: resource path below the users data directory. The string
 *            must neither begin nor end with a slash '/'.
 *
 * Ensures that $XDG_CONFIG_HOME/resource/ exist and returns its path.
 * @resource is the name of some shared resource. Use this when updating
 * a shared (between programs) database. Use the xfce_xdg_load_data_path()
 * or xfce_xdg_load_data_paths() functions for loading.
 *
 * Return value: the path to the users config directory for @resource or
 *               NULL on error.
 **/
gchar*
xfce_xdg_save_data_path (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_path_local (xdg_data_home, resource);
}

/**
 * xfce_xdg_load_config_path:
 * @resource: config resource identifier, must neither begin nor end with
 *            the slash '/' character.
 *
 * Returns the absolute path to the first directory named @resource in the
 * configuration search path, where the users configuration directory
 * is searched first and afterwards the system wide configuration storages
 * as defined in the "Basedir specification".
 *
 * Return value: the path to the first directory named @resource in the
 *               configuration search path or NULL if no such directory
 *               exists.
 */
gchar*
xfce_xdg_load_config_path (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_path (xdg_config_dirs, resource);
}

/**
 * xfce_xdg_load_data_path:
 * @resource: data resource identifier, must neither begin nor end with
 *            the slash '/' character.
 *
 * Returns the absolute path to the first directory named @resource in the
 * datadir search path, where the users configuration directory
 * is searched first and afterwards the system wide configuration storages
 * as defined in the "Basedir specification".
 *
 * Return value: the path to the first directory named @resource in the
 *               datadir search path or NULL if no such directory
 *               exists.
 */
gchar*
xfce_xdg_load_data_path (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_path (xdg_data_dirs, resource);
}

/**
 * xfce_xdg_load_config_paths:
 * @resource: config resource identifier, must neither begin nor end with
 *            the slash '/' character.
 *
 * Returns a list of paths to directories named @resource in the configuration
 * search path.
 *
 * Return value: list of paths to directories named @resource in the
 *               configuration search path.
 */
gchar**
xfce_xdg_load_config_paths (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_paths (xdg_config_dirs, resource);
}

/**
 * xfce_xdg_load_data_paths:
 * @resource: config resource identifier, must neither begin nor end with
 *            the slash '/' character.
 *
 * Returns a list of paths to directories named @resource in the datadir
 * search path.
 *
 * Return value: list of paths to directories named @resource in the
 *               datadir search path.
 */
gchar**
xfce_xdg_load_data_paths (const gchar *resource)
{
  xdg_init ();

  return xdg_resource_paths (xdg_data_dirs, resource);
}

