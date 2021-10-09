#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
//#include "/usr/local/opt/readline/include/readline/readline.h"
//#include "/usr/local/opt/readline/include/readline/history.h"
#include <signal.h>

#include "libft/libft.h"
#include "parser/parser.h"
#include "lexer/lexer.h"
#include "expander/expander.h"
#include "execute/execute.h"


//	rl_on_new_line();
//	rl_replace_line("", 0); // <- 現状、何故かこの関数だけがコンパイルエラーとなり動かない
//	rl_redisplay();

// 何らかのSIGNAL(Ctrl-C(SIGINT), Ctrl-\(SIGQUIT))を受け取った時の挙動を定義する
static void	signal_handler(int signo)
{
//	printf("signo: %d\n", signo);
	if (signo == 2)
	{
		rl_on_new_line();
		rl_redisplay();
	}
}

void	set_signal_handler(void)
{
	signal(SIGINT, &signal_handler);
	// signal(SIGQUIT, &signal_handler);
}

int minishell(char *line)
{
	if (line)
		return (execute(expand(parse(lex(line)), NULL)));
	set_signal_handler();
	while (1)
	{
		line = readline("minishell> ");
		// line can be NULL when Ctrl+d
		if (!line || ft_strncmp(line, "exit", 4) == 0)
		{
			free(line);
			ft_putendl_fd("exit", STDERR_FILENO);
			break ;
		}
		t_token *token = lex(line);
		t_ast_node *node = parse(token);
		if (!node)
		{
			fprintf(stderr, RED "parse() returned NULL!\n" RESET);
			free(line);
			continue;
		}
		// todo: No need to pass arguments `char **env`?
		node = expand(node, NULL);
		execute(node);
		add_history(line);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	if (argc == 3 && !ft_strcmp(argv[1], "-c"))
		return (minishell(argv[2]));
	return (minishell(NULL));
}
