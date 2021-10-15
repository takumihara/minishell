#include "utils.h"

static bool	print_argument_error(char *argv)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(argv, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}

bool	is_valid_argument(char *argv)
{
	const size_t	len = ft_strlen(argv);
	size_t			i;

	if (!len)
		return (print_argument_error(argv));
	i = 0;
	while (i < len)
	{
		if (i == 0 && !ft_isalpha(argv[i]) && argv[i] != '_')
			return (print_argument_error(argv));
		else if (i != 0 && !ft_isalnum(argv[i]) && argv[i] != '_')
			return (print_argument_error(argv));
		i++;
	}
	return (true);
}