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
#endif /* !HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>

#include <glib.h>

#include "option.h"


const gchar *
xfce_option(const XfceOption *options, const gchar *name)
{
	g_return_val_if_fail(name != NULL, NULL);
	g_return_val_if_fail(options != NULL, NULL);

	for (; options->name; options++)
		if (!strcmp(options->name, name))
			return(options->name);

	return(NULL);
}

const gchar *
xfce_option_alias(const XfceOption *options, const gchar *name)
{
	g_return_val_if_fail(name != NULL, NULL);
	g_return_val_if_fail(options != NULL, NULL);

	for (; options->alias; options++)
		if (!strcmp(options->name, name))
			return(options->alias);

	return(NULL);
}

GList *
xfce_option_list(const XfceOption *options)
{
	GList *list;

	g_return_val_if_fail(options != NULL, NULL);

	for (list = NULL; options->alias; options++)
		list = g_list_append(list, (gpointer)options->alias);

	return(list);
}

const gchar *
xfce_option_name(const XfceOption *options, const gchar *alias)
{
	g_return_val_if_fail(alias != NULL, NULL);
	g_return_val_if_fail(options != NULL, NULL);

	for (; options->name; options++)
		if (!strcmp(options->alias, alias))
			return(options->name);

	return(NULL);
}

