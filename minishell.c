#include <stdio.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>
// #include "/usr/local/opt/readline/include/readline/readline.h"
// #include "/usr/local/opt/readline/include/readline/history.h"
#include <signal.h>
#include <sys/fcntl.h> 
#include <sys/stat.h>
#include <sys/ioctl.h>  

#include "libft/libft.h"
#include "parser/parser.h"
#include "lexer/lexer.h"
#include "expander/expander.h"
#include "execute/execute.h"
#include "utils/get_next_line.h"
#include "execute/exit_status.h"

// 何らかのSIGNAL(Ctrl-C(SIGINT), Ctrl-\(SIGQUIT))を受け取った時の挙動を定義する
static void	signal_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	set_signal_handler(void)
{
	if (signal(SIGINT, &signal_handler) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
	if (signal(SIGQUIT, &signal_handler) == SIG_ERR)
	{
		perror("signal");
		exit(EXIT_FAILURE);
	}
}

int minishell(char *line)
{
	t_env_var	*env_vars;
	int 		exit_status;

	env_vars = init_env_lst();

	if (register_env_var_from_literal("?", "0", 0, &env_vars) == MALLOC_ERROR)
		exit(delete_env_lst(env_vars, NULL, NULL));
	if (line)
		exit(execute(parse(lex(line)), &env_vars));
	exit_status = EXIT_SUCCESS;
	set_signal_handler();
	while (1)
	{
		line = readline("minishell> ");
		if (!line)
			break ;
		// line can be NULL when Ctrl+d
		t_token *token = lex(line);
		t_ast_node *node = parse(token);
		if (!node)
		{
			free(line);
			if (register_env_var(ft_strdup("?"), ft_strdup(ES_SYNTAX_ERROR), &env_vars) == MALLOC_ERROR)
				exit(delete_env_lst(env_vars, NULL, NULL));
			continue;
		}
		exit_status = execute(node, &env_vars);
		add_history(line);
		free(line);
	}
	ft_putstr_fd("exit\n", STDERR_FILENO);
	return (exit_status);
}

int	main(int argc, char **argv)
{
	char			*line;
	t_gnl_status 	status;

	if (argc == 3 && !ft_strcmp(argv[1], "-c"))
		return (minishell(ft_strdup(argv[2])));
	else
	{
		// todo: what if there are two lines
		if (isatty(STDIN_FILENO) == 0)
		{
			status = get_next_line(STDIN_FILENO, &line);
			if (status == GNL_STATUS_ERROR_READ)
				perror_exit("read", EXIT_FAILURE);
			else if (status == GNL_STATUS_ERROR_MALLOC)
				perror_exit("malloc", EXIT_FAILURE);
			if (line[0])
				return (minishell(line));
			free(line);
		}
		return (minishell(NULL));
	}
}
