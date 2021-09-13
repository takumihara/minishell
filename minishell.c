#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
//#include "/usr/local/opt/readline/include/readline/readline.h"
//#include "/usr/local/opt/readline/include/readline/history.h"

#include <signal.h>
#include "libft/libft.h"

// 何らかのSIGNAL(Ctrl-C(SIGINT), Ctrl-\(SIGQUIT))を受け取った時の挙動を定義する
static void	signal_handler(int signo)
{
	(void)signo;
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0); // <- 現状、何故かこの関数だけがコンパイルエラーとなり動かない
	rl_redisplay();
}

void	set_signal_handler(void)
{
	signal(SIGINT, &signal_handler);
	// signal(SIGQUIT, &signal_handler);
}

int	main(void)
{
	char	*line;

	set_signal_handler();
	line = NULL;
	while (1)
	{
		line = readline("minishell> ");
		if (ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			break ;
		}
		printf("line is '%s'\n", line);
		add_history(line);
		free(line);
	}
	return (0);
}


