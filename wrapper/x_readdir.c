#include "x.h"
#include <errno.h>

struct dirent	*readdir(DIR *dirp)
{
	struct dirent	*dp;

	errno = 0;
	dp = readdir(dirp);
	if (!dp && errno != 0)
		perror_exit("readdir", EXIT_FAILURE);
	return dp;
}
