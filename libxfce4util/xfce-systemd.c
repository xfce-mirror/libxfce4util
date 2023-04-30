/*-
 * Copyright (C) 2012 Christian Hesse
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301 USA.
 */

/**
 * SECTION:xfce-systemd
 * @title: XfceSystemd
 * @short_description: Session management via systemd
 * @include: libxfce4util/libxfce4util.h
 *
 * Since: 4.19.1
 **/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <gio/gio.h>

#include "xfce-systemd.h"



#define SYSTEMD_DBUS_NAME               "org.freedesktop.login1"
#define SYSTEMD_DBUS_PATH               "/org/freedesktop/login1"
#define SYSTEMD_DBUS_INTERFACE          "org.freedesktop.login1.Manager"
#define SYSTEMD_REBOOT_ACTION           "Reboot"
#define SYSTEMD_POWEROFF_ACTION         "PowerOff"
#define SYSTEMD_SUSPEND_ACTION          "Suspend"
#define SYSTEMD_HIBERNATE_ACTION        "Hibernate"
#define SYSTEMD_HYBRID_SLEEP_ACTION     "HybridSleep"
#define SYSTEMD_REBOOT_TEST             "CanReboot"
#define SYSTEMD_POWEROFF_TEST           "CanPowerOff"
#define SYSTEMD_SUSPEND_TEST            "CanSuspend"
#define SYSTEMD_HIBERNATE_TEST          "CanHibernate"
#define SYSTEMD_HYBRID_SLEEP_TEST       "CanHybridSleep"



static void     xfce_systemd_finalize     (GObject         *object);



struct _XfceSystemd
{
  GObject __parent__;

  GDBusProxy *proxy;
};



G_DEFINE_TYPE (XfceSystemd, xfce_systemd, G_TYPE_OBJECT)



static void
xfce_systemd_class_init (XfceSystemdClass *klass)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (klass);
  gobject_class->finalize = xfce_systemd_finalize;
}



static void
name_appeared (GDBusConnection *connection,
               const gchar *name,
               const gchar *name_owner,
               gpointer user_data)
{
  XfceSystemd *systemd = user_data;
  GError *error = NULL;

  g_debug ("%s started up, owned by %s", name, name_owner);

  systemd->proxy = g_dbus_proxy_new_for_bus_sync (G_BUS_TYPE_SYSTEM,
                                                  G_DBUS_PROXY_FLAGS_DO_NOT_AUTO_START,
                                                  NULL,
                                                  SYSTEMD_DBUS_NAME,
                                                  SYSTEMD_DBUS_PATH,
                                                  SYSTEMD_DBUS_INTERFACE,
                                                  NULL,
                                                  &error);
  if (error != NULL)
    {
      g_warning ("Failed to get a systemd proxy: %s", error->message);
      g_error_free (error);
    }
}



static void
name_vanished (GDBusConnection *connection,
               const gchar *name,
               gpointer user_data)
{
  XfceSystemd *systemd = user_data;

  g_debug ("%s vanished", name);

  g_clear_object (&systemd->proxy);
}



static void
xfce_systemd_init (XfceSystemd *systemd)
{
  g_bus_watch_name (G_BUS_TYPE_SYSTEM,
                    SYSTEMD_DBUS_NAME,
                    G_BUS_NAME_WATCHER_FLAGS_AUTO_START,
                    name_appeared,
                    name_vanished,
                    systemd,
                    NULL);
}



static void
xfce_systemd_finalize (GObject *object)
{
  XfceSystemd *systemd = XFCE_SYSTEMD (object);

  g_clear_object (&systemd->proxy);

  G_OBJECT_CLASS (xfce_systemd_parent_class)->finalize (object);
}



static gboolean
xfce_systemd_can_method (XfceSystemd *systemd,
                         gboolean *can_method_out,
                         const gchar *method,
                         GError **error)
{
  GVariant *variant;
  const gchar *can_string;
  gboolean can_method;

  /* never return true if something fails */
  if (can_method_out != NULL)
    *can_method_out = FALSE;

  if (systemd->proxy == NULL)
    {
      g_debug ("No systemd proxy");
      return FALSE;
    }

  g_debug ("Calling %s", method);

  variant = g_dbus_proxy_call_sync (systemd->proxy,
                                    method,
                                    NULL,
                                    G_DBUS_CALL_FLAGS_NONE,
                                    -1,
                                    NULL,
                                    error);
  if (variant == NULL)
    return FALSE;

  g_variant_get_child (variant, 0, "&s", &can_string);
  can_method = g_strcmp0 (can_string, "yes") == 0;
  g_variant_unref (variant);

  if (can_method_out != NULL)
    *can_method_out = can_method;

  return TRUE;
}



static gboolean
xfce_systemd_try_method (XfceSystemd *systemd,
                         const gchar *method,
                         GError **error)
{
  GVariant *variant;

  if (systemd->proxy == NULL)
    {
      g_debug ("No systemd proxy");
      return FALSE;
    }

  g_debug ("Calling %s", method);

  variant = g_dbus_proxy_call_sync (systemd->proxy,
                                    method,
                                    g_variant_new ("(b)", TRUE),
                                    G_DBUS_CALL_FLAGS_NONE,
                                    -1,
                                    NULL,
                                    error);
  if (variant == NULL)
    return FALSE;

  g_variant_unref (variant);

  return TRUE;
}



/**
 * xfce_systemd_get:
 *
 * Create a new #XfceSystemd instance or increase reference count.
 *
 * Returns: (transfer full): A reference to the singleton object, to be released with g_object_unref().
 *
 * Since: 4.19.1
 **/
