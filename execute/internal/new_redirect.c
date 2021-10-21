#include <fcntl.h>

#include "../execute.h"
#include "../../utils/get_next_line.h"
#include "execute_internal.h"

void	new_redirect_out(t_simple_command *sc, char *filename, t_node_type type)
{
	if (sc->r_out != UNSET_FD)
		close(sc->r_out);
	if (type == REDIRECT_OUT_NODE)
		sc->r_out = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 00644);
	else
		sc->r_out = open(filename, O_WRONLY | O_APPEND | O_CREAT, 00644);
	if (sc->r_out == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		sc->err = REDIRECT_ERR;
	}
}

void	new_redirect_in(t_simple_command *sc, char *data, t_node_type type)
{
	int				pipefd[2];
	int				status;
	char			*line;

	if (sc->r_in != UNSET_FD)
		close(sc->r_in);
	if (type == REDIRECT_IN_NODE)
	{
		sc->r_in = open(data, O_RDONLY);
		if (sc->r_in == -1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			perror(data);
			sc->err = REDIRECT_ERR;
		}
	}
	else if (type == HEREDOC_NODE)
	{
		x_pipe(pipefd);
		while (1)
		{
			ft_putstr_fd("> ", STDOUT_FILENO);
			status = x_get_next_line(STDIN_FILENO, &line);
			if (status == GNL_STATUS_DONE || !ft_strcmp(line, data))
				break ;
			ft_putendl_fd(line, pipefd[WRITE]);
			free(line);
		}
		free(line);
		close(pipefd[WRITE]);
		sc->r_in = pipefd[READ];
	}
}
