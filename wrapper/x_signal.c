#include "x.h"
#include <signal.h>

void	x_signal(int sig, void (*f)(int))
{
	if (signal(sig, f) == SIG_ERR)
		perror_exit("signal", EXIT_FAILURE);
}