XfceSystemd *
xfce_systemd_get (void)
{
  static XfceSystemd *object = NULL;

  if (G_LIKELY (object != NULL))
    {
      g_object_ref (G_OBJECT (object));
    }
  else
    {
      object = g_object_new (XFCE_TYPE_SYSTEMD, NULL);
      g_object_add_weak_pointer (G_OBJECT (object), (gpointer) &object);
    }

  return object;
}



/**
 * xfce_systemd_try_restart:
 * @systemd: the #XfceSystemd object
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Ask systemd to trigger restart.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_try_restart (XfceSystemd *systemd,
                          GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_try_method (systemd, SYSTEMD_REBOOT_ACTION, error);
}



/**
 * xfce_systemd_try_shutdown:
 * @systemd: the #XfceSystemd object
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Ask systemd to trigger shutdown.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_try_shutdown (XfceSystemd *systemd,
                           GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_try_method (systemd, SYSTEMD_POWEROFF_ACTION, error);
}



/**
 * xfce_systemd_try_suspend:
 * @systemd: the #XfceSystemd object
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Ask systemd to trigger suspend.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_try_suspend (XfceSystemd *systemd,
                          GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_try_method (systemd, SYSTEMD_SUSPEND_ACTION, error);
}



/**
 * xfce_systemd_try_hibernate:
 * @systemd: the #XfceSystemd object
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Ask systemd to trigger hibernate.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_try_hibernate (XfceSystemd *systemd,
                            GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_try_method (systemd, SYSTEMD_HIBERNATE_ACTION, error);
}



/**
 * xfce_systemd_try_hybrid_sleep:
 * @systemd: the #XfceSystemd object
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Ask systemd to trigger hybrid sleep.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_try_hybrid_sleep (XfceSystemd *systemd,
                               GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_try_method (systemd, SYSTEMD_HYBRID_SLEEP_ACTION, error);
}



/**
 * xfce_systemd_can_restart:
 * @systemd: the #XfceSystemd object
 * @can_restart: (out) (nullable): location to store capacity or %NULL
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Check whether systemd can trigger restart.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_can_restart (XfceSystemd *systemd,
                          gboolean *can_restart,
                          GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_can_method (systemd, can_restart, SYSTEMD_REBOOT_TEST, error);
}



/**
 * xfce_systemd_can_shutdown:
 * @systemd: the #XfceSystemd object
 * @can_shutdown: (out) (nullable): location to store capacity or %NULL
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Check whether systemd can trigger shutdown.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_can_shutdown (XfceSystemd *systemd,
                           gboolean *can_shutdown,
                           GError **error)
{
  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  return xfce_systemd_can_method (systemd, can_shutdown, SYSTEMD_POWEROFF_TEST, error);
}



/**
 * xfce_systemd_can_suspend:
 * @systemd: the #XfceSystemd object
 * @can_suspend: (out) (nullable): location to store capacity or %NULL
 * @auth_suspend: (out) (nullable): location to store authorization or %NULL
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Check whether systemd can trigger and has authorization for suspend.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_can_suspend (XfceSystemd *systemd,
                          gboolean *can_suspend,
                          gboolean *auth_suspend,
                          GError **error)
{
  gboolean ret, can_method;

  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  ret = xfce_systemd_can_method (systemd, &can_method, SYSTEMD_SUSPEND_TEST, error);

  if (can_suspend != NULL)
    *can_suspend = can_method;
  if (auth_suspend != NULL)
    *auth_suspend = can_method;

  return ret;
}



/**
 * xfce_systemd_can_hibernate:
 * @systemd: the #XfceSystemd object
 * @can_hibernate: (out) (nullable): location to store capacity or %NULL
 * @auth_hibernate: (out) (nullable): location to store authorization or %NULL
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Check whether systemd can trigger and has authorization for hibernate.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_can_hibernate (XfceSystemd *systemd,
                            gboolean *can_hibernate,
                            gboolean *auth_hibernate,
                            GError **error)
{
  gboolean ret, can_method;

  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  ret = xfce_systemd_can_method (systemd, &can_method, SYSTEMD_HIBERNATE_TEST, error);

  if (can_hibernate != NULL)
    *can_hibernate = can_method;
  if (auth_hibernate != NULL)
    *auth_hibernate = can_method;

  return ret;
}



/**
 * xfce_systemd_can_hybrid_sleep:
 * @systemd: the #XfceSystemd object
 * @can_hybrid_sleep: (out) (nullable): location to store capacity or %NULL
 * @auth_hybrid_sleep: (out) (nullable): location to store authorization or %NULL
 * @error: (out) (nullable): location to store error on failure or %NULL
 *
 * Check whether systemd can trigger and has authorization for hybrid sleep.
 *
 * Returns: %TRUE if the D-Bus request was successful, %FALSE otherwise and @error is set.
 *
 * Since: 4.19.1
 **/
gboolean
xfce_systemd_can_hybrid_sleep (XfceSystemd *systemd,
                               gboolean *can_hybrid_sleep,
                               gboolean *auth_hybrid_sleep,
                               GError **error)
{
  gboolean ret, can_method;

  g_return_val_if_fail (XFCE_IS_SYSTEMD (systemd), FALSE);
  g_return_val_if_fail (error == NULL || *error == NULL, FALSE);

  ret = xfce_systemd_can_method (systemd, &can_method, SYSTEMD_HYBRID_SLEEP_TEST, error);

  if (can_hybrid_sleep != NULL)
    *can_hybrid_sleep = can_method;
  if (auth_hybrid_sleep != NULL)
    *auth_hybrid_sleep = can_method;

  return ret;
}
