#include <sys/stat.h>
#include <stdbool.h>

#include "../../wrapper/x.h"

bool	is_dir(const char *path)
{
	struct stat	stat_;

	x_stat(path, &stat_);
	if ((stat_.st_mode & S_IFMT) == S_IFDIR)
		return (true);
	else
		return (false);
}
