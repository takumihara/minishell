#include <stdlib.h>
#include "leaks.h"

#if LEAKS

void	end(void)
{
	system("leaks minishell_leaks");
}

#endif