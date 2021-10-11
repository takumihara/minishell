#ifndef BUILTIN_H
#define BUILTIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libft/libft.h"
#include "../utils/utils.h"
#include "../execute/execute.h"

int		builtin_cd(int argc, char **argv, int no_use);
int		builtin_pwd(int argc, char **argv, int no_use);
int		builtin_exit(int argc, char **argv, int last_exit_status);
int		builtin_echo(int argc, char **argv, int no_use);

#endif //BUILTIN_H
