project(
  'libxfce4util',
  'c',
version : '4.20.0',
  license : 'GPL-2',
  meson_version : '>= 0.56.0',
  default_options : ['c_std=gnu11', 'buildtype=debugoptimized', 'warning_level=2', 'b_ndebug=if-release']
)
libxfce4util_soversion = '7.0.0'
libxfce4util_soversion_major = libxfce4util_soversion.split('.')[0]

project_namespace = 'xfce'
sysconfdir = get_option('prefix') / get_option('sysconfdir')
datadir = get_option('prefix') / get_option('datadir')
pkgdatadir = get_option('prefix') / get_option('datadir') / meson.project_name()
localedir = get_option('prefix') / get_option('localedir')

cc = meson.get_compiler('c')
pkgconfig = import('pkgconfig')
gnome = import('gnome')
i18n = import('i18n')

dependency_versions = {
  'gir': '>= 1.72.0',
  'glib': '>= 2.72.0',
}

glib = dependency('glib-2.0', version: dependency_versions['glib'])
gobject = dependency('gobject-2.0', version: dependency_versions['glib'])
gio = dependency('gio-2.0', version: dependency_versions['glib'])
gir = dependency('gobject-introspection-1.0', version: dependency_versions['gir'], required: get_option('introspection'))

xdt_gen_visibility = find_program('xdt-gen-visibility', required: true)

vapigen = find_program('vapigen', required: get_option('vala'))
if get_option('vala').enabled()
  assert(get_option('introspection'), 'Building Vala bindings requires building GObject introspection data')
endif
generate_vala = not get_option('vala').disabled() and vapigen.found()

feature_cflags = []

gnu_symbol_visibility = 'default'
if get_option('visibility')
  gnu_symbol_visibility = 'hidden'
  feature_cflags += '-DENABLE_SYMBOL_VISIBILITY=1'
endif

headers = [
  'err.h',
  'errno.h',
  'grp.h',
  'libintl.h',
  'limits.h',
  'locale.h',
  'memory.h',
  'pwd.h',
  'signal.h',
  'stdarg.h',
  'stdlib.h',
  'string.h',
  'sys/stat.h',
  'sys/types.h',
  'sys/utsname.h',
  'time.h',
  'unistd.h',
]
foreach header : headers
  if cc.check_header(header)
    feature_cflags += '-DHAVE_@0@=1'.format(header.underscorify().to_upper())
  endif
endforeach

functions = [
  'gethostname',
  'getpwnam',
  'setlocale',
]
foreach function : functions
  if cc.has_function(function)
    feature_cflags += '-DHAVE_@0@=1'.format(function.to_upper())
  endif
endforeach

libintl = cc.find_library('intl', required: false)
need_libintl = false
has_bind_textdomain_codeset = cc.has_function('bind_textdomain_codeset')
if not has_bind_textdomain_codeset
  if libintl.found()
    has_bind_textdomain_codeset = cc.has_function(
      'bind_textdomain_codeset',
      dependencies: [
        libintl,
      ],
    )
    if has_bind_textdomain_codeset
      need_libintl = true
    endif
  endif
endif
if has_bind_textdomain_codeset
  feature_cflags += '-DHAVE_BIND_TEXTDOMAIN_CODESET=1'
endif

extra_cflags = []
extra_cflags_check = [
  '-Wmissing-declarations',
  '-Wmissing-noreturn',
  '-Wold-style-definition',
  '-Wredundant-decls',
  '-Wpointer-arith',
  '-Wcast-align',
  '-Winit-self',
  '-Wshadow',
  '-Wmissing-include-dirs',
  '-Wundef',
  '-Wformat',
  '-Wformat-security',
  '-Wformat-y2k',
  '-Wnested-externs',
  '-Wno-unused-parameter',
  '-Wno-declaration-after-statement',
  '-Werror=implicit-function-declaration',
  '-Wno-error=deprecated-declarations',
]

