#include "x.h"

DIR	*x_opendir(const char *str)
{
	DIR	*dir;

	dir = opendir(str);
	if (!dir)
		perror_exit("opendir", EXIT_FAILURE);
	return (dir);
}
