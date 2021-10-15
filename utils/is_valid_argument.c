#include "utils.h"

static bool	print_argument_error(char *argv, int error_type)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (error_type == EXPORT_ARG_ERROR)
		ft_putstr_fd("export: `", STDERR_FILENO);
	else if (error_type == UNSET_ARG_ERROR)
		ft_putstr_fd("unset: `", STDERR_FILENO);
	ft_putstr_fd(argv, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
	return (false);
}

bool	is_valid_argument(char *argv, size_t len, int error_type)
{
	size_t			i;

	if (!len)
		return (print_argument_error(argv, error_type));
	i = 0;
	while (i < len)
	{
		if (i == 0 && !ft_isalpha(argv[i]) && argv[i] != '_')
			return (print_argument_error(argv, error_type));
		else if (i != 0 && !ft_isalnum(argv[i]) && argv[i] != '_')
			return (print_argument_error(argv, error_type));
		i++;
	}
	return (true);
}