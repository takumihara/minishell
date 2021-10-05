#include <string.h>
#include "builtin.h"
#include "../libft/libft.h"

void	cd(int argc, char **argv)
{
	char	*path;
	if (argc == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return ;
		}
	}
	else if (argv[1][0] == '\0')
		return ;
	else
		path = argv[1];
	if (chdir(path) == -1)
		perror("minishell: cd");
}