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

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif

#ifdef HAVE_ERRNO_H
#include <errno.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libxfce4util/libxfce4util.h>


/**
 * xfce_mkdirhier:
 * @whole_path :
 * @omode      :
 * @error      : location where to store GError object to, error are returned
 *               in the %G_FILE_ERROR domain.
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

  if (!retval && error != NULL)
    {
      g_set_error (error, G_FILE_ERROR,
		   g_file_error_from_errno (errno),
		   _("Error creating directory '%s': %s"),
		   whole_path, g_strerror (errno));
    }

  return retval;
}


