#include <sys/stat.h>
#include <stdbool.h>

bool	is_dir(const char *path)
{
	int			stat_res;
	struct stat	stat_;

	stat_res = stat(path, &stat_);
	if (stat_res == -1)
		return (false);
	if ((stat_.st_mode & S_IFMT) == S_IFDIR)
		return (true);
	else
		return (false);
}