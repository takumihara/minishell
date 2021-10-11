#include "execute.h"
#include "../utils/get_next_line.h"

bool	new_executor(t_executor **e, t_ast_node *root)
{
	*e = (t_executor *)malloc(sizeof(**e));
	if (!*e)
		return (false);
	(*e)->root = root;
	(*e)->exit_status = EXIT_SUCCESS;
	(*e)->condition = CONDITION_AND_IF;
	(*e)->pipeline = NULL;
	return (true);
}

void	delete_list(void *element, t_list_type type)
{
	if (!element)
		return ;
	if (type == T_REDIRECT_OUT)
	{
		close(((t_redirect_out *)element)->fd);
		delete_list(((t_redirect_out *)element)->next, T_REDIRECT_OUT);
	}
	else if (type == T_REDIRECT_IN)
	{
		close(((t_redirect_in *)element)->fd);
		delete_list(((t_redirect_in *)element)->next, T_REDIRECT_IN);
	}
	else if (type == T_HEREDOC)
		delete_list(((t_redirect_in *)element)->next, T_HEREDOC);
	else if (type == T_SIMPLE_COMMAND)
	{
		free(((t_simple_command *)element)->argv);
		delete_list(((t_simple_command *)element)->r_out, T_REDIRECT_OUT);
		delete_list(((t_simple_command *)element)->r_in, T_REDIRECT_IN);
	}
	else if (type == T_PIPELINE)
	{
		delete_list(((t_pipeline *)element)->command, ((t_pipeline *)element)->type);
		delete_list(((t_pipeline *)element)->next, T_PIPELINE);
	}
	free(element);
}

int	ex_perror(t_executor *e, const char *s)
{
	perror(s);
	if (e)
	{
		delete_ast_nodes(e->root, NULL);
		delete_list((void *)e->pipeline, T_PIPELINE);
	}
	return (EXIT_FAILURE);
}

bool	execute_builtin(t_executor *e, int argc, char **argv, bool islast)
{
	if (!ft_strcmp(argv[0], "cd"))
	{
		if (islast)
			e->exit_status = cd(argc, argv);
		else
			cd(argc, argv);
		return (true);
	}
	else if (!ft_strcmp(argv[0], "pwd"))
	{
		if (islast)
			e->exit_status = pwd();
		else
			pwd(argc, argv);
		return (true);
	}
	else if (!ft_strcmp(argv[0], "exit"))
	{
		if (islast)
			e->exit_status = builtin_exit(argc, argv, e->exit_status);
		else
			builtin_exit(argc, argv, e->exit_status);
		return (true);
	}
	return (false);
}

bool	is_execute_condition(int condition, int exit_status)
{
	if (condition == CONDITION_AND_IF && exit_status == EXIT_SUCCESS)
		return (true);
	if (condition == CONDITION_OR_IF && exit_status != EXIT_SUCCESS)
		return (true);
	if (condition == CONDITION_NL)
		return (true);
	return (false);
}

static void process_gnl_error(t_executor *e, t_gnl_status status, char *line)
{
	if (status == GNL_STATUS_ERROR_MALLOC)
		exit(ex_perror(e, "minishell: malloc"));
	if (status == GNL_STATUS_ERROR_READ)
	{
		free(line);
		exit(ex_perror(e, "minishell: malloc"));
	}
}

void	execute_redirect(t_executor *e, t_simple_command *sc, int orig_stdfd[])
{
	int				pipefd[2];
	t_gnl_status	status;
	char			*line;

	while (sc->r_in)
	{
		if (!sc->r_in->next)
		{
			if (sc->r_in->type == T_REDIRECT_IN)
				dup2(sc->r_in->fd, STDIN_FILENO);
			else if (sc->r_in->type == T_HEREDOC)
			{
				pipe(pipefd);
				while (1)
				{
					ft_putstr_fd("> ", orig_stdfd[WRITE]);
					status = get_next_line(orig_stdfd[READ], &line);
					process_gnl_error(e, status, line);
					if (status == GNL_STATUS_DONE || !ft_strcmp(line, sc->r_in->doc))
						break ;
					ft_putendl_fd(line, pipefd[WRITE]);
					free(line);
				}
				free(line);
				close(pipefd[WRITE]);
				dup2(pipefd[READ], STDIN_FILENO);
				close(pipefd[READ]);
			}
		}
		else if (sc->r_in->type == T_HEREDOC)
		{
			while (1)
			{
				ft_putstr_fd("> ", orig_stdfd[WRITE]);
				status = get_next_line(orig_stdfd[READ], &line);
				process_gnl_error(e, status, line);
				if (status == GNL_STATUS_DONE || !ft_strcmp(line, sc->r_in->doc))
					break ;
				free(line);
			}
			free(line);
		}
		sc->r_in = sc->r_in->next;
	}
	while (sc->r_out && sc->r_out->next)
		sc->r_out = sc->r_out->next;
	if (sc->r_out)
		dup2(sc->r_out->fd, STDOUT_FILENO);
}
