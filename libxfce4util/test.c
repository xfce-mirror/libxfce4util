/*
 */

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "filter.h"

gchar *args[] = {
	"cat",
	NULL
};

int
main(int argc, char **argv)
{
	XfceFilterList *list;
	XfceFilter *f;
	int in, out;
	int r;

	if (argc != 3) {
		printf("Usage: in out\n");
		return(1);
	}

	if ((in = open(argv[1], O_RDONLY)) < 0)
		err(1, "open()");
	
	if ((out = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY)) < 0)
		err(1, "open()");

	list = xfce_filterlist_new();
	f = xfce_filter_new("cat", args);
	xfce_filterlist_append(list, f);

	if ((r = xfce_filterlist_execute(list, in, out)) <0)
		perror("xfce_filterlist_execute()");
	else
		printf("exit code: %d\n", r);
}

