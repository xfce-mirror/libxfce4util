/*
 * Copyright (c) 2003 Benedikt Meurer <benedikt.meurer@unix-ag.uni-siegen.de>
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

/*-
 * Copyright (c)2002 YAMAMOTO Takashi,
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
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

#ifndef HAVE_STRLCPY
#define strlcpy(x,y,z)	g_strlcpy(x,y,z)
#endif

#define DEFAULT_LOCALE	"C"

#if defined(__NetBSD__) && defined(HAVE___UNALIASNAME)
/*
 * NetBSD has the __unaliasname function in -lc
 */

extern const char *__unaliasname(const char *, const char *, void *, size_t);

#else
G_INLINE_FUNC int
__is_ws(gchar ch)
{
	return(ch == ' ' || ch == '\t');
}

G_INLINE_FUNC G_CONST_RETURN gchar *
__unaliasname(const gchar *dbname, const gchar *alias, gpointer buf,
		gsize bufsize)
{
	FILE *fp;
	const gchar *result = alias;
	gsize resultlen;
	gsize aliaslen;
	const gchar *p;
	gchar __buf[PATH_MAX + 1];
	gsize len;

	if ((fp = fopen(dbname, "r")) == NULL)
		goto quit;

	aliaslen = strlen(alias);

	for (;;) {
		if ((p = fgets(__buf, sizeof(__buf), fp)) == NULL)
			goto quit; /* eof or error */

		len = strlen(p);

		/* ignore terminating NL */
		if (p[len - 1] == '\n')
			len--;

		/* ignore null line and comment */
		if (len == 0 || p[0] == '#')
			continue;

		if (aliaslen > len)
			continue;

		if (memcmp(alias, p, aliaslen))
			continue;

		p += aliaslen;
		len -= aliaslen;

		if (len == 0 || !__is_ws(*p))
			continue;

		/* entry was found here */
		break;

		/* NOTREACHED */
	}

	/* skip white spaces */
	do {
		p++;
		len--;
	} while (len != 0 && __is_ws(*p));

	if (len == 0)
		goto quit;

	/* count length of result */
	resultlen = 0;
	while (resultlen < len && !__is_ws(*p))
		resultlen++;

	/* check if space is enough */
	if (bufsize < resultlen + 1)
		goto quit;

	memcpy(buf, p, resultlen);
	((gchar *)buf)[resultlen] = 0;
	result = buf;

quit:
	if (fp)
		fclose(fp);

	return(result);
}
#endif

static gchar *
__localize_path(gchar *buffer, gsize len, const gchar *path, GFileTest test)
{
	gchar langbuf[PATH_MAX];
	const gchar *lang;

#ifdef HAVE_SETLOCALE
	if ((lang = setlocale(LC_MESSAGES, NULL)) == NULL)
#endif
		lang = getenv("LANG");

	if (lang != NULL && !strchr(lang, '/')) {
		lang = __unaliasname(NLS_ALIAS_DB, lang, langbuf,
				sizeof(langbuf));

		g_snprintf(buffer, len, "%s.%s", path, lang);

		if (g_file_test(buffer, test))
			goto found;
	}

	strlcpy(buffer, path, len);

found:
	return(buffer);
}

gchar *
xfce_get_file_localized(const gchar *filename)
{
	gchar buffer[PATH_MAX + 1];

	g_return_val_if_fail(filename != NULL, NULL);

	return(g_strdup(__localize_path(buffer, sizeof(buffer), filename,
					G_FILE_TEST_IS_REGULAR)));
}

gchar *
xfce_get_file_localized_r(gchar *buffer, gsize length, const gchar *filename)
{
	g_return_val_if_fail(buffer != NULL, NULL);
	g_return_val_if_fail(filename != NULL, NULL);

	return(__localize_path(buffer, length, filename,
				G_FILE_TEST_IS_REGULAR));
}

gchar *
xfce_get_dir_localized(const gchar *directory)
{
	gchar buffer[PATH_MAX + 1];

	g_return_val_if_fail(directory != NULL, NULL);

	return(g_strdup(__localize_path(buffer, sizeof(buffer), directory,
					G_FILE_TEST_IS_DIR)));
}

gchar *
xfce_get_dir_localized_r(gchar *buffer, gsize length, const gchar *directory)
{
	g_return_val_if_fail(buffer != NULL, NULL);
	g_return_val_if_fail(directory != NULL, NULL);

	return(__localize_path(buffer, length, directory, G_FILE_TEST_IS_DIR));
}

/*
 * paths is a ':'-separated list of pathnames.
 *
 *	%F	- The filename
 *	%L	- The language string, as returned by 
 *		  setlocale(LC_MESSAGES, NULL)
 *	%l	- The language component of the language string
 *	%N	- application name
 *
 * Example paths:
 *
 *	/usr/local/lib/%L/%F:/usr/local/share/%N/%l/%F
 */
gchar *
xfce_get_path_localized(gchar *dst, gsize size, const gchar *paths,
		        const gchar *filename, GFileTest test)
{
	const gchar *f;
	gchar *dstlast;
	gchar *d;
	const gchar *locale;
	const gchar *lang;
	gchar langbuf[PATH_MAX];

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

	lang = __unaliasname(NLS_ALIAS_DB, locale, langbuf, sizeof(langbuf));

	for (; d < dst + (size - 1); ) {
		if (*paths == ':' || *paths == '\0') {
			*d = '\0';

			if (g_file_test(dst, test))
				return(dst);

			if (*paths == ':') {
				d = dst;
				paths++;
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
				for (f = lang; *f && d < dstlast; )
					*d++ = *f++;

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


