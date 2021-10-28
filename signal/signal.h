#ifndef SIGNAL_H
# define SIGNAL_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <sys/fcntl.h> 
# include <sys/stat.h>
# include <sys/ioctl.h>

# include "../env/env.h"
# include "../wrapper/x.h"

# define READLINE_SIGNAL 0
# define EXECUTE_SIGNAL 1

volatile sig_atomic_t	g_signal;

void	set_signal_handler(int sig_type);

#endif
