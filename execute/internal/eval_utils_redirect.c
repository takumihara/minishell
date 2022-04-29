#include <stdio.h>
#include <readline/readline.h>
#include <sys/fcntl.h>

#include "../execute.h"
#include "execute_internal.h"

static int	execute_heredoc(char *delim);

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
		sc->r_in = execute_heredoc(data);
		if (sc->r_in == -1)
			sc->err = REDIRECT_ERR;
	}
}

void	execute_heredoc_child(char *delim, int pipefd[2])
{
	char	*line;

	close(pipefd[READ]);
	while (1)
	{
		line = readline("> ");
		if (!line || !ft_strcmp(line, delim))
			break ;
		ft_putendl_fd(line, pipefd[WRITE]);
		free(line);
	}
	free(line);
	close(pipefd[WRITE]);
	exit(EXIT_SUCCESS);
}

int	execute_heredoc(char *delim)
{
	pid_t	pid;
	int		pipefd[2];
	int		statloc;

	x_pipe(pipefd);
	pid = x_fork();
	if (pid == CHILD_PROCESS)
	{
		x_signal(SIGINT, SIG_DFL);
		execute_heredoc_child(delim, pipefd);
	}
	close(pipefd[WRITE]);
	if (wait(&statloc) == pid && WIFSIGNALED(statloc))
		pipefd[READ] = -1;
	return (pipefd[READ]);
}
