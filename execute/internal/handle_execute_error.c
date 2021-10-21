#include <stdbool.h>
#include <unistd.h>

#include "../../libft/libft.h"
#include "exit_status.h"
#include "execute_internal.h"

static void	print_err_msg(char *path, char *msg)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(path, STDERR_FILENO);
	ft_putstr_fd(": ", STDERR_FILENO);
	ft_putendl_fd(msg, STDERR_FILENO);
}

void	handle_exec_error(char *path, bool is_exec_run)
{
	if (!is_exec_run)
	{
		print_err_msg(path, "command not found");
		exit(ES_COMMAND_NOT_FOUND);
	}
	else
	{
		if (access(path, F_OK) == -1)
		{
			print_err_msg(path, "No such file or directory");
			exit(ES_NO_SUCH_FILE);
		}
		else if (is_dir(path))
		{
			print_err_msg(path, "is a directory");
			exit(ES_IS_A_DIRECTORY);
		}
		else if (access(path, X_OK) == -1)
		{
			print_err_msg(path, "Permission denied");
			exit(ES_PERMISSION_DENIED);
		}
		exit(EXIT_SUCCESS);
	}
}
