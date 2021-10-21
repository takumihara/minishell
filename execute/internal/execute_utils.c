#include "../execute.h"
#include "../../wrapper/x.h"
#include "execute_internal.h"
#include "../../builtin/builtin.h"

void	new_executor(t_executor **e, t_ast_node *root, t_env_var **env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->root = root;
	(*e)->exit_status = EXIT_SUCCESS;
	(*e)->condition = CONDITION_AND_IF;
	(*e)->pipeline = NULL;
	(*e)->env_vars = env_vars;
}

//void	delete_executor(t_executor **e)
//{
//	delete_list((*e)->pipeline, T_PIPELINE);
//	delete_ast_nodes((*e)->root, NULL);
//	free(*e);
//	*e = NULL;
//}

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
		delete_list(((t_pipeline *)element)->command, ((t_pipeline *)element)->type);
		delete_list(((t_pipeline *)element)->next, T_PIPELINE);
	}
	free(element);
}

//int	ex_perror(t_executor *e, const char *s)
//{
//	perror(s);
//	if (e)
//		delete_executor(&e);
//	return (EXIT_FAILURE);
//}

static void	execute_builtin_internal(int argc, char **argv, t_executor *e, bool islast, int (*fn)(int, char**, int, t_env_var**))
{
	if (islast)
		e->exit_status = fn(argc, argv, e->exit_status, e->env_vars);
	else
		fn(argc, argv, e->exit_status, e->env_vars);
}

bool	execute_builtin(t_executor *e, int argc, char **argv, bool is_last)
{
	if (!ft_strcmp(argv[0], "cd"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_cd);
	else if (!ft_strcmp(argv[0], "pwd"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_pwd);
	else if (!ft_strcmp(argv[0], "exit"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_exit);
	else if (!ft_strcmp(argv[0], "echo"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_echo);
	else if (!ft_strcmp(argv[0], "export"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_export);
	else if (!ft_strcmp(argv[0], "env"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_env);
	else if (!ft_strcmp(argv[0], "unset"))
		execute_builtin_internal(argc, argv, e, is_last, builtin_unset);
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
