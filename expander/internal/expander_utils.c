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

char	*handle_multiple_stars(char *data, size_t pre_len)
{
	char			*original_data;
	char			*star_start;
	size_t			i;

	original_data = x_strdup(data);
	star_start = &data[pre_len];
	i = pre_len;
	while (data[i] == '*')
		i++;
	ft_memmove(&data[pre_len + 1], &data[i], ft_strlen(&data[i]) + 1);
	data = expand_wildcard(data, pre_len);
	if (ft_strchr(data, '*'))
	{
		free(data);
		return (original_data);
	}
	else
	{
		free(original_data);
		return (data);
	}
}
