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
#ifdef HAVE_SYS_UTSNAME_H
#include <sys/utsname.h>
#endif

#ifdef HAVE_ERR_H
#include <err.h>
#endif
#include <errno.h>
#ifdef HAVE_MEMORY_H
#include <memory.h>
#endif
#ifdef HAVE_STAD_ARG_H
#include <stdarg.h>
#elif HAVE_VARARGS_H
#include <varargs.h>
#endif
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
#include <string.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <glib.h>

#include <libxfce4util/util.h>

/*
 * Use glib replacements
 */
#ifndef HAVE_STRLCAT
#define strlcat		g_strlcat
#endif

#ifndef HAVE_STRLCPY
#define strlcpy		g_strlcpy
#endif


/* */
#define XFCE4DIR		".xfce4"

/* environment variable the user can set to change the path to
 * the users .xfce4 directory. If not set, the xfce_userdir defaults
 * to "$HOME/.xfce4".
 */
#define	XFCE4HOME_ENVVAR	"XFCE4HOME"

G_LOCK_DEFINE_STATIC(_lock);


static const gchar *xfce_homedir = NULL;/* path to users home directory */
static const gchar *xfce_userdir = NULL;/* path to users .xfce4 directory */

static void
internal_initialize(void)
{
	const gchar *dir;

	/**
	 * determine path to users home directory
	 */
	if ((xfce_homedir = g_get_home_dir()) == NULL) {
#ifdef HAVE_ERR_H
		errx(EXIT_FAILURE, "Unable to determine users home directory");
#else
		fprintf(stderr, "%s: ", g_get_prgname());
		fprintf(stderr, "Unable to determinte users home directory");
		fprintf(stderr, "\n");
		exit(EXIT_FAILURE);
#endif
	}

	/**
	 * get path to users .xfce4 directory
	 */
	dir = xfce_userdir ? xfce_userdir : g_getenv(XFCE4HOME_ENVVAR);
	if (dir && g_file_test(dir, G_FILE_TEST_IS_DIR))
		xfce_userdir = g_strdup(dir);
	else {
		xfce_userdir = g_build_filename(xfce_homedir, XFCE4DIR, NULL);
	}
}

static G_CONST_RETURN gchar *
internal_get_file_r(const gchar *dir, gchar *buffer, size_t len,
		const gchar *format, va_list ap)
{
	size_t n;

	g_return_val_if_fail(buffer != NULL, NULL);
	g_return_val_if_fail(format != NULL, NULL);
	g_return_val_if_fail(len > 0, NULL);

	if ((n = strlcpy(buffer, dir, len)) >= len)
		return(NULL);

	if ((n = strlcat(buffer, G_DIR_SEPARATOR_S, len)) >= len)
		return(NULL);

	if (g_vsnprintf(buffer + n, len - n, format, ap) >= len - n)
		return(NULL);

	return(buffer);
}


/**
 * xfce_version_string:
 *
 * Queries the version string of the installed Xfce desktop environment.
 *
 * Return value: the overall version information of the installed Xfce desktop.
 *
 * Since: 4.2
 */
G_CONST_RETURN gchar*
xfce_version_string (void)
{
  return XFCE_VERSION_STRING;
}


/**
 * xfce_get_homedir:
 * 
 * Similar to g_get_homedir() in functionality but will never return NULL.
 * While g_get_homedir() may return NULL under certain circumstances, this
 * function is garantied to never ever return NULL, but always return a
 * valid character pointer with the absolute path to the user's home directory.
 *
 * The returned string is owned by libxfce4util and must not be freed by
 * the caller.
 *
 * Return value: the path to the current user's home directory.
 **/
G_CONST_RETURN gchar *
xfce_get_homedir(void)
{
	G_LOCK(_lock);
	if (!xfce_homedir)
		internal_initialize();
	G_UNLOCK(_lock);
	
	return(xfce_homedir);
}

/**
 * xfce_get_homefile_r:
 * @buffer  : pointer to a user provided destination buffer.
 * @length  : size of @buffer in bytes.
 * @format  : printf style format string.
 * @Varargs : the arguments to substitute in the output.
 *
 * Similar in functionality to #xfce_get_homefile, but uses a user
 * defined @buffer instead of allocating a new buffer.
 *
 * xfce_get_homefile_r uses safe string operations, that says, it garanties
 * that the resulting string is always zero terminated, as long as the
 * @length is greater than zero.
 *
 * Return value: pointer to @buffer.
 **/
