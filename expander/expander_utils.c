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
