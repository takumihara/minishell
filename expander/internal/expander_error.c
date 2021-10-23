#include "expander_internal.h"

void	expand_redirect_error(char *original_data, t_expander *e)
{
	if (e->err != NO_ERR)
		free(e->err_data);
	e->err = AMBIGUOUS_REDIRECT_ERR;
	e->err_data = x_strdup(original_data);
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

static bool	is_empty_command_arg_node(char *data, t_node_type type)
{
	return (data[0] == '\0' && type == COMMAND_ARG_NODE);
}

static bool	is_empty_redirection_node(char *data, char *orig, t_node_type type)
{
	return (data[0] == '\0' && orig[0] != '\0' && type != COMMAND_ARG_NODE);
}

bool	is_expandable_data(t_expander *e, t_ast_node *node, char *original_data)
{
	if (is_empty_command_arg_node(node->data, node->type))
	{
		free(node->data);
		node->data = NULL;
		return (false);
	}
	else if (is_empty_redirection_node(node->data, original_data, node->type))
	{
		expand_redirect_error(original_data, e);
		return (false);
	}
	return (true);
}
