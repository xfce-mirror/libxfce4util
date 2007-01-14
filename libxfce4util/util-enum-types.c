
/* Generated data (by glib-mkenums) */

#include <libxfce4util/libxfce4util.h>
#include <libxfce4util/util-enum-types.h>

/* enumerations from "xfce-resource.h" */
GType
xfce_resource_type_get_type (void)
{
  static GType etype = 0;
  if (etype == 0) {
    static const GEnumValue values[] = {
      { XFCE_RESOURCE_DATA, "XFCE_RESOURCE_DATA", "data" },
      { XFCE_RESOURCE_CONFIG, "XFCE_RESOURCE_CONFIG", "config" },
      { XFCE_RESOURCE_CACHE, "XFCE_RESOURCE_CACHE", "cache" },
      { XFCE_RESOURCE_ICONS, "XFCE_RESOURCE_ICONS", "icons" },
      { XFCE_RESOURCE_THEMES, "XFCE_RESOURCE_THEMES", "themes" },
      { 0, NULL, NULL }
    };
    etype = g_enum_register_static ("XfceResourceType", values);
  }
  return etype;
}

/* Generated data ends here */

