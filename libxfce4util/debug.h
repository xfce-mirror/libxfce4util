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

#ifndef __LIBXFCE4UTIL_DEBUG_H__
#define __LIBXFCE4UTIL_DEBUG_H__

#if defined(DEBUG) && DEBUG > 0

#include <stdio.h>

#if defined(__NetBSD__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __DBG_FUNC__    __func__
#elif defined(__GNUC__) && __GNUC__ >= 3
#define __DBG_FUNC__	__FUNCTION__
#else
#define __DBG_FUNC__    "??"
#endif

#define DBG(fmt, args...)                                                   \
{                                                                           \
    fprintf(stderr, "DBG[%s:%d] %s(): ", __FILE__, __LINE__, __DBG_FUNC__); \
    fprintf(stderr, fmt, ##args);                                           \
    fprintf(stderr, "\n");                                                  \
}

#if defined(DEBUG_TRACE) && DEBUG_TRACE > 0

#define TRACE(fmt, args...)						    \
{                                                                           \
    fprintf(stderr, "TRACE[%s:%d] %s(): ",__FILE__,__LINE__,__DBG_FUNC__);  \
    fprintf(stderr, fmt, ##args);                                           \
    fprintf(stderr, "\n");                                                  \
}

#else /* !defined(DEBUG_TRACE) || DEBUG_TRACE <= 0 */

#define TRACE(fmt, args...) { do {} while(0); }

#endif

#else /* !defined(DEBUG) || DEBUG <= 0 */

#define DBG(fmt, args...)   { do {} while(0); }
#define TRACE(fmt, args...) { do {} while(0); }

#endif

#endif	/* !__LIBXFCE4UTIL_DEBUG_H__ */
