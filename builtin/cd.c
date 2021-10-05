#include <string.h>
#include "builtin.h"
#include "../libft/libft.h"

int	cd(int argc, char **argv)
{
	char	*path;
	if (argc == 1)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (EXIT_FAILURE);
		}
	}
	else if (argv[1][0] == '\0')
		return (EXIT_SUCCESS);
	else
		path = argv[1];
	if (chdir(path) == -1)
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}