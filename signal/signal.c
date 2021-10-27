#include "signal.h"

static void	signal_handler(int signo)
{
	g_signal = signo;
}

void	set_signal_handler(void)
{
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}

int	check_state(void)
{
	if (g_signal == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_done = 1;
	}
	return (0);
}

bool	check_sigint(t_env_var **env_vars, char *line)
{
	if (g_signal == SIGINT)
	{
		g_signal = 0;
		register_env_var_from_literal("?", NULL, EXIT_FAILURE, env_vars);
		free(line);
		return (true);
	}
	return (false);
}
