/*
 * Copyright (c) 2003 Benedikt Meurer (benedikt.meurer@unix-ag.uni-siegen.de)
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
#endif /* !HAVE_CONFIG_H */

#include <sys/wait.h>

#include <errno.h>
#include <unistd.h>

#include "filter.h"


XfceFilter *
xfce_filter_new(gchar *cmd, gchar **argv)
{
	XfceFilter *filter;

	g_return_val_if_fail(argv != NULL, NULL);
	g_return_val_if_fail(cmd != NULL, NULL);

	filter = g_new0(XfceFilter, 1);
	filter->command = cmd;
	filter->argv = argv;

	return(filter);
}

void
xfce_filter_free(XfceFilter *filter)
{
	gchar **p;

	g_return_if_fail(filter != NULL);

	if (filter->argv) {
		for (p = filter->argv; *p; p++)
			g_free(*p);
		g_free(filter->argv);
	}

	g_free(filter->command);
	g_free(filter);
}


XfceFilterList *
xfce_filterlist_new(void)
{
	return(g_new0(XfceFilterList, 1));
}

void
xfce_filterlist_free(XfceFilterList *filters)
{
	g_return_if_fail(filters != NULL);

	g_list_foreach(filters->filters, (GFunc)xfce_filter_free, NULL);
	g_list_free(filters->filters);
	g_free(filters);
}

void
xfce_filterlist_append(XfceFilterList *filters, XfceFilter *filter)
{
	g_return_if_fail(filter != NULL);
	g_return_if_fail(filters != NULL);

	filters->filters = g_list_append(filters->filters, filter);
}

void
xfce_filterlist_prepend(XfceFilterList *filters, XfceFilter *filter)
{
	g_return_if_fail(filter != NULL);
	g_return_if_fail(filters != NULL);

	filters->filters = g_list_prepend(filters->filters, filter);
}

XfceFilter *
xfce_filterlist_first(XfceFilterList *filters)
{
	g_return_val_if_fail(filters != NULL, NULL);

	filters->cursor = g_list_first(filters->filters);

	return(filters->cursor ? (XfceFilter *)filters->cursor->data : NULL);
}

XfceFilter *
xfce_filterlist_next(XfceFilterList *filters)
{
	g_return_val_if_fail(filters != NULL, NULL);

	if (filters->cursor)
		filters->cursor = g_list_next(filters->cursor);

	return(filters->cursor ? (XfceFilter *)filters->cursor->data : NULL);
}

int
xfce_filterlist_execute(XfceFilterList *filters, int input, int output,
	int error)
{
	XfceFilter *filter;
	int status;
	int fd[2];
	int outfd;
	pid_t pid;

	filter = xfce_filterlist_first(filters);

	for (; filter; filter = xfce_filterlist_next(filters)) {
		if (pipe(fd) < 0)
			return(-1);

		if ((pid = vfork()) < 0) {
			return(-1);
		}
		else if (pid == 0) {
			(void)close(fd[0]);

			/* connect the last processes stdout to output */
			if (!g_list_next(filters->cursor))
				outfd = output;
			else
				outfd = fd[1];

			if (input != STDIN_FILENO)
				(void)dup2(input, STDIN_FILENO);

			if (outfd != STDOUT_FILENO)
				(void)dup2(outfd, STDOUT_FILENO);

			if (error != STDERR_FILENO)
				(void)dup2(error, STDERR_FILENO);

			(void)execvp(filter->command, filter->argv);
			_exit(127);
		}

		(void)close(input);
		(void)close(fd[1]);

		input = fd[0];
	}

	(void)close(input);

	while ((pid = waitpid(-1, &status, 0)) >= 0) {
		if (!WIFEXITED(status))
			return(status);
		else if (WEXITSTATUS(status))
			return(WEXITSTATUS(status));
	}

	if (pid < 0 && errno != ECHILD)
		return(-1);

	return(0);
}


