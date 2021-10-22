#include "../../wrapper/x.h"
#include "../../builtin/builtin.h"
#include "execute_internal.h"

void	new_executor(t_executor **e, t_ast_node *root, t_env_var **env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->root = root;
	(*e)->exit_status = EXIT_SUCCESS;
	(*e)->condition = CONDITION_AND_IF;
	(*e)->pipeline = NULL;
	(*e)->env_vars = env_vars;
}

void	delete_list(void *element, t_list_type type)
{
	if (!element)
		return ;
	else if (type == T_SIMPLE_COMMAND)
	{
		free(((t_simple_command *)element)->argv);
		close(((t_simple_command *)element)->r_out);
		close(((t_simple_command *)element)->r_in);
	}
	else if (type == T_COMPOUND_LIST)
	{
		delete_list(((t_compound_list *)element)->pipeline, T_PIPELINE);
		delete_list(((t_compound_list *)element)->next, T_COMPOUND_LIST);
	}
	else if (type == T_SUBSHELL)
	{
		delete_list(((t_subshell *)element)->compound_list, T_COMPOUND_LIST);
	}
	else if (type == T_PIPELINE)
	{
		delete_list(((t_pipeline *)element)->command,
			((t_pipeline *)element)->type);
		delete_list(((t_pipeline *)element)->next, T_PIPELINE);
	}
	free(element);
}

bool	execute_builtin(t_executor *e, int argc, char **argv)
{
	if (!ft_strcmp(argv[0], "cd"))
		e->exit_status = builtin_cd(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "pwd"))
		e->exit_status = builtin_pwd(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "exit"))
		e->exit_status = builtin_exit(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "echo"))
		e->exit_status = builtin_echo(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "export"))
		e->exit_status = builtin_export(argc,
				argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "env"))
		e->exit_status = builtin_env(argc, argv, e->exit_status, e->env_vars);
	else if (!ft_strcmp(argv[0], "unset"))
		e->exit_status = builtin_unset(argc, argv, e->exit_status, e->env_vars);
	else
		return (false);
	return (true);
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

void	execute_redirect(t_simple_command *sc)
{
	if (sc->r_in != UNSET_FD)
		x_dup2(sc->r_in, STDIN_FILENO);
	if (sc->r_out != UNSET_FD)
		x_dup2(sc->r_out, STDOUT_FILENO);
}
