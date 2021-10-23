#include "expander_internal.h"

void	new_expander(t_expander **e, t_env_var *env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->err = NO_ERR;
	(*e)->err_data = NULL;
	(*e)->env_vars = env_vars;
}

void	expand_redirect_error(char *original_data, t_expander *e)
{
	if (e->err != NO_ERR)
		free(e->err_data);
	e->err = AMBIGUOUS_REDIRECT_ERR;
	e->err_data = x_strdup(original_data);
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

bool	is_expandable_data(t_expander *e, t_ast_node *node, char *original_data)
{
	if (!*node->data && node->type == COMMAND_ARG_NODE)
	{
		free(node->data);
		node->data = NULL;
		return (false);
	}
	else if (!*node->data && *original_data && node->type != COMMAND_ARG_NODE)
	{
		expand_redirect_error(original_data, e);
		return (false);
	}
	return (true);
}
