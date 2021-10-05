#include <string.h>
#include "builtin.h"
#include "../libft/libft.h"

void	cd(int argc, char **argv)
{
	char	*path;
	if (argc == 1)
		path = getenv("HOME");
	else if (ft_strlen(argv[1]) == 0)
		return ;
	else
		path = argv[1];
	if (chdir(path) == -1)
		perror("chdir");
}