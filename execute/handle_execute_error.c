#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>

#include "exit_status.h"
#include "../libft/libft.h"

void	handle_exec_error(char *path, bool is_exec_run)
{
	(void)is_exec_run;
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (!is_exec_run)
	{
		ft_putstr_fd(path, STDERR_FILENO);
		ft_putendl_fd(": command not found", STDERR_FILENO);
		exit(ES_COMMAND_NOT_FOUND);
	}
	else
	{
		perror(path);
		if (access(path, X_OK) == -1)
			exit(ES_PERMISSION_DENIED);
		exit(ES_NO_SUCH_FILE);
	}
}
