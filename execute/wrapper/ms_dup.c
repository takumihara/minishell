#include <unistd.h>

#include "../../utils/utils.h"

int	ms_dup(int fildes)
{
	int	newfd;

	newfd = dup(fildes);
	if (newfd == -1)
		perror_exit("dup", EXIT_FAILURE);
	return (newfd);
}
