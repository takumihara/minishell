#include "builtin.h"

int builtin_echo(int argc, char **argv, int no_use)
{
	const bool	nflag = (argc != 1 && !ft_strcmp(argv[1], "-n"));
	int			i;

	(void)no_use;
	i = nflag + 1;
	while (1)
	{
		ft_putstr_fd(argv[i++], STDOUT_FILENO);
		if (i == argc)
			break ;
		ft_putstr_fd(" ", STDOUT_FILENO);
	}
	if (!nflag)
		ft_putstr_fd("\n", STDOUT_FILENO);
	return (EXIT_SUCCESS);
}
