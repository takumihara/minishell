#ifndef BUILTIN_H
#define BUILTIN_H

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#include "../libft/libft.h"
#include "../utils/utils.h"
#include "../execute/execute.h"

int		cd(int argc, char **argv);
int		pwd();
int		builtin_exit(int argc, char **argv, int last_exit_status);

#endif //BUILTIN_H
