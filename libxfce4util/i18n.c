/* $Id$ */
/*-
 * Copyright (c) 2003-2004 Benedikt Meurer <benny@xfce.org>
 * Copyright (c) 2004 Jasper Huijsman <jasper@xfce.org>
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

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif
#ifdef HAVE_MEMORY_H
#include <memory.h>
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

#include <libxfce4util/i18n.h>
#include <libxfce4util/debug.h>

#ifndef HAVE_STRLCPY
#define strlcpy(x,y,z)	g_strlcpy(x,y,z)
#endif

#define DEFAULT_LOCALE	"C"

static gchar *
__localize_path(gchar *buffer, gsize len, const gchar *path, GFileTest test)
{
	const gchar *lang;
	char delim[] = { '.', '@', '_' };
	int i;

#ifdef HAVE_SETLOCALE
	if ((lang = setlocale(LC_MESSAGES, NULL)) == NULL)
#endif
		lang = getenv("LANG");

	if (lang != NULL && !strchr(lang, '/')) {
		/* ok, we will try four things here
		   - full locale name:         ll_LL@qualifier.encoding
		   - locale without encoding:  ll_LL@qualifier
		   - locale without qualifier: ll_LL
		   - base locale:              ll
		 */
		g_snprintf(buffer, len, "%s.%s", path, lang);

		if (g_file_test(buffer, test))
			goto found;

		for (i = 0; i < G_N_ELEMENTS(delim); i++) {
			char *langext, *p;
		    	char c = delim[i];
	
			if ((p = strchr(lang, c)) != NULL) {
				int s = p - lang;

				langext = g_new(char, s + 1);
				strlcpy(langext, lang, s + 1);
	
				g_snprintf(buffer, len, "%s.%s", path, langext);
				g_free(langext);

				if (g_file_test(buffer, test))
					goto found;
			}
		}
	}

	strlcpy(buffer, path, len);

found:
	return(buffer);
}


/**
 * xfce_get_file_localized:
 * @filename :
 *
 * Return value:
 **/
gchar*
xfce_get_file_localized(const gchar *filename)
{
	gchar buffer[PATH_MAX + 1];

	g_return_val_if_fail(filename != NULL, NULL);

	return(g_strdup(__localize_path(buffer, sizeof(buffer), filename,
					G_FILE_TEST_IS_REGULAR)));
}


/**
 * xfce_get_file_localized_r:
 * @buffer   :
 * @length   :
 * @filename :
 *
 * Return value:
 **/
gchar*
xfce_get_file_localized_r(gchar *buffer, gsize length, const gchar *filename)
{
	g_return_val_if_fail(buffer != NULL, NULL);
	g_return_val_if_fail(filename != NULL, NULL);

	return(__localize_path(buffer, length, filename,
				G_FILE_TEST_IS_REGULAR));
}


/**
 * xfce_get_dir_localized:
 * @directory :
 *
 * Return value:
 **/
gchar*
xfce_get_dir_localized(const gchar *directory)
{
	gchar buffer[PATH_MAX + 1];

	g_return_val_if_fail(directory != NULL, NULL);

	return(g_strdup(__localize_path(buffer, sizeof(buffer), directory,
					G_FILE_TEST_IS_DIR)));
}


/**
 * xfce_get_dir_localized_r:
 * @buffer    : 
 * @length    :
 * @directory :
 *
 * Return value:
 **/
gchar *
xfce_get_dir_localized_r(gchar *buffer, gsize length, const gchar *directory)
{
	g_return_val_if_fail(buffer != NULL, NULL);
	g_return_val_if_fail(directory != NULL, NULL);

	return(__localize_path(buffer, length, directory, G_FILE_TEST_IS_DIR));
}


/**
 * xfce_get_path_localized:
 * @dst: destination buffer.
 * @size: size of @dst in bytes.
 * @paths:
 * @filename:
 * @test:
 *
 * @paths is a ':'-separated list of pathnames.
 *
 *	%F	- The @filename
 *	%L	- The language string, as returned by 
 *		  setlocale(LC_MESSAGES, NULL)
 *	%l	- The language component of the language string
 *	%N	- application name
 *
 * Example paths:
 *
 *	/usr/local/lib/%L/%F:/usr/local/share/%N/%l/%F
 *
 * Return value:
 **/
