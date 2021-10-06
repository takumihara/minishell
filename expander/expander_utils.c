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
	{
		delete_ast_nodes(e->root, NULL);
	}
	free(e);
	return (EXIT_FAILURE);
}
