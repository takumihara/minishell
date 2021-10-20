#include <unistd.h>

#include "../utils/utils.h"

int	x_dup2(int fildes, int fildes2)
{
	int	newfd;

	newfd = dup2(fildes, fildes2);
	if (newfd == -1)
		perror_exit("dup", EXIT_FAILURE);
	return (newfd);
}