gchar *
xfce_get_path_localized(gchar *dst, gsize size, const gchar *paths,
		        const gchar *filename, GFileTest test)
{
  const gchar *f;
  gchar *dstlast;
  gchar *d;
  const gchar *locale;
  gboolean need_lang = FALSE;

  g_return_val_if_fail(dst != NULL, NULL);
  g_return_val_if_fail(size > 2, NULL);
  g_return_val_if_fail(paths != NULL, NULL);

  d = dst;

  dstlast = dst + (size - 1);

#ifdef HAVE_SETLOCALE
  if ((locale = setlocale(LC_MESSAGES, NULL)) == NULL)
#endif
    if ((locale = g_getenv("LANGUAGE")) == NULL)
      if ((locale = g_getenv("LANG")) == NULL)
	locale = DEFAULT_LOCALE;

  for (; d < dstlast; ) {
    if (*paths == ':' || *paths == '\0') {
      *d = '\0';

      if (need_lang) {
	/* ok, we will try four things here:
	   - ll_LL@qualifier.encoding
	   - ll_LL@qualifier
	   - ll_LL
	   - ll
	 */
	char buffer [size];
	char delim[] = { '.', '@', '_' };
	int i;

	g_snprintf (buffer, size, dst, locale);

	DBG ("path : %s", buffer);
	
	if (g_file_test(buffer, test)) {
	  strncpy (dst, buffer, size);
	  return dst;
	}

	for (i = 0; i < G_N_ELEMENTS(delim); i++) {
	  char *langext, *p;
	  char c = delim[i];
	  
	  if ((p = strchr(locale, c)) != NULL) {
	    int s = p - locale;

	    langext = g_new(char, s + 1);
	    strlcpy(langext, locale, s + 1);

	    g_snprintf(buffer, size, dst, langext);
	    g_free(langext);

	    DBG ("path : %s", buffer);
	
	    if (g_file_test(buffer, test)) {
	      strncpy (dst, buffer, size);
	      return dst;
	    }
	  }
	}
      }
      else if (g_file_test(dst, test)) {
	return(dst);
      }

      if (*paths == ':') {
	d = dst;
	paths++;
	need_lang = FALSE;
	continue;
      }
      break;
    }

    if (*paths == '%') {
      if (*(paths + 1) == 'F') {
	/* 
	 * if "filename" is NULL, then simply skip
	 * the %F.
	 */
	if ((f = filename) != NULL)
	  while (*f && d < dstlast)
	    *d++ = *f++;

	paths += 2;
	continue;
      }
      else if (*(paths + 1) == 'L') {
	for (f = locale; *f && d < dstlast; )
	  *d++ = *f++;

	paths += 2;
	continue;
      }
      else if (*(paths + 1) == 'l') {
	if (d + 2 < dstlast) {
	  /* Ok if someone has a path with '%s' in it this will break.
	   * That should be against the law anyway IMO ;-) */
	  *d++ = '%';
	  *d++ = 's';
	  need_lang = TRUE;
	}

	paths += 2;
	continue;
      }
      else if (*(paths + 1) == 'N') {
	if ((f = g_get_prgname()) != NULL) 
	  while (*f && d < dstlast)
	    *d++ = *f++;

	paths += 2;
	continue;
      }
    }

    *d++ = *paths++;
  }

  return(NULL);
}


/**
 * xfce_locale_match:
 * @locale1 : the current locale value as returned by setlocale(LC_MESSAGES,%NULL).
 * @locale2 : the locale value to match against.
 *
 * The locale is of the general form LANG_COUNTRY.ENCODING @ MODIFIER, where
 * each of COUNTRY, ENCODING and MODIFIER can be absent.
 *
 * The match is done by actually removing the rightmost element one by one. This
 * is not entirely according to the freedesktop.org specification, but much easier.
 * Will probably be fixed in the future.
 *
 * Return value: an integer value indicating the level of matching, where
 *               the constant #XFCE_LOCALE_FULL_MATCH indicates a full match
 *               and #XFCE_LOCALE_NO_MATCH means no match. Every other value
 *               indicates a partial match, the higher the value, the better
 *               the match. You should not rely on any specific value besides
 *               the constants #XFCE_LOCALE_FULL_MATCH and #XFCE_LOCALE_NO_MATCH,
 *               since the range of returned values may change in the future.
 **/
guint
xfce_locale_match (const gchar *locale1,
		   const gchar *locale2)
{
  g_return_val_if_fail (locale1 != NULL, XFCE_LOCALE_NO_MATCH);
  g_return_val_if_fail (locale2 != NULL, XFCE_LOCALE_NO_MATCH);

  while (*locale1 == *locale2 && *locale1 != '\0')
    {
      ++locale1;
      ++locale2;
    }

  if (*locale1 == '\0')
    {
      if (*locale2 == '\0')
	return XFCE_LOCALE_FULL_MATCH;

      /* FALL-THROUGH */
    }
  else if (*locale2 == '\0')
    {
      switch (*locale1)
	{
	case '@': return XFCE_LOCALE_NO_MATCH + 3;
	case '.': return XFCE_LOCALE_NO_MATCH + 2;
	case '_': return XFCE_LOCALE_NO_MATCH + 1;
	}

      /* FALL-THROUGH */
    }

  return XFCE_LOCALE_NO_MATCH;
}
