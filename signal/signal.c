#include "signal.h"

static void	readline_signal_handler(int signo)
{
	g_signal = signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

static void	execute_signal_handler(int signo)
{
	g_signal = signo;
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	set_signal_handler(int sig_type)
{
	if (sig_type == READLINE_SIGNAL)
		x_signal(SIGINT, &readline_signal_handler);
	else if (sig_type == EXECUTE_SIGNAL)
		x_signal(SIGINT, &execute_signal_handler);
	x_signal(SIGQUIT, SIG_IGN);
}
