/* $Id$ */
/*-
 * Copyright (c) 2004 Benedikt Meurer <benny@xfce.org>
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#include <libxfce4util/libxfce4util.h>


static void
usage (gboolean error)
{
  FILE *fp = error ? stderr : stdout;

  fprintf (fp,
           "Usage: xfce4-kiosk-query [OPTION...] <module> <capability>...\n"
           "\n"
           "Options:\n"
           "  -h          Print this help screen\n"
           "  -v          Show version information\n"
           "\n"
           "Queries the given capabilities of <module> for the current user\n"
           "and reports whether the user has the capabilities or not. This\n"
           "tool is mainly ment for system administrators to test their\n"
           "Kiosk setup.\n");

  exit (error ? EXIT_FAILURE : EXIT_SUCCESS);
}


int
main (int argc, char **argv)
{
  const gchar *module;
  XfceKiosk   *kiosk;
  int          ch;
  int          n;

  while ((ch = getopt (argc, argv, "hv")) != -1)
    {
      switch (ch)
        {
        case 'v':
          printf ("xfce4-kiosk-query %s (Xfce %s)\n\n"
                  "Copyright (c) 2003-2004\n"
                  "        The Xfce development team. All rights reserved.\n"
                  "Written for Xfce by Benedikt Meurer <benny@xfce.org>.\n\n"
                  "Please report bugs to <%s>.\n",
                  PACKAGE_VERSION, xfce_version_string (), PACKAGE_BUGREPORT);
          return EXIT_SUCCESS;

        case 'h':
        default:
          usage (ch != 'h');
        }
    }

  argc -= optind;
  argv += optind;

  if (argc < 2)
    usage (TRUE);

  module = argv[0];

  printf ("MODULE \"%s\"\n", module);

  kiosk = xfce_kiosk_new (module);

  for (n = 1; n < argc; ++n)
    {
      if (xfce_kiosk_query (kiosk, argv[n]))
        printf ("  CAP \"%s\" = ALLOWED\n", argv[n]);
      else
        printf ("  CAP \"%s\" = DENIED\n", argv[n]);
    }

  return EXIT_SUCCESS;
}


