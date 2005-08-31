/* $Id$ */
/*-
 * Copyright (c) 2003 Olivier Fourdan <fourdan@xfce.org>
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
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <libxfce4util/libxfce4util.h>

/* Around for compatibility. DEPRECATED! */
gchar*
utf8_string_remove_controls (gchar *str, gssize max_len, const gchar *end)
{
  return xfce_utf8_remove_controls (str, max_len, end);
}

/**
 * xfce_utf8_remove_controls:
 * @str     : target string.
 * @max_len : max characters to check or -1 for no character limit.
 * @end     : pointer to the endpoint in @str or %NULL for no endpoint.
 *
 * Removes all control characters from @str up to @end or up to
 * @max_len characters (note that characters does not mean bytes with
 * UTF-8), where both @str and @max_len may not be given.
 *
 * Control characters are replaced in @str by whitespaces, no new string
 * will be allocated. The operation is done in-place.
 *
 * Return value: pointer to @str or %NULL on error.
 *
 * Since: 4.2
 **/
gchar*
xfce_utf8_remove_controls (gchar *str, gssize max_len, const gchar *end)
{
    gchar *p;
    
    g_return_val_if_fail (str != NULL, NULL);
    
    p = str;
    while (p && *p && (!end || p < end) && (max_len < 0 || (p - str) < max_len))
    {
        if ((*p > 0) && (*p < 32))
            *p = ' ';
        p = g_utf8_find_next_char(p, end);
    }

    return str;
}

