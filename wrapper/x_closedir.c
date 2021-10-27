#include "x.h"

void	x_closedir(DIR *dir)
{
	int	res;

	res = closedir(dir);
	if (res == -1)
		perror_exit("closedir", EXIT_FAILURE);
}
