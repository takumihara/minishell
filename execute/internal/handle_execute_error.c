#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#include "exit_status.h"
#include "execute_internal.h"
#include "../../libft/libft.h"
#include "../execute.h"

void	handle_exec_error(char *path, bool is_exec_run)
{
	if (!is_exec_run)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(ES_COMMAND_NOT_FOUND);
	}
	else
	{
		if (access(path, F_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": No such file or directory", STDERR_FILENO);
			exit(ES_NO_SUCH_FILE);
		}
		else if (is_dir(path))
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": is a directory", STDERR_FILENO);
			exit(ES_IS_A_DIRECTORY);
		}
		else if (access(path, X_OK) == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd(path, STDERR_FILENO);
			ft_putendl_fd(": Permission denied", STDERR_FILENO);
			exit(ES_PERMISSION_DENIED);
		}
		exit(EXIT_SUCCESS);
	}
}