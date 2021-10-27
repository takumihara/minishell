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

volatile sig_atomic_t	g_signal;

void	set_signal_handler(void);
int		check_state(void);
bool	check_sigint(t_env_var **env_vars, char *line);

#endif
