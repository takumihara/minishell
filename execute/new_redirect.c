#include "execute.h"
#include "../utils/get_next_line.h"
#include "../wrapper/x.h"

void	new_t_redirect_out(t_simple_command *sc, char *filename, t_node_type type)
{
	t_redirect_out	**r_out;

	r_out = &sc->r_out;
	while (*r_out)
		r_out = &(*r_out)->next;
	*r_out = x_malloc(sizeof(**r_out));
	if (type == REDIRECT_OUT_NODE)
		(*r_out)->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 00644);
	else
		(*r_out)->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 00644);
	if ((*r_out)->fd == -1)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		perror(filename);
		sc->err = REDIRECT_ERR;
	}
	(*r_out)->next = NULL;
}

void	new_t_redirect_in(t_simple_command *sc, char *data, t_node_type type)
{
	int pipefd[2];
	int status;
	char *line;
	t_redirect_in **r_in;

	r_in = &sc->r_in;
	while (*r_in)
		r_in = &(*r_in)->next;
	*r_in = x_malloc(sizeof(**r_in));
	if (type == REDIRECT_IN_NODE)
	{
		(*r_in)->fd = open(data, O_RDONLY);
		if ((*r_in)->fd == -1)
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
		(*r_in)->fd = pipefd[READ];
	}
	(*r_in)->next = NULL;
}
