/* $Id$ */
/*-
 * Copyright (c) 2003,2004 Benedikt Meurer <benny@xfce.org>
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

/**
 * @file libxfce4util/util.h Miscellaneous utility functions.
 * @author Benedikt Meurer <benny@xfce.org>
 */

#ifndef __LIBXFCE4UTIL_UTIL_H__
#define __LIBXFCE4UTIL_UTIL_H__

#include <glib.h>

#include <libxfce4util/debug.h>

/**
 * @defgroup util Miscellaneous utility functions.
 * @{
 */

/**
 * @brief Similar to g_get_home_dir() in functionality but will never return
 *        NULL.
 *
 * While g_get_home_dir() may return NULL under certain circumstances,
 * xfce_get_homedir() is garantied to never ever return NULL, but always
 * return a valid character pointer.
 *
 * The return value is a pointer to memory internal to libxfce4util and
 * must not be freed.
 *
 * @return The path to the current user's home directory.
 */
extern G_CONST_RETURN gchar *xfce_get_homedir(void);

/**
 * @brief Returns the path to a file in the current user's home directory.
 *
 * This function takes a NULL-terminated list of character pointers and
 * appends them to the path of the current user's home directory.
 *
 * Example:
 *
 * @code
 *  gchar *file = xfce_get_homefile("dir", "file.ext", NULL);
 * @endcode
 *
 * If this piece of code is executed on a usual UNIX install as user
 * @b benny, the value of @c file will be set to the string
 * @c "/home/benny/dir/file.ext".
 *
 * @return The absolute path to the speficied file in the current user's 
 *         home directory. The returned string must be freed using
 *         g_free() when no longer needed.
 */
#define xfce_get_homefile(first_element...)	\
	(g_build_filename(xfce_get_homedir(),	## first_element))

/**
 * @brief Similar in functionality to xfce_get_homefile() but uses a
 *        user defined buffer instead of allocating a new buffer.
 *
 * Example:
 *
 * @code
 *  char buffer[128];
 *  xfce_get_homefile_r(buffer, 128, "dir%sfile.ext", G_DIR_SEPARATOR_S);
 * @endcode
 *
 * This will place the string @c "/home/benny/dir/file.ext" given the
 * fact that you are logged in as user @b benny and that you are working
 * on an usual UNIX system.
 *
 * xfce_get_homefile_r() uses safe string operations, that is, it garanties
 * that the resulting string is always zero terminated, as long as the
 * @p length is greater than zero.
 *
 * @param buffer Pointer to a user provided destination buffer.
 * @param length Size of @p buffer in bytes.
 * @param format printf style format string.
 *
 * @return Pointer to @p buffer.
 */
extern G_CONST_RETURN gchar *xfce_get_homefile_r(gchar *buffer,
		size_t length, const gchar *format, ...);

/**
 * @brief Safe way to retrieve the path to users ".xfce4" directory.
 *
 * The path to the current user's ".xfce4" directory is either taken from
 * the environment variable XFCE4HOME, or if unset is gained by concatenating
 * the path to the current user's home directory and the string ".xfce4".
 * That says, it will by default return the path @c "$HOME/.xfce4",
 * where @c $HOME is replaced with the current user's home directory.
 *
 * The return value is a pointer to memory internal to libxfce4util and
 * must not be freed.
 *
 * @return The path to the current user's ".xfce4" directory.
 */
extern G_CONST_RETURN gchar *xfce_get_userdir(void);

/**
 * @brief To be done...
 */
#define xfce_get_userfile(first_element...)	\
	(g_build_filename(xfce_get_userdir(),	## first_element))

/**
 * @brief To be done...
 */
extern G_CONST_RETURN gchar *xfce_get_userfile_r(gchar *buffer,
		size_t length, const gchar *format, ...);

/**
 * @brief Joins the @p count character strings pointed to by @p strings
 *        using @p separator to a single string.
 *
 * @return The joined string. The returned string must be freed when
 *         no longer needed using g_free().
 */
extern gchar *xfce_strjoin(gchar *separator, gchar **strings, gint count);

/**
 * @brief Portable way to query the hostname of the current host.
 *
 * This function does not ever return NULL, but always returns a
 * string containing the current hostname.
 *
 * @return The hostname of the node running the current process. The string
 *         must be freed using g_free() when no longer needed.
 */
extern gchar *xfce_gethostname(void);

/**
 * @brief Portable replacement for putenv().
 *
 * @p string has to have the form @c ``name=value''. Calling xfce_putenv()
 * this way is equal to calling xfce_setenv(name, value, 1).
 *
 * @param string Character string in the form @c ``name=value''.
 *
 * @since 4.1.1
 *
 * @return 0 if the operation was successful; otherwise the global variable
 *         @p errno is set to indicate the error and a value of -1 is returned.
 */
extern int xfce_putenv(const gchar *string);

/**
 * @brief Inserts or resets the environment variable @p name in the
 *        current environment list.
 *
 * If the variable @p name does not exists in the list, it is inserted with
 * the given @p value. If the variable does exist, the argument @p overwrite
 * is tested; if @p overwrite is zero, the variable is not reset, otherwise
 * it is reset to the given @p value.
 *
 * On plattforms that provide a native setenv function, this function is
 * used, on all other plattforms setenv() is emulated using xfce_putenv().
 *
 * @param name Environment variable name.
 * @param value New value for the environment variable @p name.
 * @param overwrite Whether to overwrite existing variable value.
 *
 * @since 4.1.1
 *
 * @return 0 if the operation was successful; otherwise the global variable
 *         @p errno is set to indicate the error and a value of -1 is returned.
 */
extern int xfce_setenv(const gchar *name, const gchar *value,
                       gboolean overwrite);

/** @} */
/* end util */

#endif	/* __LIBXFCE4UTIL_UTIL_H__ */
