#include "execute.h"
#include "../utils/get_next_line.h"

bool	new_t_redirect_out(t_redirect_out **r_out, char *filename, t_node_type type)
{
	while (*r_out)
		r_out = &(*r_out)->next;
	*r_out = malloc(sizeof(**r_out));
	if (!*r_out)
		return (false);
	if (type == REDIRECT_OUT_NODE)
		(*r_out)->fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 00644);
	else
		(*r_out)->fd = open(filename, O_WRONLY | O_APPEND | O_CREAT, 00644);
	if ((*r_out)->fd == -1)
		perror("open"); //todo: check when to print error
	(*r_out)->next = NULL;
	return (true);
}

static void process_gnl_error(t_executor *e, t_gnl_status status, char *line)
{
	if (status == GNL_STATUS_ERROR_MALLOC)
		exit(ex_perror(e, "minishell: malloc"));
	if (status == GNL_STATUS_ERROR_READ)
	{
		free(line);
		exit(ex_perror(e, "minishell: read"));
	}
}

bool	new_t_redirect_in(t_executor *e, t_redirect_in **r_in, char *data, t_node_type type)
{
	int pipefd[2];
	int status;
	char *line;


	while (*r_in)
		r_in = &(*r_in)->next;
	*r_in = malloc(sizeof(**r_in));
	if (!*r_in)
		return (false);
	if (type == REDIRECT_IN_NODE)
	{
		(*r_in)->fd = open(data, O_RDONLY);
		if ((*r_in)->fd == -1)
			ex_perror(e, "minishell: open");
		(*r_in)->next = NULL;
	}
	else if (type == HEREDOC_NODE)
	{
		pipe(pipefd);
		while (1)
		{
			ft_putstr_fd("> ", STDOUT_FILENO);
			status = get_next_line(STDIN_FILENO, &line);
			process_gnl_error(e, status, line);
			if (status == GNL_STATUS_DONE || !ft_strcmp(line, data))
				break ;
			ft_putendl_fd(line, pipefd[WRITE]);
			free(line);
		}
		free(line);
		close(pipefd[WRITE]);
		(*r_in)->fd = pipefd[READ];
		(*r_in)->next = NULL;
	}
	return (true);
}