buildtype = get_option('buildtype')
if buildtype == 'debug' or buildtype == 'debugoptimized'
  extra_cflags_check += '-fstack-protector-strong'
  extra_cflags += '-DDEBUG=1'

  if buildtype == 'debug'
    extra_cflags += [
      '-DG_ENABLE_DEBUG',
      '-DDEBUG_TRACE=1',
    ]
  endif
elif buildtype == 'minsize'
  extra_cflags += [
    '-DNDEBUG',
    '-DG_DISABLE_CAST_CHECKS',
    '-DG_DISABLE_ASSERT',
  ]
endif

if dependency_versions.has_key('glib')
  glib_version_parts = dependency_versions['glib'].split(' ')
  glib_min_version_parts = glib_version_parts[1].split('.')
  glib_min_version_define = 'GLIB_VERSION_@0@_@1@'.format(glib_min_version_parts[0], glib_min_version_parts[1])
  extra_cflags += [
    '-DGLIB_VERSION_MIN_REQUIRED=@0@'.format(glib_min_version_define),
    '-DGLIB_VERSION_MAX_ALLOWED=@0@'.format(glib_min_version_define),
    '-DG_LOG_DOMAIN="@0@"'.format(meson.project_name()),
    '-DG_LOG_USE_STRUCTURED=1',
  ]
endif

version_parts = meson.project_version().split('-dev')[0].split('.')
version_short = '@0@.@1@'.format(version_parts[0], version_parts[1])

extra_cflags += [
  '-DPACKAGE="@0@"'.format(meson.project_name()),
  '-DPACKAGE_NAME="@0@"'.format(meson.project_name()),
  '-DPACKAGE_VERSION="@0@"'.format(meson.project_version()),
  '-DVERSION="@0@"'.format(meson.project_version()),
  '-DVERSION_SHORT="@0@"'.format(version_short),
  '-DPACKAGE_STRING="@0@ @1@"'.format(meson.project_name(), meson.project_version()),
  '-DPACKAGE_DATADIR="@0@"'.format(pkgdatadir),
  '-DPACKAGE_LOCALE_DIR="@0@"'.format(get_option('prefix') / get_option('localedir')),
  '-DPACKAGE_BUGREPORT="https://gitlab.xfce.org/@0@/@1@/-/issues"'.format(project_namespace, meson.project_name()),
  '-DGETTEXT_PACKAGE="@0@"'.format(meson.project_name()),
  '-DPREFIX="@0@"'.format(get_option('prefix')),
  '-DBINDIR="@0@"'.format(get_option('prefix') / get_option('bindir')),
  '-DDATADIR="@0@"'.format(get_option('prefix') / get_option('datadir')),
  '-DINCLUDEDIR="@0@"'.format(get_option('prefix') / get_option('includedir')),
  '-DLIBDIR="@0@"'.format(get_option('prefix') / get_option('libdir')),
  '-DLIBEXECDIR="@0@"'.format(get_option('prefix') / get_option('libexecdir')),
  '-DLOCALEDIR="@0@"'.format(get_option('prefix') / get_option('localedir')),
  '-DLOCALSTATEDIR="@0@"'.format(get_option('prefix') / get_option('localstatedir')),
  '-DSBINDIR="@0@"'.format(get_option('prefix') / get_option('sbindir')),
  '-DSYSCONFDIR="@0@"'.format(get_option('prefix') / get_option('sysconfdir')),
  '-DHAVE_XFCE_REVISION_H=1',
]

add_project_arguments(cc.get_supported_arguments(extra_cflags_check), language: 'c')
add_project_arguments(extra_cflags, language: 'c')
add_project_arguments(feature_cflags, language: 'c')

xfce_revision_h = vcs_tag(
  command: ['git', 'rev-parse', '--short', 'HEAD'],
  fallback: 'UNKNOWN',
  input: 'xfce-revision.h.in',
  output: 'xfce-revision.h',
  replace_string: '@REVISION@',
)

subdir('libxfce4util')
subdir('docs' / 'reference')
subdir('po')
subdir('xfce4-kiosk-query')
