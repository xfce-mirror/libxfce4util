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

#ifndef __LIBXFCE4UTIL_FILTER_H__
#define __LIBXFCE4UTIL_FILTER_H__

#include <glib.h>


typedef struct _XfceFilter	XfceFilter;
typedef struct _XfceFilterList	XfceFilterList;

struct _XfceFilter
{
	guint	argc;
	gchar	**argv;
	gchar	*command;
	guint	size;
};

struct _XfceFilterList
{
	GList	*cursor;
	GList	*filters;
};

/* filter function prototypes */
extern XfceFilter	*xfce_filter_new(const gchar *);
extern void		xfce_filter_free(XfceFilter *);
extern void		xfce_filter_add(XfceFilter *, const gchar *, ...);

/* filterlist function prototypes */
extern XfceFilterList	*xfce_filterlist_new(void);
extern void		xfce_filterlist_free(XfceFilterList *);
extern void		xfce_filterlist_append(XfceFilterList *, XfceFilter *);
extern void		xfce_filterlist_prepend(XfceFilterList *, XfceFilter *);
extern int		xfce_filterlist_execute(XfceFilterList *, int,int,int);

#endif	/* !__LIBXFCE4UTIL_FILTER_H__ */
