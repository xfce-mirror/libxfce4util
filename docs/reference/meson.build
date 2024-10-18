if get_option('gtk-doc')
  libxfce4util_doc_name = 'libxfce4util'

  conf_data = configuration_data()
  conf_data.set_quoted('PACKAGE_NAME', libxfce4util_doc_name)
  conf_data.set_quoted('PACKAGE_VERSION', meson.project_version())
  conf_data.set_quoted('PACKAGE_STRING', '@0@-@1@'.format(libxfce4util_doc_name, meson.project_version()))

  configure_file(
    input: 'gtkdocentities.ent.in',
    output: 'gtkdocentities.ent',
    configuration: conf_data,
  )

  libxfce4util_doc_private_headers = [
    'libxfce4util-private.h',
    'xfce-private.h',
    'xfce-rc-private.h',
  ]

  libxfce4util_doc_fixxref_args = [
    '--extra-dir=@0@/share/gtk-doc/html/glib'.format(glib.get_variable(pkgconfig: 'prefix')),
    '--extra-dir=@0@/share/gtk-doc/html/gobject'.format(gobject.get_variable(pkgconfig: 'prefix')),
    '--extra-dir=@0@/share/gtk-doc/html/gio'.format(gio.get_variable(pkgconfig: 'prefix')),
  ]

  gnome.gtkdoc(
    libxfce4util_doc_name,
    src_dir: meson.project_source_root() / 'libxfce4util',
    ignore_headers: libxfce4util_doc_private_headers,
    main_xml: 'libxfce4util-docs.xml',
    dependencies: libxfce4util_dep,
    gobject_typesfile: meson.current_source_dir() / 'libxfce4util.types',
    fixxref_args: libxfce4util_doc_fixxref_args,
    install: true,
    check: true,
  )
endif
