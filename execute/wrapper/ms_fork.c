#include <unistd.h>

#include "../../utils/utils.h"

pid_t	ms_fork(void)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_exit("fork", EXIT_FAILURE);
	return (pid);
}