#include "expander.h"

bool	new_expander(t_expander **e, t_ast_node *root)
{
	*e = (t_expander *)malloc(sizeof(**e));
	if (!*e)
		return (false);
	(*e)->root = root;
	(*e)->node = root;
	return (true);
}

bool	is_expandable_string(char *str, char delimiter)
{
	if (ft_strchr(str, delimiter))
		return (true);
	return (false);
}

int	expand_perror(t_expander *e, const char *s)
{
	perror(s);
	if (e)
		delete_ast_nodes(e->node, NULL);
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
