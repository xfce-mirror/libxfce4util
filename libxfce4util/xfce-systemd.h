/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*-
 *
 * Copyright (C) 2012 Christian Hesse
 *
 * Licensed under the GNU General Public License Version 2
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#if !defined(_LIBXFCE4UTIL_INSIDE_LIBXFCE4UTIL_H) && !defined(LIBXFCE4UTIL_COMPILATION)
#error "Only <libxfce4util/libxfce4util.h> can be included directly, this file may disappear or change contents"
#endif

#ifndef __XFCE_SYSTEMD_H__
#define __XFCE_SYSTEMD_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define XFCE_TYPE_SYSTEMD (xfce_systemd_get_type ())
G_DECLARE_FINAL_TYPE (XfceSystemd, xfce_systemd, XFCE, SYSTEMD, GObject)

/**
 * XfceSystemdFlags:
 *
 * The `SD_LOGIND_*` flags to be passed to the D-Bus request, see <ulink url="https://www.freedesktop.org/software/systemd/man/latest/org.freedesktop.login1.html##Methods">systemd-logind documentation</ulink>.
 **/
typedef enum /*< prefix=XFCE_SYSTEMD_ >*/
{
  XFCE_SYSTEMD_NONE = 0,
  XFCE_SYSTEMD_ROOT_CHECK_INHIBITORS = 1 << 0,
  XFCE_SYSTEMD_KEXEC_REBOOT = 1 << 1,
  XFCE_SYSTEMD_SOFT_REBOOT = 1 << 2,
  XFCE_SYSTEMD_SOFT_REBOOT_IF_NEXTROOT_SET_UP = 1 << 3,
  XFCE_SYSTEMD_SKIP_INHIBITORS = 1 << 4,
} XfceSystemdFlags;

XfceSystemd *
xfce_systemd_get (void);

gboolean
xfce_systemd_reboot (XfceSystemd *systemd,
                     gboolean polkit_interactive,
                     GError **error);

gboolean
xfce_systemd_power_off (XfceSystemd *systemd,
                        gboolean polkit_interactive,
                        GError **error);

gboolean
xfce_systemd_suspend (XfceSystemd *systemd,
                      gboolean polkit_interactive,
                      GError **error);

gboolean
xfce_systemd_hibernate (XfceSystemd *systemd,
                        gboolean polkit_interactive,
                        GError **error);

gboolean
xfce_systemd_hybrid_sleep (XfceSystemd *systemd,
                           gboolean polkit_interactive,
                           GError **error);

gboolean
xfce_systemd_reboot_with_flags (XfceSystemd *systemd,
                                XfceSystemdFlags flags,
                                GError **error);

gboolean
xfce_systemd_power_off_with_flags (XfceSystemd *systemd,
                                   XfceSystemdFlags flags,
                                   GError **error);

gboolean
xfce_systemd_suspend_with_flags (XfceSystemd *systemd,
                                 XfceSystemdFlags flags,
                                 GError **error);

gboolean
xfce_systemd_hibernate_with_flags (XfceSystemd *systemd,
                                   XfceSystemdFlags flags,
                                   GError **error);

gboolean
xfce_systemd_hybrid_sleep_with_flags (XfceSystemd *systemd,
                                      XfceSystemdFlags flags,
                                      GError **error);

gboolean
xfce_systemd_can_reboot (XfceSystemd *systemd,
                         gboolean *can_reboot,
                         gboolean *auth_reboot,
                         GError **error);

gboolean
xfce_systemd_can_power_off (XfceSystemd *systemd,
                            gboolean *can_power_off,
                            gboolean *auth_power_off,
                            GError **error);

gboolean
xfce_systemd_can_suspend (XfceSystemd *systemd,
                          gboolean *can_suspend,
                          gboolean *auth_suspend,
                          GError **error);

gboolean
xfce_systemd_can_hibernate (XfceSystemd *systemd,
                            gboolean *can_hibernate,
                            gboolean *auth_hibernate,
                            GError **error);

gboolean
xfce_systemd_can_hybrid_sleep (XfceSystemd *systemd,
                               gboolean *can_hybrid_sleep,
                               gboolean *auth_hybrid_sleep,
                               GError **error);

G_END_DECLS

#endif /* __XFCE_SYSTEMD_H__ */
