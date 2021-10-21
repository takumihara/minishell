#include <sys/stat.h>
#include "../utils/utils.h"

// stat() fails when there is no file in the indicated path.
// Make sure to check its existence with funcs like access()
int	x_stat(const char *restrict path, struct stat *restrict buf)
{
	int	res;

	res = stat(path, buf);
	if (res == -1)
		perror_exit("stat", EXIT_FAILURE);
	return (res);
}