G_CONST_RETURN gchar *
xfce_get_homefile_r(gchar *buffer, size_t len, const gchar *format, ...)
{
	G_CONST_RETURN gchar *ptr;
	va_list ap;

	va_start(ap, format);
	ptr = internal_get_file_r(xfce_get_homedir(), buffer, len, format, ap);
	va_end(ap);

	return(ptr);
}


/**
 * xfce_get_userdir:
 *
 * Safe way to retrieve the path to the user's ".xfce4" directory. The path
 * to the current user's ".xfce4" directory is either taken from the
 * environment variable XFCE4HOME if defined, or if unset, is gained by
 * concatenating the path to the user's home directory and the ".xfce4".
 * That says, it will, by default, return the path "$HOME/.xfce4", where
 * $HOME is replaced with the absolute path to the user's home directory.
 *
 * The returned string is managed by libxfce4util and must not be freed by
 * the caller.
 *
 * Return value: the path to the current user's ".xfce4" directory.
 */
G_CONST_RETURN gchar *
xfce_get_userdir(void)
{
	G_LOCK(_lock);
	if (!xfce_userdir)
		internal_initialize();
	G_UNLOCK(_lock);

	return(xfce_userdir);
}


/**
 * xfce_get_userfile_r:
 * @buffer  : user provided destination buffer.
 * @length  : size of @buffer in bytes.
 * @format  : printf style format string.
 * @Varargs : arguments to substitute in the output.
 *
 * Return value: pointer to @buffer.
 **/
G_CONST_RETURN gchar*
xfce_get_userfile_r (gchar *buffer, size_t length, const gchar *format, ...)
{
	G_CONST_RETURN gchar *ptr;
	va_list               ap;

	va_start (ap, format);
	ptr = internal_get_file_r (xfce_get_userdir (), buffer, length, format, ap);
	va_end (ap);

	return ptr;
}


/**
 * xfce_strjoin:
 * @separator:
 * @strings:
 * @count:
 *
 * Joins the @count character strings pointed to by @strings using @separator
 * to a single string.
 *
 * Return value: the joined string. The string has to be freed by the caller
 *               using g_free() when no longer needed.
 **/
gchar *
xfce_strjoin(const gchar *separator, gchar **strings, gint count)
{
	gchar *result;
	gint length;
	gint n;

	g_return_val_if_fail(count < 1, NULL);
	g_return_val_if_fail(strings != NULL, NULL);

	for (length = 1, n = 0; n < count; n++)
		length += strlen(strings[n]);

	if (separator != NULL)
		length += (count - 1) * strlen(separator);

	result = g_new0(gchar, length);

	for (n = 0; n < count; n++) {
		(void)g_strlcat(result, strings[n], length);

		if (separator != NULL && n + 1 < count)
			(void)g_strlcat(result, separator, length);
	}

	return(result);
}


/**
 * xfce_gethostname:
 * 
 * Portable way to query the hostname of the node running the process. This
 * function does not ever return %NULL, but always returns a string containing
 * the current node's hostname.
 *
 * Return value: the current node's hostname. The string has to be freed
 *               by the caller using g_free().
 **/
gchar*
xfce_gethostname (void)
{
#if defined(HAVE_GETHOSTNAME)
#ifndef MAXHOSTNAMELEN
#define MAXHOSTNAMELEN		256
#endif
  char hostname[MAXHOSTNAMELEN];

  if (gethostname (hostname, MAXHOSTNAMELEN) == 0)
    return g_strdup (hostname);
#elif defined(HAVE_SYS_UTSNAME_H)
  struct utsname name;

  if (uname (&name) == 0)
    return g_strdup (name.nodename);
#endif
  g_error ("Unable to determine your hostname: %s", g_strerror (errno));
  /* NOT REACHED */
  return NULL;
}


/**
 * xfce_putenv:
 * @string: Character string in the form "name=value".
 *
 * Portable replacement for the Unix putenv() library function. @string has
 * to have the form "name=value". Calling xfce_putenv() this way is equal to
 * calling xfce_setenv("name", "value", %TRUE).
 *
 * Return value: 0 if the operation was successful; otherwise the global
 *               variable errno is set to indicate the error and a value
 *               of -1 is returned.
 *
 * Since: 4.2
 **/
