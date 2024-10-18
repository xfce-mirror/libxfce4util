#!/bin/sh

set -e

if test x"$XDT_CHECK_ABI" = x""; then
    echo "Environment variable XDT_CHECK_ABI must be set" >&2
    exit 1
fi

exec "$XDT_CHECK_ABI" "${srcdir:-.}"/libxfce4util.symbols .libs/libxfce4util.so
