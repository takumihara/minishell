#ifndef MINISHELL_EXECUTE_H
# define MINISHELL_EXECUTE_H

# include <stdlib.h>
# include <stdbool.h>

# include "../ast/ast.h"

# define NO_STATUS -1
# define SUCCESS 0
# define FAILURE 1
# define CONDITION_AND_IF 0
# define CONDITION_OR_IF 1

typedef struct s_executor {
	t_ast_node	*root; // for free()
	int			exit_status;
	int			condition;
}	t_executor;

bool	new_executor(t_executor **e, t_ast_node *root);

#endif //MINISHELL_EXECUTE_H
