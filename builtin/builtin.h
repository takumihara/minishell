#ifndef BUILTIN_H
#define BUILTIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define BUILTIN_MALLOC_ERROR -1

#include "../libft/libft.h"
#include "../utils/utils.h"
#include "../execute/execute.h"
#include "../expander/expander.h"
#include "../env/env.h"

int		builtin_cd(int argc, char **argv, int no_use, t_env_var *env_vars);
int		builtin_pwd(int argc, char **argv, int no_use, t_env_var *env_vars);
int		builtin_exit(int argc, char **argv, int last_exit_status, t_env_var *env_vars);
int		builtin_echo(int argc, char **argv, int no_use, t_env_var *env_vars);
int		builtin_export(int argc, char **argv, int no_use, t_env_var *env_vars);

#endif //BUILTIN_H
