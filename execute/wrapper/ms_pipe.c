#include <unistd.h>

#include "../../utils/utils.h"

int ms_pipe(int fildes[2])
{
	int	res;

	res = pipe(fildes);
	if (res == -1)
		perror_exit("pipe", EXIT_FAILURE);
	return (res);
}