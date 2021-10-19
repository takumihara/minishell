#include <stdio.h>
#include <stdlib.h>

void	perror_exit(const char *const msg, int status)
{
	perror(msg);
	exit(status);
}
