#include "expander_internal.h"

void	new_expander(t_expander **e, t_env_var *env_vars)
{
	*e = x_malloc(sizeof(**e));
	(*e)->err = NO_ERR;
	(*e)->err_data = NULL;
	(*e)->env_vars = env_vars;
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
