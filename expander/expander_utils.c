#include "expander.h"

void	new_expander(t_expander **e, t_env_var *env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->err = NO_ERR;
	(*e)->err_data = NULL;
	(*e)->env_vars = env_vars;
}

// int	expand_perror(t_expander *e, const char *s)
// {
// 	perror(s);
// 	if (e)
// 	{
// 		delete_ast_nodes(e->node, NULL);
// 		delete_env_lst(e->env_vars, NULL, NULL);
// 	}
// 	free(e);
// 	return (EXIT_FAILURE);
// }

t_ast_node	*expand_redirect_error(char *original_data, t_ast_node *node, t_expander *e)
{
	if (e->err != NO_ERR)
		free(e->err_data);
	e->err = AMBIGUOUS_REDIRECT_ERR;
	e->err_data = x_strdup(original_data);
	return (node);
}

int	quotation_status(char c, int status)
{
	if (c == '\"')
	{
		if (status == IN_DOUBLE_QUOTE)
			status = OUTSIDE;
		else if (status == IN_SINGLE_QUOTE)
			status = IN_SINGLE_QUOTE;
		else
			status = IN_DOUBLE_QUOTE;
	}
	else if (c == '\'')
	{
		if (status == IN_DOUBLE_QUOTE)
			status = IN_DOUBLE_QUOTE;
		else if (status == IN_SINGLE_QUOTE)
			status = OUTSIDE;
		else
			status = IN_SINGLE_QUOTE;
	}
	return (status);
}

t_ast_node	*handle_expand_error(t_expander *e)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	ft_putstr_fd(e->err_data, STDERR_FILENO);
	ft_putendl_fd(": ambiguous redirect", STDERR_FILENO);
	free(e->err_data);
	free(e);
	return (NULL);
}
