/*
 * Copyright (c) 2003 Benedikt Meurer (benedikt.meurer@unix-ag.uni-siegen.de)
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

#ifdef HAVE_ERR_H
#include <err.h>
#endif
#include <errno.h>
#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif

#include <glib.h>

#include "util.h"


#define XFCE4DIR		".xfce4"

/**
 * environment variable the user can set to change the path to
 * the users .xfce4 directory. If not set, the xfce_userdir defaults
 * to "$HOME/.xfce4".
 */
#define	XFCE4HOME_ENVVAR	"XFCE4HOME"

G_LOCK_DEFINE_STATIC(_lock);


static const gchar *xfce_homedir = NULL;/* path to users home directory */
static const gchar *xfce_userdir = NULL;/* path to users .xfce4 directory */

/**
 * Wrapper around gtk_init to to some stuff required for Xfce4 apps
 */
static void
initialize(void)
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

/**
 * This is garantied to never return NULL, unlike g_get_home_dir()
 */
G_CONST_RETURN gchar *
xfce_get_homedir(void)
{
	G_LOCK(_lock);
	if (!xfce_homedir)
		initialize();
	G_UNLOCK(_lock);
	
	return(xfce_homedir);
}

G_CONST_RETURN gchar *
xfce_get_userdir(void)
{
	G_LOCK(_lock);
	if (!xfce_userdir)
		initialize();
	G_UNLOCK(_lock);

	return(xfce_userdir);
}


