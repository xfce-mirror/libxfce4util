#!/bin/sh

cat <<EOF
/* Auto generated file, do not edit */

/* Keep the license stuff in one section if possible */
#if defined(__GNUC__)
#define SECTION __attribute__((section(".xfce.LICENSES")))
#else
#define SECTION
#endif

EOF

for item in $@; do
	name=`echo $item | awk 'BEGIN {FS=":"} {print $1}'`
	file=`echo $item | awk 'BEGIN {FS=":"} {print $2}'`

	echo "const char xfce_builtin_license_$name [] SECTION ="
	sed -e 's/"/\\"/g' -e 's/^\(.*\)$/  "\1\\n"/' < $file
	echo ";"
	echo
done

exit 0
