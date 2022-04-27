#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdio.h>
# include <unistd.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/fcntl.h> 
# include <sys/stat.h>
# include <sys/ioctl.h>  

# include "libft/libft.h"
# include "parser/parser.h"
# include "lexer/lexer.h"
# include "expander/expander.h"
# include "execute/execute.h"
# include "execute/exit_status.h"
# include "env/env.h"
# include "signal/signal.h"

# define BLUE    "\033[1;34m"
# define RESET   "\033[0m"

#endif