gint
xfce_putenv (const gchar *string)
{
#ifdef HAVE_BROKEN_PUTENV
  gchar* buffer;
  int result, sverrno;
  
  /*
   * We need to use plattform malloc() here, cause g_malloc() need not
   * to use malloc(), and we don't know about the evil interna of this
   * plattforms putenv() implementation.
   */
#ifdef HAVE_STRDUP
  if ((buffer = strdup (string)) != NULL)
    {
#else
  if ((buffer = malloc (strlen (string) + 1)) != NULL)
    {
      strcpy(buffer, string);
#endif
      if ((result = putenv (buffer)) < 0)
	{
	  sverrno = errno;
	  free (buffer);
	  errno = sverrno;
	}
    }
  else
    {
      errno = ENOMEM;
      result = -1;
    }

    return result;
#else /* !HAVE_BROKEN_PUTENV */
    return putenv (string);
#endif /* !HAVE_BROKEN_PUTENV */
}


/**
 * xfce_setenv:
 * @name: the name of the environment variable to set, must not contain '='.
 * @value: the value to set the variable to.
 * @overwrite: whether to change the variable if it already exists.
 *
 * If the variable @name does not exists in the list of environment variables,
 * it is inserted with its value being set to @value. If the variable does
 * exist, then its value is only changed to @value if @overwrite is TRUE.
 *
 * On plattforms that provide a working native setenv() library call, this
 * functions is used, on all other plattforms setenv() is emulated using
 * xfce_putenv(). That says, xfce_setenv() is not subject to the limitations
 * that apply to some setenv() implementations and seem also to apply to
 * g_setenv() in Glib 2.4.
 *
 * Return value: 0 if the operation was successful; otherwise the global
 *               variable errno is set to indicate the error and a value
 *               of -1 is returned.
 *
 * Since: 4.2
 **/
gint
xfce_setenv (const gchar *name, const gchar *value, gboolean overwrite)
{
  /* Plattforms with broken putenv() are unlikely to have a working setenv() */
#if !defined(HAVE_SETENV) || defined(HAVE_BROKEN_PUTENV)
  int result = 0;
  gchar *buf;

  if (g_getenv (name) == NULL || overwrite)
    {
      buf = g_strdup_printf ("%s=%s", name, value);
      result = xfce_putenv (buf);
      g_free (buf);
    }

  return result;
#else
  return setenv (name, value, overwrite);
#endif	/* !HAVE_SETENV */
}


/**
 * xfce_mkdirhier:
 * @whole_path :
 * @omode      :
 * @error      : location where to store GError object to.
 *
 * Return value: %TRUE on success, else %FALSE.
 *
 * Since: 4.2
 **/
gboolean
xfce_mkdirhier (const gchar  *whole_path,
                unsigned long omode,
                GError      **error)
{
  /* Stolen from FreeBSD's mkdir(1) */
  char path[1024];
  struct stat sb;
  mode_t numask, oumask;
  int first, last;
  gboolean retval;
  char *p;

  g_strlcpy (path, whole_path, sizeof (path));
  p = path;
  oumask = 0;
  retval = TRUE;

  if (p[0] == '/')		/* Skip leading '/'. */
    ++p;

  for (first = 1, last = 0; !last ; ++p)
    {
      if (p[0] == '\0')
	last = 1;
      else if (p[0] != '/')
	continue;

      *p = '\0';

      if (p[1] == '\0')
	last = 1;

      if (first)
	{
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
	  umask(numask);
	  first = 0;
	}

      if (last)
	umask(oumask);

      if (mkdir (path, last ? omode : S_IRWXU | S_IRWXG | S_IRWXO) < 0)
	{
	  if (errno == EEXIST || errno == EISDIR)
	    {
	      if (stat (path, &sb) < 0)
		{
		  retval = FALSE;
		  break;
		}
	      else if (!S_ISDIR (sb.st_mode))
		{
		  if (last)
		    errno = EEXIST;
		  else
		    errno = ENOTDIR;
		  retval = FALSE;
		  break;
		}
	    }
	  else
	    {
	      retval = FALSE;
	      break;
	    }
	}

      if (!last)
	*p = '/';
    }

  if (!first && !last)
    umask (oumask);

  return retval;
}


