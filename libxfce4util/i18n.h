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

#ifndef __LIBXFCE4UTIL_I18N_H
#define __LIBXFCE4UTIL_I18N_H

/*
 * gettext macros
 */
#if defined(ENABLE_NLS) && defined(PACKAGE)

#include <libintl.h>

#undef	_
#define	_(s)			(dgettext(PACKAGE, s))

#ifdef gettext_noop
#define N_(s)			(gettext_noop(s))
#else
#define N_(s)			(s)
#endif

#else /* !defined(ENABLE_NLS) || !defined(PACKAGE) */

#define _(s)	(s)
#define N_(s)	(s)

#define textdomain(s)				(s)
#define gettext(s)				(s)
#define dgettext(domain,s)			(s)
#define dcgettext(domain,s,type)		(s)
#define bindtextdomain(domain,directory)	(domain)

#endif

#endif	/* !__LIBXFCE4UTIL_I18N_H */
