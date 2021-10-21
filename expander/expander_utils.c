#include "expander.h"

void	new_expander(t_expander **e, t_ast_node *root, t_env_var *env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->root = root;
	(*e)->node = root;
	(*e)->env_vars = env_vars;
}

int	expand_perror(t_expander *e, const char *s)
{
	perror(s);
	if (e)
	{
		delete_ast_nodes(e->node, NULL);
		delete_env_lst(e->env_vars, NULL, NULL);
	}
	free(e);
	return (EXIT_FAILURE);
}

void		*expand_redirect_error(char *original_data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(original_data, 2);
	ft_putendl_fd(": ambiguous redirect", 2);
	return (NULL);
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
