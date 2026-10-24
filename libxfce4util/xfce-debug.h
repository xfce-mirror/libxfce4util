/* $Id$ */
/*-
 * Copyright (c) 2003-2005 Benedikt Meurer <benny@xfce.org>
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA
 */

#if !defined(_LIBXFCE4UTIL_INSIDE_LIBXFCE4UTIL_H) && !defined(LIBXFCE4UTIL_COMPILATION)
#error "Only <libxfce4util/libxfce4util.h> can be included directly, this file may disappear or change contents"
#endif

#ifndef __LIBXFCE4UTIL_DEBUG_H__
#define __LIBXFCE4UTIL_DEBUG_H__

#include <glib.h>
#include <stdio.h>

#if defined(DEBUG) && (DEBUG > 0) && (defined(G_HAVE_ISO_VARARGS) || defined(G_HAVE_GNUC_VARARGS))

#include <time.h>
#include <stdlib.h>

typedef struct { char timestamp[32]; } xfce_debug_timestamp;

static inline xfce_debug_timestamp xfce_debug_create_timestamp(void)
{
  xfce_debug_timestamp timestamp;
  struct timespec timespec;

  clock_gettime(CLOCK_REALTIME, &timespec);
  struct tm *tm = localtime(&timespec.tv_sec);

  if (tm) {
    size_t offset = strftime(timestamp.timestamp, sizeof(timestamp.timestamp), "%FT%T", tm);
    snprintf(timestamp.timestamp + offset, sizeof(timestamp.timestamp) - offset, ".%03ld%c%02ld:%02ld",
	     timespec.tv_nsec / 1000000, tm->tm_gmtoff >= 0 ? '+' : '-',
	     labs(tm->tm_gmtoff) / 3600, labs(tm->tm_gmtoff) % 3600 / 60);
  } else {
    *timestamp.timestamp = '\0';
  }

  return timestamp;
}

#define DBG_PRINT_PREFIX(label, file, line, func) \
    fprintf (stderr, ("[%s] " label "[%s:%d] %s(): "), xfce_debug_create_timestamp().timestamp, file, line, func)

#if defined(__NetBSD__) || (defined(__STDC_VERSION__) && __STDC_VERSION__ >= 199901L)
#define __DBG_FUNC__ __func__
#elif defined(__GNUC__) && __GNUC__ >= 3
#define __DBG_FUNC__ __FUNCTION__
#elif defined(__SVR4) && defined(__sun)
#define __DBG_FUNC__ __func__
#else
#define __DBG_FUNC__ "??"
#endif

#if defined(G_HAVE_ISO_VARARGS)

#define DBG(...) \
  G_STMT_START \
  { \
    DBG_PRINT_PREFIX("DBG", __FILE__, __LINE__, __DBG_FUNC__); \
    fprintf (stderr, __VA_ARGS__); \
    fprintf (stderr, "\n"); \
  } \
  G_STMT_END

#elif defined(G_HAVE_GNUC_VARARGS)

#define DBG(fmt, args...) \
  G_STMT_START \
  { \
    { \
      DBG_PRINT_PREFIX("DBG", __FILE__, __LINE__, __DBG_FUNC__); \
      fprintf (stderr, fmt, ##args); \
      fprintf (stderr, "\n"); \
    } \
    G_STMT_END

#endif

#if defined(DEBUG_TRACE) && (DEBUG_TRACE > 0)

#if defined(G_HAVE_ISO_VARARGS)

#define TRACE(...) \
  G_STMT_START \
  { \
    DBG_PRINT_PREFIX("TRACE", __FILE__, __LINE__, __DBG_FUNC__); \
    fprintf (stderr, __VA_ARGS__); \
    fprintf (stderr, "\n"); \
  } \
  G_STMT_END

#elif defined(G_HAVE_GNUC_VARARGS)

#define TRACE(fmt, args...) \
  G_STMT_START \
  { \
    { \
      DBG_PRINT_PREFIX("TRACE", __FILE__, __LINE__, __DBG_FUNC__); \
      fprintf (stderr, fmt, ##args); \
      fprintf (stderr, "\n"); \
    } \
    G_STMT_END

#endif

#else /* !defined(DEBUG_TRACE) || DEBUG_TRACE <= 0 */

#define TRACE(...) \
  G_STMT_START { (void) 0; } \
  G_STMT_END

#endif

#else /* !defined(DEBUG) || DEBUG <= 0 */

#define DBG(...) \
  G_STMT_START { (void) 0; } \
  G_STMT_END
#define TRACE(...) \
  G_STMT_START { (void) 0; } \
  G_STMT_END

#endif

#endif /* !__LIBXFCE4UTIL_DEBUG_H__ */
