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

#ifndef __LIBXFCE4UTIL_DEBUG_H__
#define __LIBXFCE4UTIL_DEBUG_H__

#include <stdio.h>

#include <glib.h>

#if defined(DEBUG) && (DEBUG > 0) && (defined(G_HAVE_ISO_VARARGS) \
                                        || defined(G_HAVE_GNUC_VARARGS))

#if defined(__NetBSD__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __DBG_FUNC__    __func__
#elif defined(__GNUC__) && __GNUC__ >= 3
#define __DBG_FUNC__	__FUNCTION__
#else
#define __DBG_FUNC__    "??"
#endif

#if defined(G_HAVE_ISO_VARARGS)

#define DBG(...)                G_STMT_START{                               \
    fprintf(stderr, "DBG[%s:%d] %s(): ", __FILE__, __LINE__, __DBG_FUNC__); \
    fprintf(stderr, __VA_ARGS__);                                           \
    fprintf(stderr, "\n");                                                  \
}G_STMT_END

#elif defined(G_HAVE_GNUC_VARARGS)

#define DBG(fmt, args...)       G_STMT_START{                               \
{                                                                           \
    fprintf(stderr, "DBG[%s:%d] %s(): ", __FILE__, __LINE__, __DBG_FUNC__); \
    fprintf(stderr, fmt, ##args);                                           \
    fprintf(stderr, "\n");                                                  \
}G_STMT_END

#endif

#if defined(DEBUG_TRACE) && (DEBUG_TRACE > 0)

#if defined(G_HAVE_ISO_VARARGS)

#define TRACE(...)              G_STMT_START{                               \
    fprintf(stderr, "TRACE[%s:%d] %s(): ",__FILE__,__LINE__,__DBG_FUNC__);  \
    fprintf(stderr, __VA_ARGS__);                                           \
    fprintf(stderr, "\n");                                                  \
}G_STMT_END

#elif defined (G_HAVE_GNUC_VARARGS)

#define TRACE(fmt, args...)     G_STMT_START{                               \
{                                                                           \
    fprintf(stderr, "TRACE[%s:%d] %s(): ",__FILE__,__LINE__,__DBG_FUNC__);  \
    fprintf(stderr, fmt, ##args);                                           \
    fprintf(stderr, "\n");                                                  \
}G_STMT_END

#endif

#else /* !defined(DEBUG_TRACE) || DEBUG_TRACE <= 0 */

#define TRACE(...) G_STMT_START{ (void)0; }G_STMT_END

#endif

#else /* !defined(DEBUG) || DEBUG <= 0 */

#define DBG(...)   G_STMT_START{ (void)0; }G_STMT_END
#define TRACE(...) G_STMT_START{ (void)0; }G_STMT_END

#endif

#endif	/* !__LIBXFCE4UTIL_DEBUG_H__ */
